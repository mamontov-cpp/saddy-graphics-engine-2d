#include <QLineEdit>

#include "actions.h"
#include "sceneactions.h"
#include "scenenodeactions.h"

#include "../qstdstring.h"

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

#include "../anglewidget/anglewidget.h"

#include "../rectwidget/rectwidget.h"

#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiscenenodeblock.h"
#include "../uiblocks/uisceneblock.h"
#include "../uiblocks/uianimationinstanceblock.h"

#include <label.h>
#include <geometry2d.h>

#include <p2d/vector.h>

#include <db/dbdatabase.h>


Q_DECLARE_METATYPE(sad::db::Object*) //-V566

// ============================= PUBLIC METHODS =============================

gui::actions::SceneNodeActions::SceneNodeActions(QObject* parent) : QObject(parent), gui::actions::AbstractActions()
{
    m_rotation = new gui::RotationProcess();
}

gui::actions::SceneNodeActions::~SceneNodeActions()
{
    delete m_rotation;
}


void gui::actions::SceneNodeActions::moveObject(const sad::input::MouseMoveEvent& e)
{
    sad::Point2D direction = e.pos2D() - m_editor->shared()->pivotPoint();
    sad::Rect2D area = m_editor->shared()->oldArea();
    sad::moveBy(direction, area);
    m_editor->shared()->selectedObject()->setProperty("area", area);
    this->updateRegionForNode();
}

void gui::actions::SceneNodeActions::commitObjectMoving(const sad::input::MouseReleaseEvent& e)
{
    sad::input::MouseMoveEvent ev;
    ev.Point3D = e.Point3D;
    this->moveObject(ev);

    sad::SceneNode * node = m_editor->shared()->selectedObject();
    sad::Maybe<sad::Rect2D> newvalue = node->getProperty<sad::Rect2D>("area");
    if (newvalue.exists()) {
        sad::Rect2D nv = newvalue.value();
        sad::Rect2D ov = m_editor->shared()->oldArea();
        bool eq = sad::equal(nv, ov);
        if (!eq)
        {
            m_editor->history()->add(new history::scenenodes::ChangeArea(node, ov, nv));
        }
    }
    m_editor->machine()->enterState("selected");
}

void gui::actions::SceneNodeActions::resizeObject(const sad::input::MouseMoveEvent& e)
{
    sad::Rect2D area = m_editor->shared()->oldRegion();
    
    // Compute movement distance
    sad::Point2D direction = e.pos2D() - m_editor->shared()->pivotPoint();
    sad::p2d::Vector movement = m_editor->shared()->resizingDirection();
    movement *= sad::p2d::scalar(direction, m_editor->shared()->resizingDirection());

    // Apply distance
    const sad::Pair<int, int> & indexes = m_editor->shared()->resizingIndexes();
    area[indexes.p1()] += movement;
    area[indexes.p2()] += movement;

    sad::SceneNode* node = m_editor->shared()->selectedObject(); 
    sad::Maybe<double> angle = node->getProperty<double>("angle");
    if (angle.exists())
    {
        sad::rotate(area, angle.value() * -1);

        node->setProperty("area", area);
        this->updateRegionForNode();
    }
}

void gui::actions::SceneNodeActions::commitObjectResizing(const sad::input::MouseReleaseEvent& e)
{
    sad::input::MouseMoveEvent ev;
    ev.Point3D = e.Point3D;
    this->resizeObject(ev);

    sad::SceneNode * node = m_editor->shared()->selectedObject();
    sad::Maybe<sad::Rect2D> newvalue = node->getProperty<sad::Rect2D>("area");
    if (newvalue.exists()) {
        sad::Rect2D nv = newvalue.value();
        sad::Rect2D ov = m_editor->shared()->oldArea();
        bool eq = sad::equal(nv, ov);
        if (!eq)
        {
            m_editor->history()->add(new history::scenenodes::ChangeArea(node, ov, nv));
        }
    }
    m_editor->machine()->enterState("selected");
}

void gui::actions::SceneNodeActions::navigateOrRotate(const sad::input::MouseWheelEvent& e)
{
    if (m_editor->selection()->isSelectionPending())
    {
        m_editor->selection()->navigateSelection(e);
    }
    else
    {
        this->rotate(e);
    }
}

void gui::actions::SceneNodeActions::rotate(const sad::input::MouseWheelEvent& e)
{
    m_rotation->setEditor(m_editor);

    sad::SceneNode* node = m_editor->shared()->activeObject();
    bool selected = false;
    if (!node)
    {
        node = m_editor->shared()->selectedObject();
        selected = true;
    }
    if (node)
    {
        sad::Maybe<float> maybeangle = node->getProperty<float>("angle");
        if (maybeangle.exists())
        {
            float nextangle = this->computeChangedAngle(maybeangle.value(), e.Delta);
            node->setProperty("angle", nextangle);
			gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
            m_editor->emitClosure(blocked_bind(
                blk->awSceneNodeAngle,
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

void gui::actions::SceneNodeActions::cancelSelection()
{
    m_editor->machine()->enterState("idle");
    m_editor->shared()->setSelectedObject(NULL);
	gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* w = blk->lstSceneObjects;

    void (QListWidget::*setrow)(int) = &QListWidget::setCurrentRow;
    m_editor->emitClosure(blocked_bind(
        w,
        setrow,
        -1
    ));
}

void gui::actions::SceneNodeActions::updateRegionForNode()
{
    core::Shared* s = this->m_editor->shared();
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
			gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
            m_editor->emitClosure( blocked_bind(
                blk->rwSceneNodeRect,
                &gui::rectwidget::RectWidget::setValue,
                newrect
            ));
        }
    }
}

// ============================= PUBLIC SLOTS METHODS =============================

void gui::actions::SceneNodeActions::nameEdited(const QString& name)
{
    sad::String newvalue = Q2STDSTRING(name);
    if (m_editor->shared()->activeObject() != NULL)
    {
        m_editor->shared()->activeObject()->setObjectName(newvalue);
        this->updateSceneNodeName(m_editor->shared()->activeObject());
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::String oldvalue = node->objectName();            
            if (newvalue != oldvalue)
            {
                node->setObjectName(newvalue);
                this->updateSceneNodeName(m_editor->shared()->selectedObject());
                m_editor->history()->add(new history::scenenodes::ChangeName(node, oldvalue, newvalue));
            }            
        }
    }
}

void gui::actions::SceneNodeActions::visibilityChanged(bool state)
{
    bool newvalue = state;
	gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
    if (m_editor->shared()->activeObject() != NULL)
    {
        blk->cbSceneNodeVisible->setCheckState(Qt::Checked);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<bool> oldvalue = node->getProperty<bool>("visible");
            if (oldvalue.exists())
            {
                if (newvalue != oldvalue.value())
                {
                    node->setProperty("visible", newvalue);
                    m_editor->history()->add(history::scenenodes::changeVisibility(
                        node, 
                        oldvalue.value(), 
                        newvalue
                    ));
                }
            }
        }
    }
}

void gui::actions::SceneNodeActions::colorChanged(QColor newcolor)
{
    sad::AColor newvalue;
    core::typeconverters::QColorToSadAColor::convert(newcolor, newvalue);
    if (m_editor->shared()->activeObject() != NULL)
    {
        m_editor->shared()->activeObject()->setProperty("color", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::AColor> oldvalue = node->getProperty<sad::AColor>("color");
            if (oldvalue.exists()) {
                sad::AColor ov = oldvalue.value();
                if (newvalue.r() != ov.r() || newvalue.g() != ov.g() || newvalue.b() != ov.b() || newvalue.a() != ov.a())
                {
                    node->setProperty("color", newvalue);
                    m_editor->history()->add(new history::scenenodes::ChangeColor(node, ov, newvalue));
                }
            }
        }
    }
}


void gui::actions::SceneNodeActions::areaChanged(QRectF newarea)
{
    sad::Rect2D newvalue;
    core::typeconverters::QRectFToSadRect2D::convert(newarea, newvalue);
    if (m_editor->shared()->activeObject() != NULL)
    {
        m_editor->shared()->activeObject()->setProperty("area", newvalue);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
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
                        m_editor->history()->add(new history::scenenodes::ChangeArea(node, ov, newvalue));
                    }
                }
            }
        }
    }
}

void gui::actions::SceneNodeActions::angleChanged(double newvalue)
{
    // Block changing in diagonal placing state
	gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
    if (m_editor->machine()->isInState("adding/sprite_diagonal")
        && m_editor->machine()->isInState("adding/customobject_diagonal"))
    {
        gui::anglewidget::AngleWidget* w = blk->awSceneNodeAngle;
        bool b = w->blockSignals(true);
        w->setValue(0);
        w->blockSignals(b);
        return;
    }

    m_rotation->setEditor(m_editor);

    sad::SceneNode* node = m_editor->shared()->activeObject();
    bool selected = false;
    if (!node)
    {
        node = m_editor->shared()->selectedObject();
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


void gui::actions::SceneNodeActions::removeSceneNode(sad::SceneNode* node, bool from_editor)
{
    int row = m_panel->findSceneNodeInList(node);
    if (row == -1)
    {
        row = static_cast<int>(node->scene()->findLayer(node));
    }
            

    int posininstance = this->findInComboBox<sad::db::Object*>(m_panel->UI()->cmbAnimationInstanceObject, node);
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
        m_editor->history()->add(c);
    } 
    else
    {
        m_editor->currentBatchCommand()->add(c);	
    }
    c->commit(m_editor);
}

void gui::actions::SceneNodeActions::removeSceneNode()
{
    if (m_editor->machine()->isInState("selected"))
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            this->removeSceneNode(node, true);
        }
    }
}


void gui::actions::SceneNodeActions::updateSceneNodeName(sad::SceneNode* s)
{
	gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
	gui::uiblocks::UISceneNodeBlock* nblk = m_editor->uiBlocks()->uiSceneNodeBlock();
    QListWidget* w = blk->lstSceneObjects;
    int row = this->findInList(w, s);
    if (row != -1)
    {
        w->item(row)->setText(this->viewableObjectName(s));
    }
    if (s == m_editor->shared()->selectedObject() || s == m_editor->shared()->activeObject())
    {
		QLineEdit* edit  = nblk->txtObjectName;
        bool b = edit->blockSignals(true);
		std::string on  = s->objectName().c_str(); 
		edit->setText(STD2QSTRING(on));
        edit->blockSignals(b);
    }

	gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock();
	QComboBox* aiobox = aiblk->cmbAnimationInstanceObject;
    int pos = this->findInComboBox<sad::db::Object*>(aiobox, s);
    if (pos > - 1)
    {
        aiobox->setItemText(pos, this->fullNameForNode(s));
    }
}

QString gui::actions::SceneNodeActions::fullNameForNode(sad::SceneNode* node)
{
    QString result;
    if (node->scene())
    {
        result += "[";
        result += m_editor->actions()->sceneActions()->nameForScene(node->scene());
        result += "] ";
    }
    result += this->viewableObjectName(node);
    return result;
}

// ============================= PRIVATE METHODS =============================

float gui::actions::SceneNodeActions::computeChangedAngle(float angle, float delta)
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

