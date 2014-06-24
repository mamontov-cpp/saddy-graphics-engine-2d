#include "colorpicker/colorpicker.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QHeaderView>
#include <QPainter>
#include <QTableWidgetItem>
#include <QTimer>

int ColorPicker::DefaultRowCount = 5;

int ColorPicker::DefaultColumnCount = 5;

int ColorPicker::CellSize = 15;

ColorPicker::ColorPicker(QWidget * parent) 
: QWidget(parent)
{
	m_palette = new QTableWidget(parent);
	m_palette->horizontalHeader()->hide();
	m_palette->verticalHeader()->hide();
	m_palette->setSelectionMode(QAbstractItemView::SingleSelection);
	m_palette->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_palette->setRowCount(ColorPicker::DefaultRowCount);
	m_palette->setColumnCount(ColorPicker::DefaultColumnCount);
	
	for(int i = 0; i < ColorPicker::DefaultColumnCount; i++) {
		m_palette->setColumnWidth(i, ColorPicker::CellSize);
	}

	for(int i = 0; i < ColorPicker::DefaultRowCount; i++) {
		m_palette->setRowHeight(i, ColorPicker::CellSize);
	}

	resizeWidgets(this->geometry());
	initRandomPaletteAndMakeCellsNonEditable();
	connect(m_palette, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(paletteItemChanged(QTableWidgetItem*, QTableWidgetItem*)));
}

ColorPicker::~ColorPicker()
{
	delete m_palette;
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
			m_palette->setRowHeight(m_palette->rowCount() - 1, ColorPicker::CellSize);

			fillRow(m_palette->rowCount() - 1);
			shiftRows();

			++row;
			
			changeselection = true;
		}

		if (column == 0)
		{
			m_palette->setColumnCount(m_palette->columnCount() + 1);
			m_palette->setColumnWidth(m_palette->columnCount() - 1, ColorPicker::CellSize);

			fillColumn(m_palette->columnCount() - 1);
			shiftColumns();

			++column;

			changeselection = true;
		}

		if (row == m_palette->rowCount() - 1)
		{
			m_palette->setRowCount(m_palette->rowCount() + 1);
			m_palette->setRowHeight(m_palette->rowCount() - 1, ColorPicker::CellSize);

			fillRow(m_palette->rowCount() - 1);
		}

		if (column == m_palette->columnCount() - 1)
		{
			m_palette->setColumnCount(m_palette->columnCount() + 1);
			m_palette->setColumnWidth(m_palette->columnCount() - 1, ColorPicker::CellSize);

			fillColumn(m_palette->columnCount() - 1);
		}

		if (changeselection)
		{
			m_row_to_be_set = row;
			m_col_to_be_set = column;
			QTimer::singleShot(0, this, SLOT(setPaletteSelection()));
		}

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
}

void ColorPicker::paintEvent(QPaintEvent * e)
{
	QPainter qpainter (this);
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
	m_palette->setGeometry(r.x(), r.y(), r.width() / 2, r.height());
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