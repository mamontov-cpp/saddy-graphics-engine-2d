#include <QLineEdit>
#include <QPlainTextEdit>

#include <db/custom/customobject.h>

#include "actions.h"
#include "sceneactions.h"
#include "scenenodeactions.h"
#include "customobjectactions.h"
#include "gridactions.h"


#include "../../keytovector.h"
#include "../../closuremethodcall.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"
#include "../../core/shared.h"
#include "../../core/selection.h"
#include "../../core/borders/selectionborder.h"


#include "../../core/typeconverters/qcolortosadacolor.h"
#include "../../core/typeconverters/sadrect2dtoqrectf.h"
#include "../../core/typeconverters/qrectftosadrect2d.h"

#include "../../gui/rotationprocess.h"
#include "../../gui/updateelement.h"
#include "../../gui/mainpanelproxy.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "../../gui/actions/gridactions.h"

//#include "../history/scenenodes/scenenodesnew.h"
#include "../../history/scenenodes/scenenodesremove.h"
#include "../../history/scenenodes/scenenodeschangename.h"
#include "../../history/scenenodes/scenenodeschangevisibility.h"
#include "../../history/scenenodes/scenenodeschangecolor.h"
#include "../../history/scenenodes/scenenodeschangearea.h"
#include "../../history/scenenodes/scenenodeslayerswap.h"
#include "../../history/scenenodes/scenenodesspan.h"

#include "../blockedclosuremethodcall.h"

#include "../anglewidget/anglewidget.h"

#include "../rectwidget/rectwidget.h"

#include "../qt-palette-color-picker/colorpicker.h"

#include "../resourcetreewidget/resourcetreewidget.h"

#include "../fontsizewidget/fontsizewidget.h"


#include "../uiblocks/uiblocks.h"
#include "../uiblocks/uiscenenodeblock.h"
#include "../uiblocks/uisceneblock.h"
#include "../uiblocks/uilabelblock.h"
#include "../uiblocks/uispriteblock.h"
#include "../uiblocks/uicustomobjectblock.h"
#include "../uiblocks/uianimationinstanceblock.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <label.h>
#include <geometry2d.h>

#include <p2d/vector.h>

#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


Q_DECLARE_METATYPE(sad::db::Object*) //-V566
Q_DECLARE_METATYPE(sad::SceneNode*)

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
    sad::Maybe<sad::Rect2D> new_value = node->getProperty<sad::Rect2D>("area");
    if (new_value.exists()) {
        sad::Rect2D nv = new_value.value();
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
    gui::actions::GridActions* ga = m_editor->actions()->gridActions();
    sad::layouts::Grid* g = ga->parentGridFor(node);
    if (g)
    {
        g->update();
        if (m_editor->shared()->selectedGrid() == g)
        {
            ga->updateRegion();
        }
        this->updateRegionForNode();
    }
}

void gui::actions::SceneNodeActions::commitObjectResizing(const sad::input::MouseReleaseEvent& e)
{
    sad::input::MouseMoveEvent ev;
    ev.Point3D = e.Point3D;
    this->resizeObject(ev);

    sad::SceneNode * node = m_editor->shared()->selectedObject();
    sad::Maybe<sad::Rect2D> new_value = node->getProperty<sad::Rect2D>("area");
    if (new_value.exists()) {
        sad::Rect2D nv = new_value.value();
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
            gui::actions::GridActions* ga_actions =  m_editor->actions()->gridActions();
            sad::layouts::Grid* grid = ga_actions->parentGridFor(node);
            if (selected)
            {
                if (grid)
                {
                    grid->update();
                }
            }
            gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
            m_editor->emitClosure(blocked_bind(
                blk->awSceneNodeAngle,
                &gui::anglewidget::AngleWidget::setValue,
                static_cast<double>(nextangle)
            ));
            if (selected)
            {
                if (grid)
                {
                    this->updateRegionForNode();
                }
                if (m_editor->shared()->selectedGrid() == grid)
                {
                    m_editor->actions()->gridActions()->updateRegion();
                }
                m_rotation->start(node, maybeangle.value(), nextangle);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::cancelSelection()
{
    if (m_editor->isInGridEditingState())
    {
        if (m_editor->shared()->selectedGrid())
        {
            m_editor->emitClosure(::bind(
                m_editor->actions()->gridActions(),
                &gui::actions::GridActions::cancelSelection
            ));
            return;
        }
    }
    m_editor->machine()->enterState("idle");
    m_editor->shared()->setSelectedObject(nullptr);
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* w = blk->lstSceneObjects;

    void (QListWidget::*setrow)(int) = &QListWidget::setCurrentRow;
    m_editor->emitClosure(blocked_bind(
        w,
        setrow,
        -1
    ));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::updateRegionForNode()
{
    core::Shared* s = this->m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    if (node == nullptr)
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

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::updateAngleForNode()
{
    core::Shared* s = this->m_editor->shared();
    sad::SceneNode* node = s->activeObject();
    if (node == nullptr)
    {
        node  = s->selectedObject(); 
    }
    if (node)
    {
        sad::Renderer::ref()->lockRendering();
        sad::Maybe<double> maybeangle = node->getProperty<double>("angle");
        sad::Renderer::ref()->unlockRendering();
        if (maybeangle.exists())
        {
            gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
            m_editor->emitClosure( blocked_bind(
                blk->awSceneNodeAngle,
                &gui::anglewidget::AngleWidget::setValue,
                maybeangle.value()
            ));
        }
    }
}

void gui::actions::SceneNodeActions::tryUpdateParentGridForNode(sad::SceneNode* node)
{
    gui::actions::GridActions* ga = m_editor->actions()->gridActions();
    sad::layouts::Grid* parent = ga->parentGridFor(node);
    if (parent)
    {
        parent->update();
        ga->updateParentGridsRecursively(parent, false);
        if (m_editor->isNodeSelected(node))
        {
            this->updateRegionForNode();
        }
    }
}

void gui::actions::SceneNodeActions::tryChangeAreaForActiveOrSelectedNode(const sad::Rect2D& new_area, bool force_update)
{
    sad::Rect2D new_value = new_area;
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("area", new_value);
        if (force_update)
        {
            this->updateRegionForNode();
        }
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::Rect2D> old_value = node->getProperty<sad::Rect2D>("area");
            if (old_value.exists()) {
                sad::Rect2D ov = old_value.value();
                bool eq = sad::equal(ov, new_value);
                if (!eq)
                {
                    node->setProperty("area", new_value);
                    new_value = node->getProperty<sad::Rect2D>("area").value();
                    eq = sad::equal(ov, new_value);
                    bool updated = false;
                    if (!eq) {
                        gui::actions::GridActions* ga = m_editor->actions()->gridActions();
                        sad::layouts::Grid* parent = ga->parentGridFor(node);
                        if (parent != nullptr)
                        {
                            parent->update();
                            ga->updateParentGridsRecursively(parent, true);
                            sad::Rect2D valueafterupdate = node->getProperty<sad::Rect2D>("area").value();
                            if (!sad::equal(valueafterupdate, new_value))
                            {
                                updated = true;
                                this->updateRegionForNode();
                            }
                            eq = sad::equal(valueafterupdate, ov);
                            new_value = valueafterupdate;
                        }
                        if (!eq)
                        {
                            m_editor->history()->add(new history::scenenodes::ChangeArea(node, ov, new_value));
                        }
                    }
                    if ((!updated) && force_update)
                    {
                         this->updateRegionForNode();                        
                    }
                }
            }
        }
    }
}
void gui::actions::SceneNodeActions::tryChangeAreaForCurrentNode(QRectF new_area, bool force_update) 
{
    sad::Rect2D new_value;
    core::typeconverters::QRectFToSadRect2D::convert(new_area, new_value);
    tryChangeAreaForActiveOrSelectedNode(new_value, force_update);
}

void gui::actions::SceneNodeActions::tryChangeAreaViaKeyPress(const sad::input::KeyPressEvent& ev)
{
    sad::Point2D p = keyToVector(ev.Key);
    if (sad::non_fuzzy_zero(p.x()) || sad::non_fuzzy_zero(p.y()))
    {
        sad::SceneNode* g = m_editor->shared()->activeObject();
        if (!g)
        {
            g = m_editor->shared()->selectedObject();
        }
        if (g)
        {
            sad::Maybe<sad::Rect2D> area_maybe = g->getProperty<sad::Rect2D>("area");
            if (area_maybe.exists() && m_editor->actions()->gridActions()->parentGridFor(g) == nullptr)
            {                
                sad::Rect2D r = area_maybe.value();
                for(size_t i = 0; i < 4; i++)
                {
                    r[i] += p;
                }
                this->tryChangeAreaForActiveOrSelectedNode(r, true);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::addSceneNodeToSceneNodeList(sad::SceneNode* s)
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;
    QString name = this->viewableObjectName(s);
    QListWidgetItem* i =  new QListWidgetItem();
    i->setText(name);
    
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    lstSceneObjects->addItem(i);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::removeLastSceneNodeFromSceneNodeList()
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;    
    if (lstSceneObjects->count())
    {
        QListWidgetItem* i = lstSceneObjects->takeItem(lstSceneObjects->count() - 1);
        delete i;
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::insertSceneNodeToSceneNodeList(sad::SceneNode* s, int position)
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;
    
    QString name = this->viewableObjectName(s);
    QListWidgetItem* i =  new QListWidgetItem();
    i->setText(name);
    
    QVariant v;
    v.setValue(s);
    i->setData(Qt::UserRole, v);
    lstSceneObjects->insertItem(position, i);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::removeSceneNodeFromSceneNodeList(int position)
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;    
    QListWidgetItem* i =  lstSceneObjects->takeItem(position);
    delete i;
}

void gui::actions::SceneNodeActions::removeSceneNodeFromSceneNodeListByNode(sad::SceneNode* s)
{
    int position = this->findSceneNodeInList(s);
    if (position >= 0)
    {
        removeSceneNodeFromSceneNodeList(position);
    }
}

void gui::actions::SceneNodeActions::setSceneNodesInList(sad::SceneNode* n1, sad::SceneNode* n2, int pos1, int pos2)
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;
    
    sad::SceneNode* s = this->editor()->shared()->selectedObject();
    lstSceneObjects->item(pos1)->setText(this->viewableObjectName(n1));
    QVariant v1;
    v1.setValue(n1);
    lstSceneObjects->item(pos1)->setData(Qt::UserRole, v1);

    lstSceneObjects->item(pos2)->setText(this->viewableObjectName(n2));
    QVariant v2;
    v2.setValue(n2);
    lstSceneObjects->item(pos2)->setData(Qt::UserRole, v2);

    if (s == n1)
    {
        void (QListWidget::*row)(int) = &QListWidget::setCurrentRow;
        invoke_blocked(lstSceneObjects, row, pos1);
        this->currentSceneNodeChanged(lstSceneObjects->currentRow());
    }
    if (s == n2)
    {
        void (QListWidget::*row)(int) = &QListWidget::setCurrentRow;
        invoke_blocked(lstSceneObjects, row, pos2);
        this->currentSceneNodeChanged(lstSceneObjects->currentRow());
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
int gui::actions::SceneNodeActions::findSceneNodeInList(sad::SceneNode* s)
{
    QListWidget* lstSceneObjects = m_editor->uiBlocks()->uiSceneBlock()->lstSceneObjects;
    return this->findInList(lstSceneObjects, s);    
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::placeFirstPointForSpanning(const sad::input::MouseReleaseEvent& e)
{
    core::Shared* shared = m_editor->shared();
    shared->setPivotPoint(e.pos2D());
    m_editor->panelProxy()->highlightState("Please, place second point...");
    m_editor->machine()->enterState("selected/spanning/secondpoint");
}

void gui::actions::SceneNodeActions::moveSecondPointForSpanning(const sad::input::MouseMoveEvent& e)
{
    core::Shared* shared = m_editor->shared();
    this->spanSelectedObjectBetweenTwoPoints(shared->pivotPoint(), e.pos2D());
    this->updateRegionForNode();
    this->updateAngleForNode();
}

void gui::actions::SceneNodeActions::commitSecondPointForSpanning(const sad::input::MouseReleaseEvent& e)
{
    core::Shared* shared = m_editor->shared();
    sad::SceneNode* node = shared->selectedObject();
    this->spanSelectedObjectBetweenTwoPoints(shared->pivotPoint(), e.pos2D());
    this->updateRegionForNode();
    this->updateAngleForNode();

    sad::Maybe<sad::Rect2D> old_area_maybe = node->getProperty<sad::Rect2D>("area");
    sad::Maybe<double> old_angle_maybe = node->getProperty<double>("angle");
    if (old_area_maybe.exists() && old_angle_maybe.exists()) {
        m_editor->history()->add(new history::scenenodes::Span(
            node,
            shared->oldArea(),
            shared->oldAngle(),
            old_area_maybe.value(),
            old_angle_maybe.value()
        ));
    }
    m_editor->panelProxy()->highlightState("Editing item...");
    m_editor->machine()->enterState("selected");
    m_editor->selectionBorder()->toggleShowHotspot(true);
}

void gui::actions::SceneNodeActions::cancelSpanningObject(const sad::input::KeyPressEvent&)
{
     m_editor->panelProxy()->highlightState("Editing item...");
     m_editor->machine()->enterState("selected");
     core::Shared* shared = m_editor->shared();
     sad::SceneNode* node = shared->selectedObject();
     if (node)
     {
         node->setProperty("area", shared->oldArea());
         node->setProperty("angle", shared->oldAngle());
         this->updateRegionForNode();
         this->updateAngleForNode();
     }
     m_editor->selectionBorder()->toggleShowHotspot(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void gui::actions::SceneNodeActions::spanObjectBetweenTwoPoints(
    sad::SceneNode* node,
    sad::Point2D p1,
    sad::Point2D p2
)
{
    if (node)
    {
        if (node->isInstanceOf("sad::Label"))
        {
            static_cast<sad::Label*>(node)->makeSpanBetweenPoints(p1, p2);
        }
        if (node->isInstanceOf("sad::Sprite2D"))
        {
            static_cast<sad::Sprite2D*>(node)->makeSpanBetweenPoints(p1, p2);
        }
        if (node->isInstanceOf("sad::db::custom::Object"))
        {
            static_cast<sad::db::custom::Object*>(node)->makeSpanBetweenPoints(p1, p2);
        }
    }
}

// ============================= PUBLIC SLOTS METHODS =============================

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::copyPositionAndSizeFromOtherNodeClicked()
{
    sad::SceneNode* object = m_editor->shared()->activeObject();
    if (!object)
    {
       object = m_editor->shared()->selectedObject();
    }
    if (object)
    {
        gui::ChildrenProvider provider;
        provider.setProxy(m_editor->panelProxy());
        provider.doNotRemoveGridChildren();
        provider.excludeMajorId(object->MajorId);
        QVector<QPair<QString, unsigned long long> > pairs = provider.nodeList();
        if (pairs.size() != 0)
        {
            core::Editor::NodePickingDialog dlg = core::Editor::dialogForSelectingNode(
                m_editor->panelAsWidget(),
                "Please, pick a child to add into cell",
                pairs
            );
            if (dlg.Dialog->exec() == QDialog::Accepted)
            {
                int current_item = dlg.NodeList->currentRow();
                if (current_item > -1)
                {
                    unsigned long long id = dlg.NodeList->item(current_item)->data(Qt::UserRole).value<unsigned long long>();
                    sad::Maybe<sad::SceneNode*> maybe_node = sad::Renderer::ref()->database("")->objectByMajorId<sad::SceneNode>(id);
                    if (maybe_node.exists())
                    {
                        sad::Maybe<sad::Rect2D> maybe_rect = maybe_node.value()->getProperty<sad::Rect2D>("area");
                        if (maybe_rect.exists())
                        {
                            QRectF r;
                            const sad::Rect2D& mr = maybe_rect.value();
                            core::typeconverters::SadRect2DToQRectF::convert(mr, r);
                            // Fix for inverse rect
                            if (mr[2].y() < mr[0].y())
                            {
                                r.setTop(mr[2].y());
                                r.setHeight(mr.height());
                            }
                            this->tryChangeAreaForCurrentNode(r, true);
                        }
                    }
                }
            }

            delete dlg.Dialog;
        }
    }
}

void gui::actions::SceneNodeActions::nameEdited(const QString& name)
{
    sad::String new_value = Q2STDSTRING(name);
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setObjectName(new_value);
        this->updateSceneNodeName(m_editor->shared()->activeObject(), false);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::String old_value = node->objectName();            
            if (new_value != old_value)
            {
                node->setObjectName(new_value);
                this->updateSceneNodeName(m_editor->shared()->selectedObject(), false);
                m_editor->history()->add(new history::scenenodes::ChangeName(node, old_value, new_value));
            }            
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::visibilityChanged(bool state)
{
    bool new_value = state;
    gui::uiblocks::UISceneNodeBlock* blk = m_editor->uiBlocks()->uiSceneNodeBlock();
    if (m_editor->shared()->activeObject() != nullptr)
    {
        blk->cbSceneNodeVisible->setCheckState(Qt::Checked);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<bool> old_value = node->getProperty<bool>("visible");
            if (old_value.exists())
            {
                if (new_value != old_value.value())
                {
                    node->setProperty("visible", new_value);
                    m_editor->history()->add(history::scenenodes::changeVisibility(
                        m_editor,
                        node, 
                        old_value.value(), 
                        new_value
                    ));
                }
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::colorChanged(QColor new_color)
{
    sad::AColor new_value;
    core::typeconverters::QColorToSadAColor::convert(new_color, new_value);
    if (m_editor->shared()->activeObject() != nullptr)
    {
        m_editor->shared()->activeObject()->setProperty("color", new_value);
    }
    else
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::AColor> old_value = node->getProperty<sad::AColor>("color");
            if (old_value.exists()) {
                sad::AColor ov = old_value.value();
                if (new_value.r() != ov.r() || new_value.g() != ov.g() || new_value.b() != ov.b() || new_value.a() != ov.a())
                {
                    node->setProperty("color", new_value);
                    m_editor->history()->add(new history::scenenodes::ChangeColor(node, ov, new_value));
                }
            }
        }
    }
}


void gui::actions::SceneNodeActions::areaChanged(QRectF new_area)
{
    this->tryChangeAreaForCurrentNode(new_area, false);
}

void gui::actions::SceneNodeActions::angleChanged(double new_value)
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
            node->setProperty("angle", new_value);
            if (selected)
            {
                gui::actions::GridActions* ga_actions = m_editor->actions()->gridActions();
                sad::layouts::Grid* grid = ga_actions->parentGridFor(node);
                if (grid)
                {
                    grid->update();
                    this->updateRegionForNode();
                    if (m_editor->shared()->selectedGrid() == grid)
                    {
                        ga_actions->updateRegion();
                    }
                }
                m_rotation->start(node, maybeangle.value(), new_value);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::removeSceneNode(sad::SceneNode* node, bool from_editor)
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* w = blk->lstSceneObjects;
    int row = this->findInList(w, node);
    if (row == -1)
    {
        row = static_cast<int>(node->scene()->findLayer(node));
    }
            
    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock(); 
    int posininstance = this->findInComboBox<sad::db::Object*>(aiblk->cmbAnimationInstanceObject, node);
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
    sad::Vector<gui::GridPosition> grids_vector = m_editor->actions()->gridActions()->findRelatedGrids(node);
    c->set(grids_vector);
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

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::selectLastSceneNodeSlot()
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* lst = blk->lstSceneObjects;
    if (lst->count() != 0)
    {
        bool b = lst->blockSignals(true);
        lst->setCurrentRow(lst->count() - 1);
        lst->blockSignals(b);
    }	
}

void gui::actions::SceneNodeActions::updateSceneNodeName(sad::SceneNode* s, bool update_field)
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    gui::uiblocks::UISceneNodeBlock* nblk = m_editor->uiBlocks()->uiSceneNodeBlock();
    QListWidget* w = blk->lstSceneObjects;
    int row = this->findInList(w, s);
    if (row != -1)
    {
        w->item(row)->setText(this->viewableObjectName(s));
    }
    if (update_field)
    {
        if (s == m_editor->shared()->selectedObject() || s == m_editor->shared()->activeObject())
        {
            QLineEdit* edit  = nblk->txtObjectName;
            int pos = edit->cursorPosition();
            bool b = edit->blockSignals(true);
            std::string on  = s->objectName().c_str(); 
            edit->setText(STD2QSTRING(on));
            edit->blockSignals(b);
            edit->setCursorPosition(pos);
        }
    }

    gui::uiblocks::UIAnimationInstanceBlock* aiblk = m_editor->uiBlocks()->uiAnimationInstanceBlock();
    QComboBox* aiobox = aiblk->cmbAnimationInstanceObject;
    int pos = this->findInComboBox<sad::db::Object*>(aiobox, s);
    if (pos > - 1)
    {
        aiobox->setItemText(pos, this->fullNameForNode(s));
    }

    m_editor->actions()->gridActions()->tryUpdateNodeNameInGrid(s);
}

// ReSharper disable once CppMemberFunctionMayBeConst
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

void gui::actions::SceneNodeActions::selectLastSceneNode()
{
    m_editor->emitClosure(bind(this, &gui::actions::SceneNodeActions::selectLastSceneNodeSlot));
}

// ============================= PRIVATE METHODS =============================

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
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

void gui::actions::SceneNodeActions::currentSceneNodeChanged(int index)
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* lstSceneObjects = blk->lstSceneObjects;

    if (m_editor->machine()->isInState("adding")
        || (m_editor->machine()->isInState("selected") 
            && m_editor->machine()->currentState() != "selected"))
    {		
        if (this->editor()->shared()->selectedObject())
        {
            bool b = lstSceneObjects->blockSignals(true);
            int row = this->findSceneNodeInList(this->editor()->shared()->selectedObject());
            if (row != -1)
            {
                lstSceneObjects->setCurrentRow(row);
            }
            lstSceneObjects->blockSignals(b);
            return;
        }
    }
    if (index != -1) 
    {
        QListWidgetItem* i = lstSceneObjects->item(index);
        sad::SceneNode* s = i->data(Qt::UserRole).value<sad::SceneNode*>();

        if (m_editor->machine()->isInState("idle"))
        {
            m_editor->machine()->enterState("selected");
        }
        if (m_editor->machine()->isInState("selected"))
        {
            m_editor->shared()->setSelectedObject(s);
            this->updateUIForSelectedSceneNode();
        }
    }
}


void  gui::actions::SceneNodeActions::sceneNodeMoveBack()
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* lstSceneObjects = blk->lstSceneObjects;

    if (m_editor->machine()->isInState("selected"))
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            int row = lstSceneObjects->currentRow();
            int row2 = this->findSceneNodeInList(node);
            if (row > 0 && row == row2)
            {
                sad::SceneNode* previousnode = lstSceneObjects->item(row - 1)->data(Qt::UserRole).value<sad::SceneNode*>();

                history::Command* c = new history::scenenodes::LayerSwap(node, previousnode, row, row - 1);
                this->m_editor->history()->add(c);
                c->commit(m_editor);

                invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(lstSceneObjects, &QListWidget::setCurrentRow, row - 1);
            }
        }
    }
}

void gui::actions::SceneNodeActions::sceneNodeMoveFront()
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    QListWidget* lstSceneObjects = blk->lstSceneObjects;
    if (m_editor->machine()->isInState("selected"))
    {
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        if (node)
        {
            int row = lstSceneObjects->currentRow();
            int row2 = this->findSceneNodeInList(node);
            if (row < lstSceneObjects->count() - 1 && row > -1 && row == row2)
            {
                sad::SceneNode* nextnode = lstSceneObjects->item(row + 1)->data(Qt::UserRole).value<sad::SceneNode*>();

                history::Command* c = new history::scenenodes::LayerSwap(node, nextnode, row, row + 1);
                this->m_editor->history()->add(c);
                c->commit(m_editor);

                invoke_blocked<QListWidget, void (QListWidget::*)(int), int>(lstSceneObjects, &QListWidget::setCurrentRow, row + 1);
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::updateUIForSelectedSceneNode()
{
    QTimer::singleShot(0, this, SLOT(updateUIForSelectedSceneNodeNow()));
}

void gui::actions::SceneNodeActions::updateUIForSelectedSceneNodeNow()
{
    gui::uiblocks::UISceneBlock* blk = m_editor->uiBlocks()->uiSceneBlock();
    gui::uiblocks::UISceneNodeBlock* nblk = m_editor->uiBlocks()->uiSceneNodeBlock();
    gui::uiblocks::UILabelBlock* lblk = m_editor->uiBlocks()->uiLabelBlock();
    gui::uiblocks::UISpriteBlock* sblk = m_editor->uiBlocks()->uiSpriteBlock();
    gui::uiblocks::UICustomObjectBlock* coblock = m_editor->uiBlocks()->uiCustomObjectBlock();

    QListWidget* lstSceneObjects = blk->lstSceneObjects;
    sad::SceneNode* node = m_editor->shared()->selectedObject();
    if (node)
    {
        // Scene tab
        int row = this->findSceneNodeInList(node);
        if (row != lstSceneObjects->currentRow()) {
            void (QListWidget::*setRow)(int) = &QListWidget::setCurrentRow;
            invoke_blocked(lstSceneObjects, setRow, row);
        }
        invoke_blocked(nblk->txtObjectName, &QLineEdit::setText, STD2QSTRING(node->objectName().c_str()));

        // SceneNode tab
        this->updateRegionForNode();
        sad::Maybe<bool> maybevisible = node->getProperty<bool>("visible");
        if (maybevisible.exists())
        {
            invoke_blocked(nblk->cbSceneNodeVisible, &QCheckBox::setCheckState, (maybevisible.value()) ? Qt::Checked : Qt::Unchecked);
        }
        gui::UpdateElement<double>::with(node, "angle", nblk->awSceneNodeAngle, &gui::anglewidget::AngleWidget::setValue);
        gui::UpdateElement<QColor>::with(node, "color", nblk->clpSceneNodeColor, &gui::colorpicker::ColorPicker::setSelectedColor);

        // Label tab
        gui::UpdateElement<sad::String>::with(node, "font", lblk->rtwLabelFont, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
        gui::UpdateElement<unsigned int>::with(node, "fontsize", lblk->fswLabelFontSize, &gui::fontsizewidget::FontSizeWidget::setValue);
        gui::UpdateElement<float>::with(node, "linespacing", lblk->dsbLineSpacingRatio, &QDoubleSpinBox::setValue);
        gui::UpdateElement<QString>::with(node, "text", lblk->txtLabelText, &QPlainTextEdit::setPlainText);
        gui::UpdateElement<unsigned int>::with(node, "maximallinewidth", lblk->spbMaximalLineWidth, &QSpinBox::setValue);
        gui::UpdateElement<unsigned int>::with(node, "overflowstrategy", lblk->cmbLabelOverflowStrategy, &QComboBox::setCurrentIndex);
        gui::UpdateElement<unsigned int>::with(node, "breaktext", lblk->cmbLabelBreakText, &QComboBox::setCurrentIndex);
        gui::UpdateElement<unsigned int>::with(node, "textellipsisposition", lblk->cmbLabelTextEllipsis, &QComboBox::setCurrentIndex);
        gui::UpdateElement<unsigned int>::with(node, "maximallinescount", lblk->spbMaximalLinesCount, &QSpinBox::setValue);
        gui::UpdateElement<unsigned int>::with(node, "overflowstrategyforlines", lblk->cmbLabelOverflowStrategyForLines, &QComboBox::setCurrentIndex);
        gui::UpdateElement<unsigned int>::with(node, "textellipsispositionforlines", lblk->cmbLabelTextEllipsisForLines, &QComboBox::setCurrentIndex);
        sad::Maybe<bool> maybehasformatting= node->getProperty<bool>("hasformatting");
        if (maybehasformatting.exists())
        {
            invoke_blocked(lblk->cbLabelHasFormatting, &QCheckBox::setCheckState, (maybehasformatting.value()) ? Qt::Checked : Qt::Unchecked);
        }


        // Sprite2D tab
        gui::UpdateElement<sad::String>::with(node, "options", sblk->rtwSpriteSprite, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
        sad::Maybe<bool> maybeflipx = node->getProperty<bool>("flipx");
        if (maybeflipx.exists())
        {
            invoke_blocked(sblk->cbFlipX, &QCheckBox::setCheckState, (maybeflipx.value()) ? Qt::Checked : Qt::Unchecked);
        }
        sad::Maybe<bool> maybeflipy = node->getProperty<bool>("flipy");
        if (maybeflipy.exists())
        {
            invoke_blocked(sblk->cbFlipY, &QCheckBox::setCheckState, (maybeflipy.value()) ? Qt::Checked : Qt::Unchecked);
        }


        // Custom object tab
        gui::UpdateElement<sad::String>::with(node, "schema", coblock->rtwCustomObjectSchemas, &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName);
        if (node->metaData()->canBeCastedTo("sad::db::custom::Object"))
        {
            m_editor->actions()->customObjectActions()->fillCustomObjectProperties(node);
        }
        else
        {
            m_editor->actions()->customObjectActions()->clearCustomObjectPropertiesTable();
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void gui::actions::SceneNodeActions::enterSpanningObjectBetweenTwoPoints()
{
    sad::SceneNode* node = m_editor->shared()->selectedObject();
    if (m_editor->isInEditingState() == false 
        &&  m_editor->shared()->isAnyKindOfAnimationIsRunning() == false
        && 	node != nullptr
        &&  m_editor->machine()->isInState("selected"))
    {
        if (m_editor->actions()->gridActions()->isInGrid(node))
        {
            QMessageBox::critical(nullptr, "Node cannot be spanned", "This node cannot be spanned between two points, because it\'s within a grid");
        }
        sad::Maybe<sad::Rect2D> old_area_maybe = node->getProperty<sad::Rect2D>("area");
        sad::Maybe<double> old_angle_maybe = node->getProperty<double>("angle");
        if (old_area_maybe.exists() && old_angle_maybe.exists()) {
            m_editor->shared()->setOldArea(old_area_maybe.value());
            m_editor->shared()->setOldAngle(old_angle_maybe.value());
            m_editor->selectionBorder()->toggleShowHotspot(false);
            m_editor->machine()->enterState("selected/spanning/firstpoint");
            m_editor->panelProxy()->highlightState("Please, place first point");
        }
    }
}


void gui::actions::SceneNodeActions::spanSelectedObjectBetweenTwoPoints(
    sad::Point2D p1,
    sad::Point2D p2
)
{
    this->spanObjectBetweenTwoPoints(m_editor->shared()->selectedObject(), p1, p2);
}
