#include "colorpicker/colorpicker.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QHeaderView>
#include <QPainter>
#include <QTableWidgetItem>
#include <QTimer>

#define _USE_MATH_DEFINES
#include <math.h>

int ColorPicker::PaletteDefaultRowCount = 5;

int ColorPicker::PaletteDefaultColumnCount = 5;

int ColorPicker::PaletteCellSize = 15;

int ColorPicker::VerticalPadding = 2;

int ColorPicker::HorizontalPadding = 2;

int ColorPicker::PreviewRowCount = 3;

int ColorPicker::PreviewColumnCount = 3;

int ColorPicker::MarkerSize = 3;

int ColorPicker::LightnessGradientWidth = 5;

int ColorPicker::AlphaGradientHeight = 5;

ColorPicker::ColorPicker(QWidget * parent) 
: QWidget(parent)
{
	m_lightness_image = NULL;
	m_alpha_image = NULL;

	m_palette = new QTableWidget(parent);
	m_palette->horizontalHeader()->hide();
	m_palette->verticalHeader()->hide();
	m_palette->setSelectionMode(QAbstractItemView::SingleSelection);
	m_palette->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_palette->setRowCount(ColorPicker::PaletteDefaultRowCount);
	m_palette->setColumnCount(ColorPicker::PaletteDefaultColumnCount);
	
	for(int i = 0; i < ColorPicker::PaletteDefaultColumnCount; i++) {
		m_palette->setColumnWidth(i, ColorPicker::PaletteCellSize);
	}

	for(int i = 0; i < ColorPicker::PaletteDefaultRowCount; i++) {
		m_palette->setRowHeight(i, ColorPicker::PaletteCellSize);
	}

	m_preview = new QTableWidget(parent);
	m_preview->horizontalHeader()->hide();
	m_preview->verticalHeader()->hide();
	m_preview->setSelectionMode(QAbstractItemView::SingleSelection);
	m_preview->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_preview->setRowCount(ColorPicker::PreviewRowCount);
	m_preview->setColumnCount(ColorPicker::PreviewColumnCount);

	m_colors_grid_container = new QWidget(parent);
	m_colors_layout = new QGridLayout(m_colors_grid_container);
	m_colors_layout->setSpacing(2);

	const char labels[4][3] = {
		"R:",
		"G:",
		"B:",
		"A:"
	};
	for(int i = 0; i < 4; i++)  // 4 - amount of items in colorpicker
	{
		m_color_labels[i] = new QLabel(parent);
		m_color_labels[i]->setText(labels[i]);

		m_colors_data[i] = new QSpinBox(parent);
		m_colors_data[i]->setMinimum(0);
		m_colors_data[i]->setMaximum(255);

		m_colors_layout->addWidget(m_color_labels[i], i, 0, Qt::AlignLeft);
		m_colors_layout->addWidget(m_colors_data[i],  i, 1, Qt::AlignRight);
	}

	createPreviewCells();
	resizeWidgets(this->geometry());
	initRandomPaletteAndMakeCellsNonEditable();
	connect(m_palette, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(paletteItemChanged(QTableWidgetItem*, QTableWidgetItem*)));
	connect(m_colors_data[0], SIGNAL(valueChanged(int)), this, SLOT(redChanged(int)));
	connect(m_colors_data[1], SIGNAL(valueChanged(int)), this, SLOT(greenChanged(int)));
	connect(m_colors_data[2], SIGNAL(valueChanged(int)), this, SLOT(blueChanged(int)));
	connect(m_colors_data[3], SIGNAL(valueChanged(int)), this, SLOT(alphaChanged(int)));
	

	m_palette->setCurrentCell(ColorPicker::PaletteDefaultRowCount / 2, ColorPicker::PaletteDefaultColumnCount / 2);
}

QColor ColorPicker::selectedColor()
{
	return QColor(255, 0, 0);
}

ColorPicker::~ColorPicker()
{
	delete m_palette;
	delete m_preview;
	delete m_colors_grid_container;
	
	delete m_lightness_image;
	delete m_alpha_image;
}

void ColorPicker::paletteItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous)
{
	if (current)
	{
		int row = current->row();
		int column = current->column();
		int prevrow = row;
		int prevcolumn = column;
		bool changeselection = false;
		QBrush brush = m_palette->item(prevrow, prevcolumn)->background();
		QColor color = brush.color();
		if (row == 0) 
		{
			m_palette->setRowCount(m_palette->rowCount() + 1);
			m_palette->setRowHeight(m_palette->rowCount() - 1, ColorPicker::PaletteCellSize);

			fillRow(m_palette->rowCount() - 1);
			shiftRows();

			++row;
			
			changeselection = true;
		}

		if (column == 0)
		{
			m_palette->setColumnCount(m_palette->columnCount() + 1);
			m_palette->setColumnWidth(m_palette->columnCount() - 1, ColorPicker::PaletteCellSize);

			fillColumn(m_palette->columnCount() - 1);
			shiftColumns();

			++column;

			changeselection = true;
		}

		if (row == m_palette->rowCount() - 1)
		{
			m_palette->setRowCount(m_palette->rowCount() + 1);
			m_palette->setRowHeight(m_palette->rowCount() - 1, ColorPicker::PaletteCellSize);

			fillRow(m_palette->rowCount() - 1);
		}

		if (column == m_palette->columnCount() - 1)
		{
			m_palette->setColumnCount(m_palette->columnCount() + 1);
			m_palette->setColumnWidth(m_palette->columnCount() - 1, ColorPicker::PaletteCellSize);

			fillColumn(m_palette->columnCount() - 1);
		}

		if (changeselection)
		{
			m_row_to_be_set = row;
			m_col_to_be_set = column;
			QTimer::singleShot(0, this, SLOT(setPaletteSelection()));
		}
		else
		{
			updateFullPreviewTable(row, column);
			updateSilentlyColorParts(row, column);
		}

		m_palette->setStyleSheet(
			QString("QTableWidget::item:selected{ background-color: rgba(%1, %2, %3, %4) }")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue())
			.arg(color.alpha())
		);

		// TODO: Handle selected color
		
	}
}

void  ColorPicker::setPaletteSelection()
{
	bool old = m_palette->blockSignals(true);
	m_palette->clearSelection();
	m_palette->selectionModel()->clearSelection();
	m_palette->setCurrentCell(m_row_to_be_set, m_col_to_be_set);
	m_palette->blockSignals(old);
	m_palette->repaint();

	updateFullPreviewTable(m_row_to_be_set, m_col_to_be_set);
	updateSilentlyColorParts(m_row_to_be_set, m_col_to_be_set);
}

void  ColorPicker::redChanged(int v)
{
	updateColorsInPalettePreviewColorWheel(
		QColor(
			v, 
			m_colors_data[1]->value(),
			m_colors_data[2]->value(),
			m_colors_data[3]->value()
		)
	);
}

void  ColorPicker::greenChanged(int v)
{
	updateColorsInPalettePreviewColorWheel(
		QColor(
			m_colors_data[0]->value(), 
			v,
			m_colors_data[2]->value(),
			m_colors_data[3]->value()
		)
	);
}

void  ColorPicker::blueChanged(int v)
{
	updateColorsInPalettePreviewColorWheel(
		QColor(
			m_colors_data[0]->value(), 
			m_colors_data[1]->value(),
			v,
			m_colors_data[3]->value()
		)
	);
}

void  ColorPicker::alphaChanged(int v)
{
	updateColorsInPalettePreviewColorWheel(
		QColor(
			m_colors_data[0]->value(), 
			m_colors_data[1]->value(),
			m_colors_data[2]->value(),
			v
		)
	);
}

void ColorPicker::paintEvent(QPaintEvent * e)
{
	{
		QPainter qpainter (this);
		if (m_lightness_image)
		{
			qpainter.drawImage(
				QRectF(
					QPointF(this->width() - ColorPicker::MarkerSize - ColorPicker::LightnessGradientWidth ,0), 
					QSizeF(m_lightness_image->width(), m_lightness_image->height())
				),
				*m_lightness_image
			);
		}
		if (m_alpha_image)
		{
			int y = this->height() / 2 
				  - ColorPicker::VerticalPadding 
				  - ColorPicker::MarkerSize 
				  - ColorPicker::AlphaGradientHeight;
			qpainter.drawImage(
				QRectF(
					QPointF(this->width() / 2 + ColorPicker::HorizontalPadding, y), 
					QSizeF(m_alpha_image->width(), m_alpha_image->height())
				),
				*m_alpha_image
			);
		}


		QColor color = this->selectedColor();
		
		QPair<int, int> pair(color.lightness(), color.alpha());
		if (m_color_wheels.contains(pair) == false)
		{
			generateColorWheel(color.lightness(), color.alpha(), m_wheel_size);
		}

		QImage & i = m_color_wheels[pair];

		int colorwheelmaxwidth = this->width() / 2 
							   - ColorPicker::HorizontalPadding
							   - ColorPicker::MarkerSize 
							   - ColorPicker::LightnessGradientWidth;
		int x = this->width() / 2 + ColorPicker::HorizontalPadding + colorwheelmaxwidth / 2 - i.width() / 2;
		int colorwheelmaxheight = this->height() / 2
						     - ColorPicker::VerticalPadding
							 - ColorPicker::MarkerSize 
							 - ColorPicker::AlphaGradientHeight;
		int y = colorwheelmaxheight / 2 - i.height() / 2;
		qpainter.drawImage(
				QRectF(
					QPointF(x, y), 
					QSizeF(i.width(), i.height())
				),
				i
		);
	}
	this->QWidget::paintEvent(e);
}

void ColorPicker::resizeEvent(QResizeEvent * e)
{
	QRect oldrect = this->geometry();
	QRect r(oldrect.x(), oldrect.y(), e->size().width(), e->size().height());
	resizeWidgets(r);
}

void ColorPicker::moveEvent(QMoveEvent * e)
{
	QRect oldrect = this->geometry();	
	QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
	resizeWidgets(r);
}

void ColorPicker::resizeWidgets(const QRect & r)
{
	m_palette->setGeometry(
		r.x(), 
		r.y(), 
		r.width() / 2 - ColorPicker::HorizontalPadding, 
		r.height() / 2 - ColorPicker::VerticalPadding
	);
	m_preview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_preview->verticalHeader()->setResizeMode(QHeaderView::Stretch);
	m_preview->setGeometry(
		r.x(),
		r.y() + r.height() / 2 + ColorPicker::VerticalPadding,
		r.width() / 2 - ColorPicker::HorizontalPadding, 
		r.height() / 2.0  - ColorPicker::VerticalPadding
	);	

	m_colors_grid_container->setGeometry(
		r.x() + r.width() / 2 + ColorPicker::HorizontalPadding, 
		r.y() + r.height() / 2 + ColorPicker::VerticalPadding , 
		r.width() / 2 - ColorPicker::HorizontalPadding, 
		r.height() / 2 - ColorPicker::VerticalPadding
	);

	regenerateImages();
}


void ColorPicker::createPreviewCells()
{
	for(int i = 0; i < m_preview->rowCount(); i++)
	{
		for(int j = 0; j < m_preview->columnCount(); j++)
		{
			QTableWidgetItem * item = new  QTableWidgetItem();
			item->setFlags(item->flags() 
						 & ~Qt::ItemIsEditable 
						 & ~Qt::ItemIsSelectable
						 & ~Qt::ItemIsEnabled
			);
			m_preview->setItem(i, j, item);
		}
	}
}

void ColorPicker::initRandomPaletteAndMakeCellsNonEditable()
{
	for(int i = 0; i < m_palette->rowCount(); i++)
	{
		for(int j = 0; j < m_palette->columnCount(); j++)
		{
			QTableWidgetItem * item = new  QTableWidgetItem();
			item->setBackground(
				QColor( rand() % 255, rand() % 255, rand() % 255, rand() % 255)
			);
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);
			m_palette->setItem(i, j, item);
		}
	}
}

void ColorPicker::fillRow(int row)
{
	for(int j = 0; j < m_palette->columnCount(); j++)
	{
		QTableWidgetItem * item = new  QTableWidgetItem();
		item->setBackground(
			QColor(255, 255, 255)
		);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		m_palette->setItem(row, j, item);
	}
}

void ColorPicker::shiftRows()
{
	for(int i = m_palette->rowCount() - 1; i >= 1; i--)
	{
		for(int j = 0; j < m_palette->columnCount(); j++)
		{
			QTableWidgetItem * previtem = m_palette->item(i - 1, j);
			QTableWidgetItem * curitem = m_palette->item(i, j);
			curitem->setBackground(previtem->background());			
		}
	}

	for(int j = 0; j < m_palette->columnCount(); j++)
	{
		QTableWidgetItem * curitem = m_palette->item(0, j);
		curitem->setBackground(QColor(255, 255, 255));			
	}
}

void ColorPicker::fillColumn(int column)
{
	for(int j = 0; j < m_palette->rowCount(); j++)
	{
		QTableWidgetItem * item = new  QTableWidgetItem();
		item->setBackground(
			QColor(255, 255, 255)
		);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		m_palette->setItem(j, column, item);
	}
}

void ColorPicker::shiftColumns()
{
	for(int i = m_palette->columnCount() - 1; i >= 1; i--)
	{
		for(int j = 0; j < m_palette->rowCount(); j++)
		{
			QTableWidgetItem * previtem = m_palette->item(j, i - 1);
			QTableWidgetItem * curitem = m_palette->item(j, i);
			curitem->setBackground(previtem->background());			
		}
	}

	for(int j = 0; j < m_palette->rowCount(); j++)
	{
		QTableWidgetItem * curitem = m_palette->item(j, 0);
		curitem->setBackground(QColor(255, 255, 255));			
	}
}

void ColorPicker::updateFullPreviewTable(int row, int column)
{
	for(int i = 0; i < ColorPicker::PreviewRowCount; i++)
	{
		for(int j = 0; j < ColorPicker::PreviewColumnCount; j++)
		{
			int krow = i - 1;
			int kcolumn = j - 1;
			
			const QBrush & brush = m_palette->item(
				row + krow, 
				column + kcolumn
			)->background();
			m_preview->item(i, j)->setBackground(brush);
		}
	}
}

void ColorPicker::updateSilentlyColorParts(int row, int column)
{
	const QBrush & brush = m_palette->item(row,column)->background();
	const QColor & c = brush.color();

	int (QColor::*methods[4])() const = {
		&QColor::red,
		&QColor::green,
		&QColor::blue,
		&QColor::alpha
	};
	for(int i = 0; i < 4; i++) 
	{
		bool b = m_colors_data[i]->blockSignals(true);
		m_colors_data[i]->setValue((c.*(methods[i]))());
		m_colors_data[i]->blockSignals(b);
	}
}

void ColorPicker::updateColorsInPalettePreviewColorWheel(const QColor & c)
{
	m_palette->setStyleSheet(
			QString("QTableWidget::item:selected{ background-color: rgba(%1, %2, %3, %4) }")
			.arg(c.red())
			.arg(c.green())
			.arg(c.blue())
			.arg(c.alpha())
	);
	QList<QTableWidgetItem *> items = m_palette->selectedItems();
	if (items.size() != 0) {
		items[0]->setBackground(c);
	}
	m_preview->item(1, 1)->setBackground(c);
}

void ColorPicker::regenerateImages()
{
	int width = this->width() / 2 
	  - ColorPicker::HorizontalPadding 
	  - ColorPicker::MarkerSize
	  - ColorPicker::LightnessGradientWidth;
	int height = this->height() / 2
	  - ColorPicker::VerticalPadding
	  - ColorPicker::MarkerSize
	  - ColorPicker::AlphaGradientHeight;
	m_wheel_size = std::min(width,height);

	regenerateLightnessImage(height);
	regenerateAlphaImage(width);
	m_color_wheels.clear();
}

void ColorPicker::regenerateLightnessImage(int height)
{
	delete m_lightness_image;
	m_lightness_image = new QImage(ColorPicker::LightnessGradientWidth, height, QImage::Format_ARGB32);
	QPainter lightnesspainter(m_lightness_image);
	if (height > 255)
	{
		double lineheight = height / 255.0;
		for(int i = 0 ; i < 255; i++) 
		{
			lightnesspainter.fillRect(
				QRect(
					QPoint(0,  (int)(lineheight * i)), 
					QSize(ColorPicker::LightnessGradientWidth, lineheight)	
				), 
				QColor::fromHsl(255, 255, i)
			);
		}
	}
	else
	{
		float parts = 255.0 / height;
		for(int i = 0; i < height; i++)
		{
			lightnesspainter.fillRect(
				QRect(
					QPoint(0,  i), 
					QSize(ColorPicker::LightnessGradientWidth, 1)	
				), 
				QColor::fromHsl(255, 255, 255 - parts * i)
			);
		}
	}
}

void ColorPicker::regenerateAlphaImage(int width)
{
	delete m_alpha_image;
	m_alpha_image = new QImage(width, ColorPicker::AlphaGradientHeight, QImage::Format_ARGB32);
	QPainter painter(m_alpha_image);
	if (width > 255)
	{
		double linewidth = width / 255.0;
		for(int i = 0 ; i < 255; i++) 
		{
			painter.fillRect(
				QRect(
					QPoint((int)(linewidth * i), 0), 
					QSize(linewidth, ColorPicker::AlphaGradientHeight)	
				), 
				QColor(255, 0, 0, i)
			);
		}
	}
	else
	{
		float parts = 255.0 / width;
		for(int i = 0; i < width; i++)
		{
			painter.fillRect(
				QRect(
					QPoint(i, 0), 
					QSize(1, ColorPicker::AlphaGradientHeight)	
				), 
				QColor(0, 0, 0, i)
			);
		}
	}
}

void ColorPicker::generateColorWheel(int lightness, int alpha, int side)
{
	QImage im(side, side, QImage::Format_ARGB32);
	im.fill(Qt::transparent);
	QPainter painter(&im);
	double halfside = side / 2.0;
	double sidesquared = halfside * halfside;

	for(int x = 0; x < side; x++)
	{
		for(int y = 0; y < side; y++)
		{
			double relx = x - halfside;
			double rely = y - halfside;
			double posx = relx * relx + rely * rely; 
			if (posx <= sidesquared) 
			{
				double hue = 0;
				if (relx != 0)
				{
					hue = atan2(rely, relx) * 180.0 / M_PI;
					if (hue < 0)
					{
						hue = 360.0 + hue;
					}
				}
				else
				{
					hue = (rely > 0) ? 90 : 270;
				}
				double saturation = sqrt(posx) / halfside * 255;
				painter.setPen(QColor::fromHsl(hue, saturation, lightness, alpha));
				painter.drawLine(QPoint(x, y), QPoint(x, y));
			}
		}
	}

	m_color_wheels.insert(QPair<int,int>(lightness, alpha), im.copy());
}
