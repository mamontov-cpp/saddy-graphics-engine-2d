#include "scenenodeactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"
#include "../core/shared.h"
#include "../core/selection.h"

#include "../core/typeconverters/qcolortosadacolor.h"
#include "../core/typeconverters/qrectftosadrect2d.h"

#include "../gui/rotationprocess.h"

#include "../history/scenenodes/scenenodesnew.h"
#include "../history/scenenodes/scenenodesremove.h"
#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodeschangevisibility.h"
#include "../history/scenenodes/scenenodeschangecolor.h"
#include "../history/scenenodes/scenenodeschangearea.h"

#include "../blockedclosuremethodcall.h"

#include <label.h>
#include <geometry2d.h>

#include <p2d/vector.h>

Q_DECLARE_METATYPE(sad::db::Object*)

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

void gui::SceneNodeActions::moveObject(const sad::input::MouseMoveEvent& e)
{
	sad::Point2D direction = e.pos2D() - m_panel->editor()->shared()->pivotPoint();
	sad::Rect2D area = m_panel->editor()->shared()->oldArea();
	sad::moveBy(direction, area);
	m_panel->editor()->shared()->selectedObject()->setProperty("area", area);
	this->updateRegionForNode();
}

void gui::SceneNodeActions::commitObjectMoving(const sad::input::MouseReleaseEvent& e)
{
	sad::input::MouseMoveEvent ev;
	ev.Point3D = e.Point3D;
	this->moveObject(ev);

	sad::SceneNode * node = m_panel->editor()->shared()->selectedObject();
	sad::Maybe<sad::Rect2D> newvalue = node->getProperty<sad::Rect2D>("area");
    if (newvalue.exists()) {
        sad::Rect2D nv = newvalue.value();
		sad::Rect2D ov = m_panel->editor()->shared()->oldArea();
        bool eq = sad::equal(nv, ov);
        if (!eq)
        {
            m_panel->editor()->history()->add(new history::scenenodes::ChangeArea(node, ov, nv));
        }
    }
	m_panel->editor()->machine()->enterState("selected");
}

void gui::SceneNodeActions::resizeObject(const sad::input::MouseMoveEvent& e)
{
	sad::Rect2D area = m_panel->editor()->shared()->oldRegion();
	
	// Compute movement distance
	sad::Point2D direction = e.pos2D() - m_panel->editor()->shared()->pivotPoint();
	sad::p2d::Vector movement = m_panel->editor()->shared()->resizingDirection();
	movement *= sad::p2d::scalar(direction, m_panel->editor()->shared()->resizingDirection());

	// Apply distance
	const sad::Pair<int, int> & indexes = m_panel->editor()->shared()->resizingIndexes();
	area[indexes.p1()] += movement;
	area[indexes.p2()] += movement;

	sad::SceneNode* node = m_panel->editor()->shared()->selectedObject(); 
	sad::Maybe<double> angle = node->getProperty<double>("angle");
	if (angle.exists())
	{
		sad::rotate(area, angle.value() * -1);

		node->setProperty("area", area);
		this->updateRegionForNode();
	}
}

void gui::SceneNodeActions::commitObjectResizing(const sad::input::MouseReleaseEvent& e)
{
	sad::input::MouseMoveEvent ev;
	ev.Point3D = e.Point3D;
	this->resizeObject(ev);

	sad::SceneNode * node = m_panel->editor()->shared()->selectedObject();
	sad::Maybe<sad::Rect2D> newvalue = node->getProperty<sad::Rect2D>("area");
    if (newvalue.exists()) {
        sad::Rect2D nv = newvalue.value();
		sad::Rect2D ov = m_panel->editor()->shared()->oldArea();
        bool eq = sad::equal(nv, ov);
        if (!eq)
        {
            m_panel->editor()->history()->add(new history::scenenodes::ChangeArea(node, ov, nv));
        }
    }
	m_panel->editor()->machine()->enterState("selected");
}

void gui::SceneNodeActions::navigateOrRotate(const sad::input::MouseWheelEvent& e)
{
	if (m_panel->editor()->selection()->isSelectionPending())
	{
		m_panel->editor()->selection()->navigateSelection(e);
	}
	else
	{
		this->rotate(e);
	}
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
                static_cast<double>(nextangle)
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


void gui::SceneNodeActions::removeSceneNode(sad::SceneNode* node, bool from_editor)
{
	int row = m_panel->findSceneNodeInList(node);
	if (row == -1)
	{
		row = static_cast<int>(node->scene()->findLayer(node));
	}
			

	int posininstance = m_panel->findInComboBox<sad::db::Object*>(m_panel->UI()->cmbAnimationInstanceObject, node);
	sad::Vector<sad::db::Object*> objects;
	sad::Renderer::ref()->database("")->table("animationinstances")->objects(objects);
	sad::Vector<sad::animations::Instance*> instances;
	for(size_t i = 0; i < objects.size(); i++)
	{
		sad::db::Object* o = objects[i];
		if (o->isInstanceOf("sad::animations::Instance") || o->isInstanceOf("sad::animations::WayInstance"))
		{
			sad::animations::Instance* instance = static_cast<sad::animations::Instance*>(o);
			if (instance->objectId() == node->MajorId)
			{
				instances << instance;
			}
		}
	}


	history::scenenodes::Remove* c = new history::scenenodes::Remove(node, row);
	c->set(posininstance, instances);
	if (from_editor) 
	{
		m_panel->editor()->history()->add(c);
	} 
	else
	{
		m_panel->editor()->currentBatchCommand()->add(c);	
	}
	c->commit(m_panel->editor());
}

void gui::SceneNodeActions::removeSceneNode()
{
	if (m_panel->editor()->machine()->isInState("selected"))
	{
		sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
		if (node)
		{
			this->removeSceneNode(node, true);
		}
	}
}

// ============================= PRIVATE METHODS =============================

float gui::SceneNodeActions::computeChangedAngle(float angle, float delta)
{
    double onedegree = 1.0 / 180.0 * M_PI;
    double result;
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

