#include "qspritetablewidget.h"
#include <unused.h>

QSpriteTableWidget::QSpriteTableWidget(QComboBox * configCombo,
									   QLayout* layout)
{

	m_configCombo = configCombo;

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
	connect(this->m_configCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)),Qt::UniqueConnection);
	QPalette* palette = new QPalette();
	palette->setColor(QPalette::Highlight, QColor(0,0,255,100));
	m_viewer->setPalette(*palette);

	addToForm(layout);

}

QList<QString> QSpriteTableWidget::getGroups(QString configName)
{
	QList<QString> res;

	QListIterator<CellInfo> i(m_values);
	while(i.hasNext())
	{
		CellInfo cinfo = i.next();
		if (cinfo.config == configName)
		{
			res.append(cinfo.group);
		}
	}

	return res;
}

QList<QString> QSpriteTableWidget::getIndexes(QString configName, QString group)
{
	QList<QString> res;

	QListIterator<CellInfo> i(m_values);
	while(i.hasNext())
	{
		CellInfo cinfo = i.next();
		if (cinfo.config == configName && cinfo.group == group)
		{
			res.append(QString::number(cinfo.index));
		}
	}
	return res;
}


void QSpriteTableWidget::configChanged(int index)
{
	if (index != -1)
	{
		rebuildTable();
	}
}


void QSpriteTableWidget::clear()
{
	m_viewer->clear();
	m_configCombo->clear();

	m_curRow = 0;
	m_curCol = 0;
}
void QSpriteTableWidget::addToForm(QLayout* layout)
{
	layout->addWidget(m_viewer);
}

void QSpriteTableWidget::on_currentCellChanged(UNUSED int currentRow, UNUSED int currentColumn, UNUSED int previousRow, UNUSED int previousColumn )
{
	if (m_viewer->currentItem()!=NULL)
	{
		CellInfo cell = m_viewer->currentItem()->data(Qt::UserRole).value<CellInfo>();
		if (cell.config != "")
		{
			emit spriteSelected(cell.config, cell.group, cell.index);
			m_configCombo->setCurrentIndex(m_configCombo->findText(cell.config));
		}
	}
}




/** Adds new sprite to spriteviewer
\param[in] it Iterator of the sprite DB
*/
void QSpriteTableWidget::add(const AbstractSpriteDatabaseIterator& it)
{


	CellInfo info;
	info.config = it.config();
	info.group = it.group();
	info.index = it.groupIndex();
	info.image = const_cast<AbstractSpriteDatabaseIterator&>(it).image();

	info.image = info.image.scaledToHeight(100);
	if(!isCellExists(info))
	{
		m_values.append(info);
	}


}

bool QSpriteTableWidget::isCellExists(CellInfo& cellInfo)
{
	bool res=false;
	QListIterator<CellInfo> i(m_values);
	while(i.hasNext() && !res)
	{
		CellInfo curCell = i.next();
		if (curCell.config == cellInfo.config &&
			curCell.group == cellInfo.group &&
			curCell.index == cellInfo.index)
		{
			res=true;
		}
	}

	return res;
}

void QSpriteTableWidget::buildCombo()
{
	QListIterator<CellInfo> i(m_values);
	while (i.hasNext())
	{
		CellInfo curCell = i.next();
		if(m_configCombo->findText(curCell.config)==-1)
		{
			m_configCombo->addItem(curCell.config);
		}

	}
	m_configCombo->setCurrentIndex(0);
}

void QSpriteTableWidget::rebuildTable()
{
	m_curRow = 0;
	m_curCol = 0;
	m_viewer->clear();

	QString curConfig = m_configCombo->currentText();

	QListIterator<CellInfo> i(m_values);
	while (i.hasNext())
	{
		CellInfo curCell = i.next();
		if (curCell.config == curConfig)
		{
			QTableWidgetItem* item = new QTableWidgetItem();
			const QVariant& var = QVariant::fromValue<CellInfo>(curCell);
			item->setData(Qt::UserRole, var);
			item->setData(Qt::SizeHintRole, curCell.image.size());

			item->setFlags(item->flags() ^ Qt::ItemIsEditable);
			QSize imgSize = curCell.image.size();
			item->setSizeHint(imgSize);

			m_viewer->setItem(m_curRow, m_curCol, item);

			m_viewer->resizeColumnsToContents();
			m_viewer->resizeRowsToContents();

            if (m_curCol)
                ++m_curRow;
            m_curCol = m_curCol ? 0 : 1;

		}
	}
	if (m_viewer->item(m_curRow, m_curCol) == NULL)
	{
		QTableWidgetItem* dummy = new QTableWidgetItem();
		dummy->setFlags(dummy->flags() ^ Qt::ItemIsEditable);
		m_viewer->setItem(m_curRow, m_curCol, dummy);
	}
	if (m_viewer->rowCount()>0)
	{
		CellInfo firstCell = m_viewer->item(0,0)->data(Qt::UserRole).value<CellInfo>();
		setSelection(QSpriteTableWidgetSelection(curConfig, firstCell.group, firstCell.index));
		m_viewer->setRowCount(m_curRow+1);
	}

}

void QSpriteTableWidget::finishSyncronizing()
{
	clear();
	buildCombo();
	rebuildTable();
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
	int index = m_configCombo->findText(config);
	if (index!=-1)
	{
		m_configCombo->setCurrentIndex(index);
	}

	int row = -1;
	int col = -1;
	int rowCount = m_viewer->rowCount();
	int colCount = m_viewer->columnCount();
	bool flag = false;
	for (int i=0;i<rowCount && !flag;i++)
	{
		for (int j=0;j<colCount && !flag;j++)
		{
			if (m_viewer->item(i,j)!=NULL)
			{
				CellInfo curCell = m_viewer->item(i,j)->data(Qt::UserRole).value<CellInfo>();
				if (curCell.group == sel.group() && curCell.index == sel.index())
				{
					flag = true;
					row = i;
					col = j;
				}
			}
		}
	}

	m_viewer->setCurrentCell(row, col);
}
