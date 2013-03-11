#include "qspritetablewidgetselection.h"

QSpriteTableWidgetSelection::QSpriteTableWidgetSelection(const QString & config, 
														 const QString & group, 
														 int index)
{
	m_config = config;
	m_group = group;
	m_index = index;
}

bool QSpriteTableWidgetSelection::invalid() const
{
	return m_config.length() == 0  || m_group.length() == 0 || m_index < 0;
}

