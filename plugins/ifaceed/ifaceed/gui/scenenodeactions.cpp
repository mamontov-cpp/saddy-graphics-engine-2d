#include "scenenodeactions.h"

#include "../mainpanel.h"

gui::SceneNodeActions::SceneNodeActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	
}

gui::SceneNodeActions::~SceneNodeActions()
{
	
}

void gui::SceneNodeActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::SceneNodeActions::panel() const
{
	return m_panel;
}

