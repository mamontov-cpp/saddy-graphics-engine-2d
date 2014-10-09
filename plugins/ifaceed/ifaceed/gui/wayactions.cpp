#include "wayactions.h"

gui::WayActions::WayActions(QObject* parent) : QObject(parent)
{
	
}

gui::WayActions::~WayActions()
{
	
}

void gui::WayActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::WayActions::panel() const
{
	return m_panel;
}

