#include "qspritetablewidget.h"

QSpriteTableWidget::QSpriteTableWidget(QComboBox * combo, QLayout* layout)
{
	
	m_combo = combo;
	CellDelegate* mydelegate = new CellDelegate();
	m_viewer = new QTableWidget();
	m_viewer->setItemDelegate(mydelegate);
	m_viewer->setRowCount(255);
	m_viewer->setColumnCount(2);
	m_viewer->setSelectionMode(QAbstractItemView::SingleSelection);
	m_viewer->setSelectionBehavior(QAbstractItemView::SelectItems);
	m_curRow = 0;
	m_curCol = 0;
	m_viewer->horizontalHeader()->hide();
	m_viewer->verticalHeader()->hide();
	connect(m_viewer, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(on_currentCellChanged(int, int, int, int)),Qt::UniqueConnection);
	QPalette* palette = new QPalette();
	palette->setColor(QPalette::Highlight, QColor(0,0,255,100));
	m_viewer->setPalette(*palette);

	addToForm(layout);
	
}

void QSpriteTableWidget::clear()
{
	m_viewer->clear();
	m_curRow = 0;
	m_curCol = 0;
}
void QSpriteTableWidget::addToForm(QLayout* layout)
{
	layout->addWidget(m_viewer);
}

void QSpriteTableWidget::on_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	if (m_viewer->currentItem()!=NULL)
	{
		CellInfo cell = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
		emit spriteSelected(cell.config, cell.group, cell.index);
	}
}




/** Adds new sprite to spriteviewer
	\param[in] it Iterator of the sprite DB
 */
void QSpriteTableWidget::add(const AbstractSpriteDatabaseIterator& it)
{

	QTableWidgetItem* item = new QTableWidgetItem();

	
	CellInfo info;
	info.config = it.config();
	info.group = QString::number(m_curRow);
	info.index = m_curCol;
	info.image = const_cast<AbstractSpriteDatabaseIterator&>(it).image();

	info.image = info.image.scaledToHeight(100);

	const QVariant& var = QVariant::fromValue<CellInfo>(info);
	item->setData(Qt::UserRole, var);
	item->setData(Qt::SizeHintRole, info.image.size());

	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	QSize imgSize = info.image.size();
	item->setSizeHint(imgSize);
	m_viewer->setItem(m_curRow, m_curCol, item);
	m_viewer->setCurrentItem(item);
	
	QModelIndex modelIndex = m_viewer->model()->index(m_curRow, info.index);
	QStyleOptionViewItem option;
	m_viewer->resizeColumnsToContents();
	m_viewer->resizeRowsToContents();
	//option.rect = QRect(info.image.width(), indo.image.height(), item);
	//emit sizeHintChanged(modelIndex);

	m_curRow = m_curCol ? ++m_curRow : m_curRow;
	m_curCol = m_curCol ? 0 : 1;

}


/** Return selection object
	\return selection object
 */
QSpriteTableWidgetSelection QSpriteTableWidget::selection()
{
	CellInfo curCellInfo = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
	QSpriteTableWidgetSelection res(curCellInfo.config, curCellInfo.group, curCellInfo.index);
	return res;
}

/** Sets current selection
	\param[in] sel selection object
*/
void QSpriteTableWidget::setSelection(const QSpriteTableWidgetSelection & sel)
{
	QString config = sel.config();
	int index = m_combo->findText(config);
	if (index!=-1)
	{
		m_combo->setCurrentIndex(index);
	}

	int groupNum = sel.group().toInt();
	m_viewer->setCurrentCell(groupNum, sel.index());

}