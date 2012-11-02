#include "qspritetablewidget.h"

QSpriteTableWidget::QSpriteTableWidget(QComboBox * configCombo, QComboBox * groupCombo, QComboBox * indexCombo,
									   QLayout* layout)
{

	m_configCombo = configCombo;
	m_groupCombo = groupCombo;
	m_indexCombo = indexCombo;
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
	connect(this->m_groupCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(groupChanged(int)), Qt::UniqueConnection);
	connect(this->m_indexCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)),Qt::UniqueConnection);
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
		QString config = m_configCombo->currentText();
		QList<QString> listOfGroups = getGroups(config);
		if (!listOfGroups.isEmpty())
		{
			m_groupCombo->clear();
			m_groupCombo->addItems(listOfGroups);
			m_groupCombo->setCurrentIndex(0);
		

			QList<QString> listOfIndexes = getIndexes(config, listOfGroups.at(0));
			m_indexCombo->clear();
			m_indexCombo->addItems(listOfIndexes);
			m_indexCombo->setCurrentIndex(0);

			rebuildTable();
		}
	}
}
void QSpriteTableWidget::groupChanged(int index)
{
	if (index!=-1)
	{
		QString group = m_groupCombo->currentText();
		QList<QString> listOfIndexes = getIndexes(m_configCombo->currentText(), group);
		if (!listOfIndexes.isEmpty())
		{
			m_indexCombo->clear();
			m_indexCombo->addItems(listOfIndexes);
			m_indexCombo->setCurrentIndex(0);

			QSpriteTableWidgetSelection selection(m_configCombo->currentText(), group, listOfIndexes.at(0).toInt());
			setSelection(selection);
			emit spriteSelected(selection.config(),selection.group(),selection.index());
		}
	}
}
void QSpriteTableWidget::indexChanged(int index)
{
	if (index!=-1)
	{
		QSpriteTableWidgetSelection selection(m_configCombo->currentText(), m_groupCombo->currentText(),
			m_indexCombo->itemText(index).toInt());
		setSelection(selection);
		emit spriteSelected(selection.config(),selection.group(),selection.index());
	}
}

void QSpriteTableWidget::clear()
{
	m_viewer->clear();
	m_configCombo->clear();
	m_groupCombo->clear();
	m_indexCombo->clear();
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

		/*disconnect(m_configCombo, SIGNAL(currentIndexChanged(int)),0,0);
		disconnect(m_groupCombo, SIGNAL(currentIndexChanged(int)),0,0);
		disconnect(m_indexCombo, SIGNAL(currentIndexChanged(int)),0,0);
*/

		m_configCombo->setCurrentIndex(m_configCombo->findText(cell.config));
		m_groupCombo->setCurrentIndex(m_groupCombo->findText(cell.group));
		m_indexCombo->setCurrentIndex(m_indexCombo->findText(QString::number(cell.index)));
/*
		connect(this->m_configCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));
		connect(this->m_groupCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(groupChanged(int)));
		connect(this->m_indexCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
*/
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
	info.index = m_curCol;
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

void QSpriteTableWidget::buildCombos()
{
	QListIterator<CellInfo> i(m_values);
	while (i.hasNext())
	{
		CellInfo curCell = i.next();
		if(m_configCombo->findText(curCell.config)==-1)
		{
			m_configCombo->addItem(curCell.config);
		}
		if (m_groupCombo->findText(curCell.group)==-1)
		{
			m_groupCombo->addItem(curCell.group);
		}
		if (m_indexCombo->findText(QString::number(curCell.index))==-1)
		{
			m_indexCombo->addItem(QString::number(curCell.index));
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
			m_viewer->setCurrentItem(item);

			m_viewer->resizeColumnsToContents();
			m_viewer->resizeRowsToContents();

			m_curRow = m_curCol ? ++m_curRow : m_curRow;
			m_curCol = m_curCol ? 0 : 1;

		}
	}
	if (m_viewer->rowCount()>0)
	{
		CellInfo firstCell = m_viewer->item(0,0)->data(Qt::UserRole).value<CellInfo>();
		setSelection(QSpriteTableWidgetSelection(curConfig, firstCell.group, firstCell.index));
		m_viewer->setRowCount(m_curRow);
	}

}

void QSpriteTableWidget::finishSyncronizing()
{
	clear();
	buildCombos();
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

	int indexGroup = m_groupCombo->findText(sel.group());
	QString gr = sel.group();
	if (indexGroup != -1)
	{
		m_groupCombo->setCurrentIndex(indexGroup);
	}

	int indexIndex = m_indexCombo->findText(QString::number(sel.index()));
	if (indexIndex != -1)
	{
		m_indexCombo->setCurrentIndex(indexIndex);
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




