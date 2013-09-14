#include "objectlist.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include <algorithm>

void ScreenObjectList::updateWidget(ScreenTemplate * t, AbstractScreenObject * tw)
{
	m_simple_list.clear();
	AbstractScreenObject * o = t->templateBegin();
	int selected = -1;
	while(o)
	{
		if (o->active()) 
		{
			m_simple_list.push_back(o);
		}
		o = t->templateNext();
	}
	LayerComparator c;
	std::sort(m_simple_list.begin(), m_simple_list.end(), c);
	m_widget->clear();
    for(unsigned int i = 0; i < m_simple_list.size(); i++)
	{
		m_widget->addItem(m_simple_list[i]->description().data());
		if (m_simple_list[i] == tw)
		{
			selected = i;
		}
	}
	m_selfchanged = true;
	m_widget->setCurrentRow(selected);
}
