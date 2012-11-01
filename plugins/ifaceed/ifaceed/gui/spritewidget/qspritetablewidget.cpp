#include "qspritetablewidget.h"

QSpriteTableWidget::QSpriteTableWidget(QComboBox * combo, QLayout* layout)
{
	
	m_combo = combo;
	CellDelegate* mydelegate = new CellDelegate();
	m_viewer = new QTableWidget();
	m_viewer->setItemDelegate(mydelegate);
	m_viewer->setRowCount(64);
	m_viewer->setColumnCount(64);
	m_viewer->setSelectionMode(QAbstractItemView::SingleSelection);
	m_viewer->setSelectionBehavior(QAbstractItemView::SelectItems);
	m_curRow = 0;
	connect(m_viewer, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(on_currentCellChanged(int, int, int, int)),Qt::UniqueConnection);
	//connect(m_viewer, SIGNAL(sizeHintChanged(QModexIndex), mydelegate, SLOT(sizeHint(const QStyleOptionViewItem&, const QModelIndex&)))
	QPalette* palette = new QPalette();
	palette->setColor(QPalette::Highlight, QColor(0,0,255,100));
	m_viewer->setPalette(*palette);

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


/** Search group number by its name in cell->data(Qt::UserRole)
	\param[in] rowName name
	\return group number
 */
int QSpriteTableWidget::findGroupRow(QString rowName)
{
	int res = -1;
	int count = m_viewer->rowCount();
	for(int i=0;i<count;i++)
	{
		if (m_viewer->currentItem()!=NULL)
		{
			CellInfo curCell = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
			if (QString::compare(curCell.group, rowName) == 0 )
			{
				res = i;
				break;
			}
		}
	}

	return res;
}

/** Adds new sprite to spriteviewer
	\param[in] it Iterator of the sprite DB
 */
void QSpriteTableWidget::add(const AbstractSpriteDatabaseIterator& it)
{

	QTableWidgetItem* item = new QTableWidgetItem();
	CellInfo info;
	info.config = it.config();
	info.group = it.group();
	info.index = it.groupIndex();
	info.image = const_cast<AbstractSpriteDatabaseIterator&>(it).image();

	info.image = info.image.scaledToHeight(100);

	const QVariant& var = QVariant::fromValue<CellInfo>(info);
	item->setData(Qt::UserRole, var);
	item->setData(Qt::SizeHintRole, info.image.size());
	int groupNum = findGroupRow(info.group);
	if (groupNum == -1)
	{
		groupNum = m_curRow;
		m_curRow++;
	}
	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	QSize imgSize = info.image.size();
	item->setSizeHint(imgSize);
	m_viewer->setItem(groupNum, info.index, item);
	m_viewer->setCurrentItem(item);
	
	QModelIndex modelIndex = m_viewer->model()->index(groupNum, info.index);
	QStyleOptionViewItem option;
	m_viewer->resizeColumnsToContents();
	m_viewer->resizeRowsToContents();
	//option.rect = QRect(info.image.width(), indo.image.height(), item);
	//emit sizeHintChanged(modelIndex);

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

	int groupNum = findGroupRow(sel.group());
	m_viewer->setCurrentCell(groupNum, sel.index());

}