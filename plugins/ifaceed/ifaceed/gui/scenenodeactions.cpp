#include "scenenodeactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include "../history/scenenodes/scenenodesnew.h"
#include "../history/scenenodes/scenenodeschangename.h"

#include <label.h>
#include <geometry2d.h>

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

void gui::SceneNodeActions::nameEdited(const QString& name)
{
	sad::String newvalue = name.toStdString();
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setObjectName(newvalue);
		m_panel->updateSceneNodeName(m_panel->editor()->shared()->activeObject());
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
			sad::String oldvalue = node->objectName();            
			if (newvalue != oldvalue)
            {
                node->setObjectName(newvalue);
				m_panel->updateSceneNodeName(m_panel->editor()->shared()->selectedObject());
                m_panel->editor()->history()->add(new history::scenenodes::ChangeName(node, oldvalue, newvalue));
            }            
        }
    }
}