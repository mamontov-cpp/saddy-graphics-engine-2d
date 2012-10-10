#include "qspritetablewidget.h"

QSpriteTableWidget::QSpriteTableWidget(QComboBox * combo, const QRectF & tableRect)
{
	m_rect = tableRect;
	m_combo = combo;
	CellDelegate* mydelegate = new CellDelegate();
	m_viewer = new QTableWidget();
	m_viewer->setItemDelegate(mydelegate);

	connect(m_viewer, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(on_currentCellChanged(int, int, int, int)),Qt::UniqueConnection);
}

void QSpriteTableWidget::on_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	CellInfo cell = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
	emit spriteSelected(cell.config, cell.group, cell.index);
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
		CellInfo curCell = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
		if (QString::compare(curCell.group, rowName) == 0 )
		{
			res = i;
			break;
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
	const QVariant& var = QVariant::fromValue<CellInfo>(info);
	item->setData(Qt::UserRole, var);
	int groupNum = findGroupRow(info.group);
	if (groupNum == -1)
	{
		groupNum = m_viewer->rowCount();
	}
	m_viewer->setItem(groupNum, info.index, item);

}
/** Adds to main window anything
	\param[in] w window object
 */
void QSpriteTableWidget::addToForm(QMainWindow* w)
{
	m_viewer->setParent(w);
	m_viewer->setGeometry(m_rect.toRect());
	m_viewer->show();
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