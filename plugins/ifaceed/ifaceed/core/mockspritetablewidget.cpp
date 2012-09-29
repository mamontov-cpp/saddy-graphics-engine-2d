#include "mockspritetablewidget.h"
#include "spritedatabase.h"

void MockSpriteTableWidget::clear()
{
	m_configcombo->clear();
	m_groupcombo->clear();
	m_indexcombo->clear();
	m_values.clear();
}

void MockSpriteTableWidget::connectSlots()
{
	connect(this->m_configcombo, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));
	connect(this->m_groupcombo, SIGNAL(currentIndexChanged(int)), this, SLOT(groupChanged(int)));
	connect(this->m_indexcombo, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

void MockSpriteTableWidget::add(const AbstractSpriteDatabaseIterator & i )
{
	if (m_values.contains(i.config()) == false)
	{
		m_values.insert(i.config(), QHash<QString, QVector<int> >());
	}
	if (m_values[i.config()].contains(i.group()) == false)
	{
		m_values[i.config()].insert(i.group(), QVector<int>());
	}
	if (m_values[i.config()][i.group()].contains(i.groupIndex()) == false)
	{
		m_values[i.config()][i.group()] << i.groupIndex();
	}
	this->rebuildWidget();
}


void MockSpriteTableWidget::rebuildWidget()
{
	m_configcombo->clear();
	m_groupcombo->clear();
	m_indexcombo->clear();
	for (QHash<QString, QHash<QString, QVector<int> > >::iterator it = this->m_values.begin();
		it!= this->m_values.end();
		it++)
	{
		m_configcombo->addItem(it.key());
	}
	if (m_configcombo->count())
	{
		m_configcombo->setCurrentIndex(0);
		m_emit_event = false;
		this->configChanged(0);
		m_emit_event = true;
	}
}

int MockSpriteTableWidget::indexByText(QComboBox * cmb, const QString & text)
{
	for (int i=0;i<cmb->count();i++)
	{
		if (cmb->itemText(i) == text)
			return i;
	}
	return -1;
}

void MockSpriteTableWidget::setSelection( const QSpriteTableWidgetSelection & selection)
{
	int configIndex = indexByText(m_configcombo, selection.config());
	if (configIndex!=-1)
	{
		m_emit_event = false;

		m_configcombo->setCurrentIndex(configIndex);
		configChanged(configIndex);

		m_emit_event = true;

		int groupIndex = indexByText(m_groupcombo, selection.group());
		if (groupIndex!=-1)
		{
			m_groupcombo->setCurrentIndex(groupIndex);
			
			m_emit_event = false; 
			groupChanged(groupIndex);
			m_emit_event = true;

			int spriteIndex = indexByText(m_indexcombo, QString::number(selection.index()));
			if (spriteIndex!=-1)
			{
				m_indexcombo->setCurrentIndex(spriteIndex);
				indexChanged(spriteIndex);
			}
		}
		m_emit_event = true;
	}
}

QSpriteTableWidgetSelection MockSpriteTableWidget::selection()
{
	QString config;
	QString group;
	int index;
	if (m_configcombo->currentIndex()!=-1)
	{
		config = m_configcombo->currentText();
		if (m_groupcombo->currentIndex()!=-1)
		{
			group = m_groupcombo->currentText();
			if (m_indexcombo->currentIndex()!=-1)
			{
				bool ok = false;
				index = m_indexcombo->currentText().toInt(&ok);
			}
		}
	}
	return QSpriteTableWidgetSelection(config,group,index);
}

void MockSpriteTableWidget::configChanged(int index)
{
	if (index!=-1)
	{
		QString config = m_configcombo->itemText(index);
		if (m_values.contains(config))
		{
			QList<QString> groups = m_values[config].keys();
			m_groupcombo->clear();
			for(int i=0;i<groups.count();i++)
			{
				m_groupcombo->addItem(groups[i]);
			}
			if (m_groupcombo->count()!=0)
			{
				m_groupcombo->setCurrentIndex(0);
				bool old_emit_event = m_emit_event;
				m_emit_event = false;
				groupChanged(0);
				m_emit_event = old_emit_event;
			}

			if (m_emit_event)
			{
				QSpriteTableWidgetSelection selection = this->selection();
				emit spriteSelected(selection.config(),selection.group(),selection.index());
			}
			
		}
	}
}


void MockSpriteTableWidget::groupChanged(int index)
{
	if (index!=-1 && m_configcombo->currentIndex()!=-1)
	{
		QString config = m_configcombo->currentText();
		QString group = m_groupcombo->itemText(index);
		bool ok = m_values.contains(config);
		if (ok)
		{
			ok = m_values[config].contains(group);
		}
		if (ok)
		{
			QVector<int> indexes = m_values[config][group];
			m_indexcombo->clear();
			for(int i=0;i<indexes.count();i++)
			{
				m_indexcombo->addItem(QString::number(indexes[i]));
			}
			if (m_indexcombo->count()!=0)
			{
				m_indexcombo->setCurrentIndex(0);
				bool old_emit_event = m_emit_event;
				m_emit_event = false;
				indexChanged(0);
				m_emit_event = old_emit_event;
			}

			if (m_emit_event)
			{
				QSpriteTableWidgetSelection selection = this->selection();
				emit spriteSelected(selection.config(),selection.group(),selection.index());
			}
			
		}
	}
}

void MockSpriteTableWidget::indexChanged(int index)
{
	if (index!=-1 && m_configcombo->currentIndex()!=-1 && m_groupcombo->currentIndex()!=-1)
	{
		QString config = m_configcombo->currentText();
		QString group = m_groupcombo->currentText();
		bool ok = true;
		int groupIndex = m_indexcombo->itemText(index).toInt(&ok);
		ok = m_values.contains(config);
		if (ok)
		{
			ok = m_values[config].contains(group);
			if (ok)
			{
				ok = m_values[config][group].contains(index);
				if (ok && m_emit_event)
				{
					QSpriteTableWidgetSelection selection = this->selection();
					emit spriteSelected(selection.config(),selection.group(),selection.index());
				}
			}
		}
	}
}
