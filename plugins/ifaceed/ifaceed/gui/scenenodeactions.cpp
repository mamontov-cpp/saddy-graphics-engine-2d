#include "scenenodeactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include "../core/typeconverters/qcolortosadacolor.h"
#include "../core/typeconverters/qrectftosadrect2d.h"

#include "../gui/rotationprocess.h"

#include "../history/scenenodes/scenenodesnew.h"
#include "../history/scenenodes/scenenodesremove.h"
#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodeschangevisibility.h"
#include "../history/scenenodes/scenenodeschangecolor.h"
#include "../history/scenenodes/scenenodeschangearea.h"

#include "../../blockedclosuremethodcall.h"

#include <label.h>
#include <geometry2d.h>

// ============================= PUBLIC METHODS =============================

gui::SceneNodeActions::SceneNodeActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
    m_rotation = new gui::RotationProcess();
}

gui::SceneNodeActions::~SceneNodeActions()
{
    delete m_rotation;
}

void gui::SceneNodeActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::SceneNodeActions::panel() const
{
	return m_panel;
}

void gui::SceneNodeActions::rotate(const sad::input::MouseWheelEvent& e)
{
    m_rotation->setEditor(m_panel->editor());

    sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
    bool selected = false;
    if (!node)
    {
        node = m_panel->editor()->shared()->selectedObject();
        selected = true;
    }
    if (node)
    {
        sad::Maybe<float> maybeangle = node->getProperty<float>("angle");
        if (maybeangle.exists())
        {
            float nextangle = this->computeChangedAngle(maybeangle.value(), e.Delta);
            node->setProperty("angle", nextangle);
            m_panel->editor()->emitClosure(blocked_bind(
                m_panel->UI()->awSceneNodeAngle,
                &gui::anglewidget::AngleWidget::setValue,
                (double)nextangle
            ));
            if (selected)
            {
                m_rotation->start(node, maybeangle.value(), nextangle);
            }
        }
    }
}

void gui::SceneNodeActions::cancelSelection()
{
	m_panel->editor()->machine()->enterState("idle");
	m_panel->editor()->shared()->setSelectedObject(NULL);
	QListWidget* w = m_panel->UI()->lstSceneObjects;

	void (QListWidget::*setrow)(int) = &QListWidget::setCurrentRow;
	m_panel->editor()->emitClosure(blocked_bind(
		w,
		setrow,
		-1
	));
}

void gui::SceneNodeActions::updateRegionForNode()
{
	core::Shared* s = this->m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	if (node == NULL)
	{
		node  = s->selectedObject(); 
	}
	if (node)
	{
		sad::Renderer::ref()->lockRendering();
		sad::Maybe<sad::Rect2D> maybearea = node->getProperty<sad::Rect2D>("area");
		sad::Renderer::ref()->unlockRendering();
		if (maybearea.exists())
		{
			const sad::Rect2D & v = maybearea.value();
            QRectF newrect = QRectF(v[0].x(), v[0].y(), v.width(), v.height());
            m_panel->editor()->emitClosure( blocked_bind(
                m_panel->UI()->rwSceneNodeRect,
                &gui::rectwidget::RectWidget::setValue,
                newrect
            ));
		}
	}
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
					m_panel->editor()->history()->add(history::scenenodes::changeVisibility(
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

void gui::SceneNodeActions::angleChanged(double newvalue)
{
	// Block changing in diagonal placing state
	if (m_panel->editor()->machine()->isInState("adding/sprite_diagonal")
		&& m_panel->editor()->machine()->isInState("adding/customobject_diagonal"))
	{
		gui::anglewidget::AngleWidget* w = m_panel->UI()->awSceneNodeAngle;
		bool b = w->blockSignals(true);
		w->setValue(0);
		w->blockSignals(b);
		return;
	}

    m_rotation->setEditor(m_panel->editor());

    sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
    bool selected = false;
    if (!node)
    {
        node = m_panel->editor()->shared()->selectedObject();
        selected = true;
    }
    if (node)
    {
        sad::Maybe<float> maybeangle = node->getProperty<float>("angle");
        if (maybeangle.exists())
        {
            node->setProperty("angle", newvalue);
            if (selected)
            {
                m_rotation->start(node, maybeangle.value(), newvalue);
            }
        }
    }
}

void gui::SceneNodeActions::removeSceneNode()
{
	if (m_panel->editor()->machine()->isInState("selected"))
	{
		sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
		if (node)
		{
			int row = m_panel->findSceneNodeInList(node);
			if (row == -1)
			{
				row = (int)(node->scene()->findLayer(node));
			}
			
			history::Command* c = new history::scenenodes::Remove(node, row);
			m_panel->editor()->history()->add(c);
			c->commit(m_panel->editor());
		}
	}
}

// ============================= PRIVATE METHODS =============================

float gui::SceneNodeActions::computeChangedAngle(float angle, float delta)
{
    double onedegree = 1.0 / 180.0 * M_PI;
    double result = 0;
    if (sad::is_fuzzy_equal(0, angle, 0.001) && delta < 0)
    {
        result = 2 * M_PI - onedegree;
    }
    else
    {
        if (sad::is_fuzzy_equal(2 * M_PI, angle, 0.001) && delta > 0)
        {
            result = onedegree;
        }
        else
        {
            float direction = (delta > 0) ? 1 : -1;
            result = angle + direction * onedegree;
        }
    }
    return result;
}

