#include "scenenodeactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include "../core/typeconverters/qcolortosadacolor.h"
#include "../core/typeconverters/qrectftosadrect2d.h"

#include "../history/scenenodes/scenenodesnew.h"
#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodesvisibilitychange.h"
#include "../history/scenenodes/scenenodescolorchange.h"
#include "../history/scenenodes/scenenodeschangearea.h"


#include <label.h>
#include <geometry2d.h>

// ============================= PUBLIC METHODS =============================

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

// ============================= PUBLIC SLOTS METHODS =============================

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

void gui::SceneNodeActions::visibilityChanged(bool state)
{
	bool newvalue = state;
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
		m_panel->UI()->cbSceneNodeVisible->setCheckState(Qt::Checked);
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
			sad::Maybe<bool> oldvalue = node->getProperty<bool>("visible");
			if (oldvalue.exists())
			{
				if (newvalue != oldvalue.value())
				{
					node->setProperty("visible", newvalue);
					m_panel->editor()->history()->add(history::scenenodes::visibilityChange(
						node, 
						oldvalue.value(), 
						newvalue
					));
				}
			}
        }
    }
}

void gui::SceneNodeActions::colorChanged(QColor newcolor)
{
    sad::AColor newvalue;
    core::typeconverters::QColorToSadAColor::convert(newcolor, newvalue);
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("color", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::AColor> oldvalue = node->getProperty<sad::AColor>("color");
            if (oldvalue.exists()) {
                sad::AColor ov = oldvalue.value();
                if (newvalue.r() != ov.r() || newvalue.g() != ov.g() || newvalue.b() != ov.b() || newvalue.a() != ov.a())
                {
                    node->setProperty("color", newvalue);
                    m_panel->editor()->history()->add(new history::scenenodes::ChangeColor(node, ov, newvalue));
                }
            }
        }
    }
}


void gui::SceneNodeActions::areaChanged(QRectF newarea)
{
    sad::Rect2D newvalue;
    core::typeconverters::QRectFToSadRect2D::convert(newarea, newvalue);
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("area", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::Rect2D> oldvalue = node->getProperty<sad::Rect2D>("area");
            if (oldvalue.exists()) {
                sad::Rect2D ov = oldvalue.value();
                bool eq = sad::equal(ov, newvalue);
                if (!eq)
                {
                    node->setProperty("area", newvalue);
                    newvalue = node->getProperty<sad::Rect2D>("area").value();
                    eq = sad::equal(ov, newvalue);
                    if (!eq) {
                        m_panel->editor()->history()->add(new history::scenenodes::ChangeArea(node, ov, newvalue));
                    }
                }
            }
        }
    }
}
