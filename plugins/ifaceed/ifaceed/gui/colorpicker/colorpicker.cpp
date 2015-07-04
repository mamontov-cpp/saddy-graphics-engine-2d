#include "gui/colorpicker/colorpicker.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QHeaderView>
#include <QPainter>
#include <QTableWidgetItem>
#include <QTimer>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

#ifndef HAVE_QT5
	#define HAVE_QT5 (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif

int gui::colorpicker::ColorPicker::PaletteDefaultRowCount = 5;

int gui::colorpicker::ColorPicker::PaletteDefaultColumnCount = 5;

int gui::colorpicker::ColorPicker::PaletteCellSize = 15;

int gui::colorpicker::ColorPicker::VerticalPadding = 2;

int gui::colorpicker::ColorPicker::HorizontalPadding = 2;

int gui::colorpicker::ColorPicker::PreviewRowCount = 3;

int gui::colorpicker::ColorPicker::PreviewColumnCount = 3;

int gui::colorpicker::ColorPicker::MarkerSize = 3;

int gui::colorpicker::ColorPicker::LightnessGradientWidth = 5;

int gui::colorpicker::ColorPicker::AlphaGradientHeight = 5;

int gui::colorpicker::ColorPicker::ColorWheelSelectionSize = 3;

gui::colorpicker::ColorPicker::ColorPicker(QWidget * parent) 
: QWidget(parent)
{
	m_removing_data = false;
	m_force_selection = false;
	m_lightness_image = NULL;
	m_alpha_image = NULL;
    m_do_not_expand_table = false;

	m_palette = new QTableWidget(parent);
	m_palette->horizontalHeader()->hide();
	m_palette->verticalHeader()->hide();
	m_palette->setSelectionMode(QAbstractItemView::SingleSelection);
	m_palette->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_palette->setRowCount(gui::colorpicker::ColorPicker::PaletteDefaultRowCount);
	m_palette->setColumnCount(gui::colorpicker::ColorPicker::PaletteDefaultColumnCount);
	
	for(int i = 0; i < gui::colorpicker::ColorPicker::PaletteDefaultColumnCount; i++) {
		m_palette->setColumnWidth(i, gui::colorpicker::ColorPicker::PaletteCellSize);
	}

	for(int i = 0; i < gui::colorpicker::ColorPicker::PaletteDefaultRowCount; i++) {
		m_palette->setRowHeight(i, gui::colorpicker::ColorPicker::PaletteCellSize);
	}

	m_preview = new QTableWidget(parent);
	m_preview->horizontalHeader()->hide();
	m_preview->verticalHeader()->hide();
	m_preview->setSelectionMode(QAbstractItemView::SingleSelection);
	m_preview->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_preview->setRowCount(gui::colorpicker::ColorPicker::PreviewRowCount);
	m_preview->setColumnCount(gui::colorpicker::ColorPicker::PreviewColumnCount);

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
	

	m_palette->setCurrentCell(gui::colorpicker::ColorPicker::PaletteDefaultRowCount / 2, gui::colorpicker::ColorPicker::PaletteDefaultColumnCount / 2);
	this->update();
}

QColor gui::colorpicker::ColorPicker::selectedColor() const
{
	QList<QTableWidgetItem *> items = m_palette->selectedItems();
	if (items.count())
	{
		return items[0]->background().color();
	}
	return QColor(255, 255, 255);
}

void gui::colorpicker::ColorPicker::setSelectedColor(const QColor & c)
{
	bool found = false;
	int foundrow = 0, foundcol = 0;
	for(int i = 0; i < m_palette->rowCount(); i++)
	{
		for(int j = 0; j < m_palette->columnCount(); j++)
		{
			if (m_palette->item(i, j)->background().color() == c)
			{
				found = true;
				foundrow = i;
				foundcol = j;
			}
		}
	}

    m_do_not_expand_table = true;

	if (found)
	{        
		m_palette->setCurrentCell(foundrow, foundcol);
	}
	else
	{
		QList<QTableWidgetItem *> items = m_palette->selectedItems();
		int row = 0;
		int column = 0;
		if (items.count() != 0)
		{
			row = items[0]->row();
			column = items[0]->column();
			items[0]->setBackground(c);
			
		}
		else
		{
			bool b = m_palette->blockSignals(true);
			m_palette->setCurrentCell(0, 0);
			m_palette->blockSignals(b);
			QTableWidgetItem * item = m_palette->item(0, 0);
			item->setBackground(c);
		}
		updateFullPreviewTable(row, column);
		updateSilentlyColorParts(row, column);
		this->update();
	}
	m_palette->setStyleSheet(
			QString("QTableWidget::item:selected{ background-color: rgba(%1, %2, %3, %4) }")
			.arg(c.red())
			.arg(c.green())
			.arg(c.blue())
			.arg(c.alpha())
		);	
	m_palette->update();
}

QList<QList<QColor> > gui::colorpicker::ColorPicker::palette() const
{
	QList<QList<QColor> > result;
	for(int i = 0; i < m_palette->rowCount(); i++)
	{
		result << QList<QColor>();
		for(int j = 0; j < m_palette->columnCount(); j++)
		{
			result[i] << m_palette->item(i, j)->background().color(); 
		}
	}
	return result;
}

void gui::colorpicker::ColorPicker::setPalette(const QList<QList<QColor> > & palette)
{
	if (palette.count() < 1)
	{
		qDebug("A palette must at least have one row");
		return;
	}
	if (palette[0].count() < 1)
	{
		qDebug("A palette must at least have one column");
		return;
	}
	m_palette->clearSelection();
	m_removing_data = true;
	m_palette->setRowCount(palette.size());
	int columncount = 0;
	for(int i = 0; i < palette.size(); i++)
	{
		columncount = std::max(0, palette[i].size());
	}
	m_palette->setColumnCount(columncount);
	m_removing_data = false;
	for(int i = 0; i < palette.size(); i++)
	{
		this->fillRow(i);
		for(int j = 0; j < palette[i].size(); j++)
		{
			m_palette->item(i, j)->setBackground(palette[i][j]);
		}
	}

	m_row_to_be_set = 0;
	m_col_to_be_set = 0;
	m_palette->setStyleSheet(
			QString("QTableWidget::item:selected{ background-color: rgba(%1, %2, %3, %4) }")
			.arg(palette[0][0].red())
			.arg(palette[0][0].green())
			.arg(palette[0][0].blue())
			.arg(palette[0][0].alpha())
	);

	for(int i = 0; i < m_palette->columnCount(); i++) {
		m_palette->setColumnWidth(i, gui::colorpicker::ColorPicker::PaletteCellSize);
	}

	for(int i = 0; i < m_palette->rowCount(); i++) {
		m_palette->setRowHeight(i, gui::colorpicker::ColorPicker::PaletteCellSize);
	}

	QTimer::singleShot(0, this, SLOT(setPaletteSelection()));
	emit selectedColorChanged(palette[0][0]);
}

gui::colorpicker::ColorPicker::~ColorPicker()
{
	delete m_palette;
	delete m_preview;
	delete m_colors_grid_container;
	
	delete m_lightness_image;
	delete m_alpha_image;

	for(QHash<int, QHash<int, QImage*> >::iterator it = m_color_wheels.begin();
		it != m_color_wheels.end();
		++it
		)
	{
		QHash<int, QImage*> & h = it.value();
		for(QHash<int, QImage*>::iterator iit = h.begin();
			iit != h.end();
			++iit)
		{
			delete iit.value();
		}
	}
}

void gui::colorpicker::ColorPicker::paletteItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous)
{
	if (m_removing_data)
		return;
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
            if (m_do_not_expand_table == false)
            {
			    m_palette->setRowCount(m_palette->rowCount() + 1);
			    m_palette->setRowHeight(m_palette->rowCount() - 1, gui::colorpicker::ColorPicker::PaletteCellSize);

			    fillRow(m_palette->rowCount() - 1);
			    shiftRows();

    			++row;
            }

			
			changeselection = true;
		}

		if (column == 0)
        {
            if (m_do_not_expand_table == false)
            {
			    m_palette->setColumnCount(m_palette->columnCount() + 1);
			    m_palette->setColumnWidth(m_palette->columnCount() - 1, gui::colorpicker::ColorPicker::PaletteCellSize);

			    fillColumn(m_palette->columnCount() - 1);
			    shiftColumns();
    			++column;
            }

			changeselection = true;
		}

		if (row == m_palette->rowCount() - 1 && m_do_not_expand_table == false)
		{
			m_palette->setRowCount(m_palette->rowCount() + 1);
			m_palette->setRowHeight(m_palette->rowCount() - 1, gui::colorpicker::ColorPicker::PaletteCellSize);

			fillRow(m_palette->rowCount() - 1);
		}

		if (column == m_palette->columnCount() - 1 && m_do_not_expand_table == false)
		{
			m_palette->setColumnCount(m_palette->columnCount() + 1);
			m_palette->setColumnWidth(m_palette->columnCount() - 1, gui::colorpicker::ColorPicker::PaletteCellSize);

			fillColumn(m_palette->columnCount() - 1);
		}
		// Drop selection cache
		m_force_selection = false;

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
			this->update();
		}

		m_palette->setStyleSheet(
			QString("QTableWidget::item:selected{ background-color: rgba(%1, %2, %3, %4) }")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue())
			.arg(color.alpha())
		);	
		emit selectedColorChanged(color);
	}

    if (m_do_not_expand_table)
    {
        m_do_not_expand_table = false;
    }
}

void  gui::colorpicker::ColorPicker::setPaletteSelection()
{
	bool old = m_palette->blockSignals(true);
	m_palette->clearSelection();
	m_palette->selectionModel()->clearSelection();
	m_palette->setCurrentCell(m_row_to_be_set, m_col_to_be_set);
	m_palette->blockSignals(old);
	m_palette->update();

	updateFullPreviewTable(m_row_to_be_set, m_col_to_be_set);
	updateSilentlyColorParts(m_row_to_be_set, m_col_to_be_set);
	this->update();
}

void  gui::colorpicker::ColorPicker::redChanged(int v)
{
	m_force_selection = false;
	QColor color(
			v, 
			m_colors_data[1]->value(),
			m_colors_data[2]->value(),
			m_colors_data[3]->value()
	);
	updateColorsInPalettePreviewColorWheel(color);
	emit selectedColorChanged(color);
}

void  gui::colorpicker::ColorPicker::greenChanged(int v)
{
	m_force_selection = false;
	QColor color(
			m_colors_data[0]->value(), 
			v,
			m_colors_data[2]->value(),
			m_colors_data[3]->value()
	);
	updateColorsInPalettePreviewColorWheel(color);
	emit selectedColorChanged(color);
}

void  gui::colorpicker::ColorPicker::blueChanged(int v)
{
	m_force_selection = false;
	QColor color(
			m_colors_data[0]->value(), 
			m_colors_data[1]->value(),
			v,
			m_colors_data[3]->value()
	);
	updateColorsInPalettePreviewColorWheel(color);
	emit selectedColorChanged(color);
}

void  gui::colorpicker::ColorPicker::alphaChanged(int v)
{
	m_force_selection = false;
	QColor color(
			m_colors_data[0]->value(), 
			m_colors_data[1]->value(),
			m_colors_data[2]->value(),
			v
	);
	updateColorsInPalettePreviewColorWheel(color);
	emit selectedColorChanged(color);
}

void gui::colorpicker::ColorPicker::paintEvent(QPaintEvent * e)
{
	{
		QPainter qpainter (this);
		if (m_lightness_image)
		{
			qpainter.drawImage(m_lightness_image_location, *m_lightness_image);
		}
		if (m_alpha_image)
		{
			qpainter.drawImage(m_alpha_image_location, *m_alpha_image);
		}


		QColor color = this->selectedColor();
		
		bool contains;
		if (m_color_wheels.contains(color.lightness()) == false)
		{
			contains = m_color_wheels[color.lightness()].contains(color.alpha());
		}
		else
		{
			contains = false;
		}
		if (!contains)
		{
			generateColorWheel(color.lightness(), color.alpha(), m_wheel_size);
		}
		QImage & i = *(m_color_wheels[color.lightness()][color.alpha()]);
		qpainter.drawImage(m_color_wheel_location, i);

		// Render color selection in color wheel
		{
			QPointF center = this->colorPositionOnWheel(color);
			qpainter.setPen(Qt::black);
			int d = gui::colorpicker::ColorPicker::ColorWheelSelectionSize / 2;
			int size = gui::colorpicker::ColorPicker::ColorWheelSelectionSize;
			qpainter.drawRect(center.x() - d, center.y() - d, size, size);
		}

		// Render alpha selection
		{
			double alpha = color.alpha() / 255.0;
			double x = m_alpha_image_location.x() + m_alpha_image_location.width() * alpha;
			double y = m_alpha_image_location.bottom();
			QPointF p1(x, y),
					p2(x - gui::colorpicker::ColorPicker::MarkerSize, y + gui::colorpicker::ColorPicker::MarkerSize),
					p3(x + gui::colorpicker::ColorPicker::MarkerSize, y + gui::colorpicker::ColorPicker::MarkerSize);
			qpainter.setPen(Qt::black);
			qpainter.drawLine(p1, p2);
			qpainter.drawLine(p1, p3);
			qpainter.drawLine(p2, p3);
		}

		// Render lightness selection
		{
			double lightness = (255 - color.lightness()) / 255.0;
			double x = m_lightness_image_location.right();
			double y = m_lightness_image_location.y() + m_lightness_image_location.height() * lightness;
			// 1 is a padding to render right part, otherwise it will be clamped
			QPointF p1(x, y),
					p2(x + gui::colorpicker::ColorPicker::MarkerSize - 1, y - gui::colorpicker::ColorPicker::MarkerSize + 1),
					p3(x + gui::colorpicker::ColorPicker::MarkerSize - 1, y + gui::colorpicker::ColorPicker::MarkerSize - 1);
			qpainter.setPen(Qt::black);
			qpainter.drawLine(p1, p2);
			qpainter.drawLine(p1, p3);
			qpainter.drawLine(p2, p3);
		}
	}
	this->QWidget::paintEvent(e);
}

void	gui::colorpicker::ColorPicker::keyPressEvent(QKeyEvent * e)
{
	QColor c = this->selectedColor();
	bool must_handle = false;
	QPointF d(0, 0);
	if (e->key() == Qt::Key_A)
	{
		must_handle = true;
		d = QPointF(-1, 0);
	}
	if (e->key() == Qt::Key_W)
	{
		must_handle = true;
		d = QPointF(0, -1);
	}
	if (e->key() == Qt::Key_S)
	{
		must_handle = true;
		d = QPointF(0, 1);
	}
	if (e->key() == Qt::Key_D)
	{
		must_handle = true;
		d = QPointF(1, 0);
	}
	if (must_handle)
	{	
		QPointF p = colorPositionOnWheel(c);
		p += d;
		QColor result;
		if (handleColorWheelPositionChange(p, result))
		{
			m_force_selection = true;
			m_force_point = p;
			updateColorsInPalettePreviewColorWheel(result);
			updateSilentlyColorParts(result);
		}
	}
	if (e->key() == Qt::Key_F)
	{
		int lightness = c.lightness();
		if (lightness > 0) 
		{
			lightness--;
		}
		QColor newcolor = QColor::fromHsl(c.hue(), c.saturation(), lightness, c.alpha());
		updateColorsInPalettePreviewColorWheel(newcolor);
		updateSilentlyColorParts(newcolor);
	}
	if (e->key() == Qt::Key_R)
	{
		int lightness = c.lightness();
		if (lightness < 255) 
		{
			lightness++;
		}
		QColor newcolor = QColor::fromHsl(c.hue(), c.saturation(), lightness, c.alpha());
		updateColorsInPalettePreviewColorWheel(newcolor);
		updateSilentlyColorParts(newcolor);
	}
	if (e->key() == Qt::Key_G)
	{
		int alpha = c.alpha();
		if (alpha > 0)
		{
			alpha--;
		}
		c.setAlpha(alpha);
		updateColorsInPalettePreviewColorWheel(c);
		updateSilentlyColorParts(c);
	}
	if (e->key() == Qt::Key_T)
	{
		int alpha = c.alpha();
		if (alpha < 255)
		{
			alpha++;
		}
		c.setAlpha(alpha);
		updateColorsInPalettePreviewColorWheel(c);
		updateSilentlyColorParts(c);
	}
	this->QWidget::keyPressEvent(e);
}

void gui::colorpicker::ColorPicker::resizeEvent(QResizeEvent * e)
{
	QRect oldrect = this->geometry();
	QRect r(oldrect.x(), oldrect.y(), e->size().width(), e->size().height());
	resizeWidgets(r);
}

void gui::colorpicker::ColorPicker::moveEvent(QMoveEvent * e)
{
	QRect oldrect = this->geometry();	
	QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
	resizeWidgets(r);
}

void gui::colorpicker::ColorPicker::mouseMoveEvent(QMouseEvent * e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		this->handleMouseEvents(e);
	}
	this->QWidget::mouseMoveEvent(e);
}

void gui::colorpicker::ColorPicker::mousePressEvent(QMouseEvent * e)
{
	setFocus();
	if (e->buttons() & Qt::LeftButton)
	{
		this->handleMouseEvents(e);
	}
	this->QWidget::mousePressEvent(e);
}

void gui::colorpicker::ColorPicker::resizeWidgets(const QRect & r)
{
	m_palette->setGeometry(
		r.x(), 
		r.y(), 
		r.width() / 2 - gui::colorpicker::ColorPicker::HorizontalPadding, 
		r.height() / 2 - gui::colorpicker::ColorPicker::VerticalPadding
	);
#if HAVE_QT5
    m_preview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_preview->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
#else
	m_preview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_preview->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif	
	m_preview->setGeometry(
		r.x(),
		r.y() + r.height() / 2 + gui::colorpicker::ColorPicker::VerticalPadding,
		r.width() / 2 - gui::colorpicker::ColorPicker::HorizontalPadding, 
		r.height() / 2.0  - gui::colorpicker::ColorPicker::VerticalPadding
	);	

	m_colors_grid_container->setGeometry(
		r.x() + r.width() / 2 + gui::colorpicker::ColorPicker::HorizontalPadding, 
		r.y() + r.height() / 2 + gui::colorpicker::ColorPicker::VerticalPadding , 
		r.width() / 2 - gui::colorpicker::ColorPicker::HorizontalPadding, 
		r.height() / 2 - gui::colorpicker::ColorPicker::VerticalPadding
	);

	regenerateImages();	
}


void gui::colorpicker::ColorPicker::createPreviewCells()
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

void gui::colorpicker::ColorPicker::initRandomPaletteAndMakeCellsNonEditable()
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

void gui::colorpicker::ColorPicker::fillRow(int row)
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

void gui::colorpicker::ColorPicker::shiftRows()
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

void gui::colorpicker::ColorPicker::fillColumn(int column)
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

void gui::colorpicker::ColorPicker::shiftColumns()
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

void gui::colorpicker::ColorPicker::updateFullPreviewTable(int row, int column)
{
	for(int i = 0; i < gui::colorpicker::ColorPicker::PreviewRowCount; i++)
	{
		for(int j = 0; j < gui::colorpicker::ColorPicker::PreviewColumnCount; j++)
		{
			int krow = i - 1;
			int kcolumn = j - 1;
			
			int trow = row + krow;
			int tcolumn = column + kcolumn;
			if (trow > -1 && trow < m_palette->rowCount()
				&& tcolumn > -1 && tcolumn < m_palette->columnCount())
			{
				const QBrush & brush = m_palette->item(trow, tcolumn)->background();
				m_preview->item(i, j)->setBackground(brush);
			}
			else
			{
				m_preview->item(i, j)->setBackground(Qt::white);
			}
		}
	}
}

void gui::colorpicker::ColorPicker::updateSilentlyColorParts(int row, int column)
{
	const QBrush & brush = m_palette->item(row,column)->background();
	const QColor & c = brush.color();
	updateSilentlyColorParts(c);
}

void gui::colorpicker::ColorPicker::updateSilentlyColorParts(const QColor & c)
{
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

void gui::colorpicker::ColorPicker::updateColorsInPalettePreviewColorWheel(const QColor & c)
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
	this->update();
}



void gui::colorpicker::ColorPicker::regenerateImages()
{
	int width = this->width() / 2 
	  - gui::colorpicker::ColorPicker::HorizontalPadding 
	  - gui::colorpicker::ColorPicker::MarkerSize
	  - gui::colorpicker::ColorPicker::LightnessGradientWidth;
	int height = this->height() / 2
	  - gui::colorpicker::ColorPicker::VerticalPadding
	  - gui::colorpicker::ColorPicker::MarkerSize
	  - gui::colorpicker::ColorPicker::AlphaGradientHeight;
	m_wheel_size = std::min(width,height);

	regenerateLightnessImage(height);
	regenerateAlphaImage(width);

	m_color_wheels.clear();
	// Recompute color wheel location
	int colorwheelmaxwidth = this->width() / 2 
						   - gui::colorpicker::ColorPicker::HorizontalPadding
						   - gui::colorpicker::ColorPicker::MarkerSize 
						   - gui::colorpicker::ColorPicker::LightnessGradientWidth;
	int x = this->width() / 2 + gui::colorpicker::ColorPicker::HorizontalPadding + colorwheelmaxwidth / 2 - m_wheel_size / 2;
	int colorwheelmaxheight = this->height() / 2
						    - gui::colorpicker::ColorPicker::VerticalPadding
							- gui::colorpicker::ColorPicker::MarkerSize 
							- gui::colorpicker::ColorPicker::AlphaGradientHeight;
	int y = colorwheelmaxheight / 2 - m_wheel_size / 2;
	m_color_wheel_location = QRectF(QPointF(x, y),  QSizeF(m_wheel_size, m_wheel_size));
}

void gui::colorpicker::ColorPicker::regenerateLightnessImage(int height)
{
	delete m_lightness_image;
	m_lightness_image = new QImage(gui::colorpicker::ColorPicker::LightnessGradientWidth, height, QImage::Format_ARGB32);
	QPainter lightnesspainter(m_lightness_image);
	QLinearGradient g(0, 0, gui::colorpicker::ColorPicker::LightnessGradientWidth, height);
	g.setColorAt(0, QColor::fromHsl(255, 255, 255));
	g.setColorAt(1, QColor::fromHsl(255, 255, 0));
	lightnesspainter.fillRect(
		QRect(
			QPoint(0,  0), 
			QSize(gui::colorpicker::ColorPicker::LightnessGradientWidth, height)	
		),
		g
	);
	
	m_lightness_image_location = QRectF(
		QPointF(this->width() - gui::colorpicker::ColorPicker::MarkerSize - gui::colorpicker::ColorPicker::LightnessGradientWidth ,0), 
		QSizeF(m_lightness_image->width(), m_lightness_image->height())
	);
}

void gui::colorpicker::ColorPicker::regenerateAlphaImage(int width)
{
	delete m_alpha_image;
	m_alpha_image = new QImage(width, gui::colorpicker::ColorPicker::AlphaGradientHeight, QImage::Format_ARGB32);
	QPainter painter(m_alpha_image);
	QLinearGradient g(0, 0, width, gui::colorpicker::ColorPicker::AlphaGradientHeight);
	g.setColorAt(0, QColor(255, 0, 0, 0));
	g.setColorAt(1, QColor(255, 0, 0, 255));
	painter.fillRect(
		QRect(
			QPoint(0,  0), 
			QSize(width, gui::colorpicker::ColorPicker::AlphaGradientHeight)	
		),
		g
	);
	
	int y = this->height() / 2 
		  - gui::colorpicker::ColorPicker::VerticalPadding 
		  - gui::colorpicker::ColorPicker::MarkerSize 
		  - gui::colorpicker::ColorPicker::AlphaGradientHeight;
	m_alpha_image_location = QRectF(
		QPointF(this->width() / 2 + gui::colorpicker::ColorPicker::HorizontalPadding, y), 
		QSizeF(m_alpha_image->width(), m_alpha_image->height())
	);
}

void gui::colorpicker::ColorPicker::generateColorWheel(int lightness, int alpha, int side)
{
	QImage* im = new QImage(side, side, QImage::Format_ARGB32);
	im->fill(Qt::transparent);
	QPainter painter(im);
	double halfside = side / 2.0;
	double sidesquared = halfside * halfside;

	for(int x = 0; x < side; x++)
	{
		for(int y = 0; y < side; y++)
		{
			double relx = x - halfside;
			double rely = halfside - y;
			double posx = relx * relx + rely * rely; 
			if (posx <= sidesquared) 
			{
				double hue;
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
	if (m_color_wheels.contains(lightness) == false)
	{
		m_color_wheels.insert(lightness, QHash<int, QImage*>());
	}
	m_color_wheels[lightness].insert(alpha, im);
}

void gui::colorpicker::ColorPicker::handleMouseEvents(QMouseEvent* e)
{
	QColor color = this->selectedColor();

#define IS_WITHIN(A, R) ((A)->x() >= (R).left()  \
					&&  (A)->x() <= (R).right()  \
					&&  (A)->y() >= (R).top()    \
					&&  (A)->y() <= ((R).bottom() + gui::colorpicker::ColorPicker::MarkerSize))
	if (IS_WITHIN(e, m_alpha_image_location))
	{
		double alpha = (e->x() - m_alpha_image_location.left()) / m_alpha_image_location.width() * 255.0;
		color.setAlpha(alpha);
		updateColorsInPalettePreviewColorWheel(color);
		updateSilentlyColorParts(color);
		emit selectedColorChanged(color);
	}
#undef IS_WITHIN
#define IS_WITHIN(A, R) ((A)->x() >= (R).left()  \
					&&  (A)->x() <= ((R).right() + gui::colorpicker::ColorPicker::MarkerSize) \
					&&  (A)->y() >= (R).top()    \
					&&  (A)->y() <= ((R).bottom()))
	if (IS_WITHIN(e, m_lightness_image_location))
	{
		double ky = e->y();
		double dy = (ky - m_lightness_image_location.top());
		double rellightness = dy / m_lightness_image_location.height();
		double lightness = rellightness * 255.0;
		lightness = 255 - lightness;
		if (lightness < 0)
		{
			lightness = 0;
		}
		color = QColor::fromHsl(color.hue(), color.saturation(), lightness, color.alpha());
		updateColorsInPalettePreviewColorWheel(color);
		updateSilentlyColorParts(color);
		emit selectedColorChanged(color);
	}

	if (IS_WITHIN(e, m_color_wheel_location))
	{
		QColor result;
#if HAVE_QT5
		QPointF local_pos = e->localPos();
#else
		QPointF local_pos = e->posF();
#endif 		
		if (handleColorWheelPositionChange(local_pos, result))
		{
			m_force_selection = true;
			m_force_point = local_pos;
			updateColorsInPalettePreviewColorWheel(result);
			updateSilentlyColorParts(result);
			emit selectedColorChanged(result);
		}
	}

#undef IS_WITHIN
}

QPointF gui::colorpicker::ColorPicker::colorPositionOnWheel(const QColor & c)
{
	double hue = c.hue() / 180.0 * M_PI;
	double saturation = c.saturation() / 255.0 * m_wheel_size / 2;
	QPointF center = m_color_wheel_location.center();
	center += QPointF(saturation * cos(hue) , saturation * sin(hue) * -1);
	if (m_force_selection)
	{
		center = m_force_point;
	}
	return center;
}

bool gui::colorpicker::ColorPicker::handleColorWheelPositionChange(const QPointF & p, QColor & c)
{
	QColor color = this->selectedColor();
	double halfsize = m_color_wheel_location.width() / 2;
	double dx = p.x() - m_color_wheel_location.center().x();
	double dy = p.y() - m_color_wheel_location.center().y();
	double dist2 = dx * dx + dy * dy; 
	bool result = false;
	if (dist2 <= halfsize * halfsize)
	{
		double saturation = sqrt(dist2) / halfsize * 255.0;
		if (saturation > 255)
		{
			saturation = 255;
		}
		double hue;
		if (fabs(dx) > 0.001)
		{
			hue = atan2(-dy, dx);
			if (hue < 0)
			{
				hue = 2 * M_PI + hue;
			}
			hue = hue / M_PI * 180.0;
		}
		else
		{
			hue = (-dy > 0) ? 90 : 270;
		}
		c = QColor::fromHsl(hue, saturation, color.lightness(), color.alpha());		
		result = true;
	}
	return result;
}
