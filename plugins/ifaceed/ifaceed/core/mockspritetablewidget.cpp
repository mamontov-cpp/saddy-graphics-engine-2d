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
		m_values[i.config()].insert(i.config(), QVector<int>());
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

}


void MockSpriteTableWidget::groupChanged(int index)
{

}

void MockSpriteTableWidget::indexChanged(int index)
{

}
