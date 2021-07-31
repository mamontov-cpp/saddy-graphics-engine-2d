#include "selection.h"

#include "editor.h"

#include "history/scenenodes/scenenodesremove.h"

#include "borders/selectionborder.h"

#include "gui/actions/wayactions.h"
#include "gui/actions/actions.h"
#include "gui/actions/scenenodeactions.h"
#include "gui/actions/sceneactions.h"
#include "gui/actions/gridactions.h"

#include "gui/uiblocks/uiblocks.h"
#include "gui/uiblocks/uiwayblock.h"

#include "gui/mainpanelproxy.h"
#include "gui/rendergrids.h"

#include "../closuremethodcall.h"

#include <layouts/grid.h>
#include <geometry2d.h>

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

// =========================================== PUBLIC METHODS =========================================== 

core::Selection::Selection() 
: m_editor(nullptr),
  m_scene_node_selection_change(false), 
  m_scene_node_current_position(0),
  m_grid_selection_change(false), 
  m_current_grid_chain_position(0)
{
     connect(&m_scene_node_nav_timer, SIGNAL(timeout()), this, SLOT(disableSceneNodeSelectionNavigation()));
     connect(&m_grid_nav_timer, SIGNAL(timeout()), this, SLOT(disableGridSelectionNavigation()));
}

core::Selection::~Selection()
{
    
}

void core::Selection::setEditor(core::Editor* e)
{
    m_editor = e;
}

core::Editor* core::Selection::editor() const
{
    return m_editor;
}

const int core::Selection::TIMEOUT = 700;


void core::Selection::trySelect(const sad::input::MousePressEvent& e)
{
    if (this->m_editor->isInWaysEditingState())
    {
        this->trySelectWay(e);
    }
    if (this->m_editor->isInObjectEditingState())
    {
        if (this->m_editor->isInGridEditingState())
        {
            this->trySelectGrid(e);
        } 
        else
        {
            this->trySelectObject(e);
        }
    }
}

void core::Selection::navigateSelection(const sad::input::MouseWheelEvent& e)
{
    // Do not allow changing item, when object is being edited
    if (this->m_editor->isInEditingState())
    {
        return;
    }

    if (m_editor->isInGridEditingState())
    {
        if (m_grid_selection_change)
        {
            this->navigateGridSelection(e);
        }
    }
    else
    {
        if (m_scene_node_selection_change) 
        {
            this->navigateSceneNodeSelection(e);
        }
    }
}

void core::Selection::navigateSceneNodeSelection(const sad::input::MouseWheelEvent& e)
{
    int last_position = m_scene_node_current_position;
    if (e.Delta > 0)
    {
        if (m_scene_node_current_position == m_scene_node_selection_chain.size() - 1)
        {
            m_scene_node_current_position = 0;
        }
        else
        {
            m_scene_node_current_position++;
        }
    }
    else
    {
        if (m_scene_node_current_position == 0)
        {
            m_scene_node_current_position = m_scene_node_selection_chain.size() - 1;
        }
        else
        {
            m_scene_node_current_position--;
        }
    }

    if (last_position != m_scene_node_current_position && m_scene_node_selection_chain.size())
    {
        if (m_scene_node_current_position > -1 && m_scene_node_current_position < m_scene_node_selection_chain.size())
        {
            m_editor->shared()->setSelectedObject(m_scene_node_selection_chain[m_scene_node_current_position]);
            m_editor->machine()->enterState("selected");
            m_editor->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
            m_editor->emitClosure( bind(this, &core::Selection::startSceneNodeNavigationTimer));
        }
    }
}

void core::Selection::navigateGridSelection(const sad::input::MouseWheelEvent& e)
{
    int last_position = m_current_grid_chain_position;
    if (e.Delta > 0)
    {
        if (m_current_grid_chain_position == m_grid_selection_chain.size() - 1)
        {
            m_current_grid_chain_position = 0;
        }
        else
        {
            m_current_grid_chain_position++;
        }
    }
    else
    {
        if (m_current_grid_chain_position == 0)
        {
            m_current_grid_chain_position = m_grid_selection_chain.size() - 1;
        }
        else
        {
            m_current_grid_chain_position--;
        }
    }

    if (last_position != m_current_grid_chain_position && m_grid_selection_chain.size())
    {
        if (m_current_grid_chain_position > -1 && m_current_grid_chain_position < m_grid_selection_chain.size())
        {
            m_editor->shared()->setSelectedGrid(m_grid_selection_chain[m_current_grid_chain_position]);
            m_editor->actions()->gridActions()->updateGridPropertiesInUI(false);
            m_editor->emitClosure( bind(this, &core::Selection::startGridNavigationTimer));
        }
    }
}

bool core::Selection::isSelectionPending() const
{
    return m_scene_node_selection_change || m_grid_selection_change;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::cancelGridSelectionOrQuit()
{
    if (m_editor)
    {
        bool handled = false;
        if (m_editor->isInGridEditingState())
        {
            if (m_editor->shared()->selectedGrid())
            {
                handled = true;
                m_editor->emitClosure(::bind(
                    m_editor->actions()->gridActions(),
                    &gui::actions::GridActions::cancelSelection
                ));
            }
        }
    }
}

// =========================================== PROTECTED SLOTS ===========================================

void core::Selection::disableSceneNodeSelectionNavigation()
{
    m_scene_node_selection_change = false;
}

void core::Selection::disableGridSelectionNavigation()
{
    m_grid_selection_change = false;    
}

void core::Selection::startSceneNodeNavigationTimer()
{
    if (m_scene_node_selection_change)
    {
        m_scene_node_nav_timer.stop();
    }
    m_scene_node_nav_timer.setSingleShot(true);
    m_scene_node_nav_timer.setInterval(core::Selection::TIMEOUT);
    m_scene_node_nav_timer.start();
    m_scene_node_selection_change = true;
}

void core::Selection::startGridNavigationTimer()
{
    if (m_grid_selection_change)
    {
        m_grid_nav_timer.stop();
    }
    m_grid_nav_timer.setSingleShot(true);
    m_grid_nav_timer.setInterval(core::Selection::TIMEOUT);
    m_grid_nav_timer.start();
    m_grid_selection_change = true;
}

bool core::Selection::forceEditorEnterMovingState(const sad::input::MousePressEvent& e) const
{
    bool result = false;
    sad::Vector<sad::Rect2D> regions;
    m_editor->shared()->selectedObject()->regions(regions);
    if (sad::isWithin(e.pos2D(), regions))
    {			
        sad::SceneNode* node = m_editor->shared()->selectedObject();
        // If node is within grid - exit
        if (m_editor->actions()->gridActions()->isInGrid(node))
        {
            return true;
        }
        sad::Maybe<sad::Rect2D> oldarea = node->getProperty<sad::Rect2D>("area");
        if (oldarea.exists())
        {
            m_editor->shared()->setPivotPoint(e.pos2D());
            m_editor->shared()->setOldArea(oldarea.value());
            m_editor->machine()->enterState("selected/moving");
        }
        result = true;
    }
    return result;
}

bool core::Selection::forceEditorEnterGridMovingState(const sad::input::MousePressEvent& e) const
{
    bool result = false;
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    sad::Rect2D rect = grid->area();
    if (sad::isWithin(e.pos2D(), rect))
    {
        // If node is within grid - exit
        if (m_editor->actions()->gridActions()->isInGrid(grid))
        {
            return true;
        }
        picojson::value value(picojson::object_type, false);
        grid->save(value);
        m_editor->shared()->setOldState(value);
        m_editor->shared()->setPivotPoint(e.pos2D());
        m_editor->shared()->setOldArea(rect);
        m_editor->machine()->enterState("layouts/moving");		
        result = true;
    }
    return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::forceEditorEnterResizingState(
    core::borders::ResizeHotspot* h,
    const sad::input::MousePressEvent& e
)
{
    sad::Maybe<sad::Rect2D> oldarea = m_editor->shared()->selectedObject()->getProperty<sad::Rect2D>("area");
    if (oldarea.exists())
    {
        sad::Vector<sad::Rect2D> regions;
        m_editor->shared()->selectedObject()->regions(regions);
        m_editor->shared()->setPivotPoint(e.pos2D());
        m_editor->shared()->setOldArea(oldarea.value());
        m_editor->shared()->setResizingIndexes(h->resizingIndexes());
        m_editor->shared()->setResizingDirection(h->directionVector(
            regions[0]
        ));
        m_editor->shared()->setOldRegion(regions[0]);
        m_editor->shared()->setNormalizedResizingDirection(h->defaultDirectionVector());
        m_editor->machine()->enterState("selected/resizing");
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
 void  core::Selection::forceEditorEnterGridResizingState(
    core::borders::ResizeHotspot* h,
    const sad::input::MousePressEvent& e
)
{
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    sad::Rect2D oldarea = grid->area();
    picojson::value value(picojson::object_type, false);
    grid->save(value);
    m_editor->shared()->setOldState(value);
    m_editor->shared()->setPivotPoint(e.pos2D());
    m_editor->shared()->setOldArea(oldarea);
    m_editor->shared()->setResizingIndexes(h->resizingIndexes());
    m_editor->shared()->setResizingDirection(h->directionVector(
        oldarea
    ));
    m_editor->shared()->setOldRegion(oldarea);
    m_editor->shared()->setNormalizedResizingDirection(h->defaultDirectionVector());
    m_editor->machine()->enterState("layouts/resizing");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::removeItem()
{
    sad::SceneNode* node = m_editor->shared()->selectedObject();
    if (node)
    {
        void (gui::actions::SceneNodeActions::*method)(sad::SceneNode*, bool) = &gui::actions::SceneNodeActions::removeSceneNode;
        m_editor->emitClosure(
           ::bind(
              m_editor->actions()->sceneNodeActions(),
              method,
              node,
              true
           )
        );
    }
}

// =========================================== PROTECTED METHODS ===========================================

void core::Selection::trySelectGrid(const sad::input::MousePressEvent& e)
{
    m_scene_node_selection_change = false;
    m_editor->emitClosure( bind(&m_grid_nav_timer, &QTimer::stop) );
    sad::layouts::Grid* grid = m_editor->shared()->selectedGrid();
    if (grid)
    {
        bool ret = false;
        core::borders::ResizeHotspot* h = m_editor->renderGrids()->selectedResizeHotspot(e.pos2D());
        if (h)
        {
            this->forceEditorEnterGridResizingState(h, e);
            ret = true;
        }
        else 
        {
            if (sad::isWithin(e.pos2D(), grid->area()))
            {
                ret = this->forceEditorEnterGridMovingState(e);
            }
        }
        if (ret)
        {
            return;
        }
    }

    // Fill grid navigation chain 
    m_scene_node_selection_chain.clear();
    m_grid_selection_chain.clear();
    sad::Vector<sad::layouts::Grid*> grids;
    m_editor->actions()->gridActions()->activeGrids(grids);
    for(int i = grids.size() - 1; i > -1; i--)
    {
        grid = grids[i];
        if (grid->active())
        {
            sad::Vector<sad::Rect2D> regions;
            if (sad::isWithin(e.pos2D(), grid->area()))
            {
                m_grid_selection_chain << grid;
            }
        }
    }

    if (m_grid_selection_chain.count() == 0)
    {
        m_editor->shared()->setSelectedGrid(nullptr);
        m_editor->emitClosure(::bind(m_editor->panelProxy(), &gui::MainPanelProxy::enterGlobalOffsetEditingState, e.pos2D()));
    }
    else
    {
        m_editor->shared()->setSelectedGrid(m_grid_selection_chain[0]);
        m_editor->actions()->gridActions()->updateGridPropertiesInUI(false);
        m_current_grid_chain_position = 0;
        m_editor->emitClosure( ::bind(this, &core::Selection::startGridNavigationTimer));
        m_editor->emitClosure( ::bind(this, &core::Selection::forceEditorEnterGridMovingState, e));
    }
}

void core::Selection::trySelectObject(const sad::input::MousePressEvent& e)
{
    // Stop selection
    m_scene_node_selection_change = false;
    m_editor->emitClosure( bind(&m_scene_node_nav_timer, &QTimer::stop) );

    if (m_editor->shared()->selectedObject())
    {
        // Check, whether we hit the hotspot
        bool ret;
        core::borders::ResizeHotspot* h = m_editor->selectionBorder()->selectedResizeHotspot(e.pos2D());
        if (h)
        {
            this->forceEditorEnterResizingState(h, e);
            ret = true;
        }
        else
        {
            if (m_editor->selectionBorder()->deleteHotspot()->isWithin(e.pos2D()))
            {
                m_editor->emitClosure(bind(this, &core::Selection::removeItem));
                ret = true;
            }
            else
            {
                ret = this->forceEditorEnterMovingState(e);
            }
        }
        if (ret)
        {
            return;
        }
    }


    // Fill navigation chain
    m_grid_selection_chain.clear();
    sad::Scene* current_scene = m_editor->actions()->sceneActions()->currentScene();
    if (current_scene)
    {
        m_scene_node_selection_chain.clear();
        const sad::Vector<sad::SceneNode*>& objects = current_scene->objects();
        for(int i = objects.size() - 1; i > -1; i--)
        {
            if (objects[i]->active() && objects[i]->visible())
            {
                sad::Vector<sad::Rect2D> regions;
                objects[i]->regions(regions);
                if (sad::isWithin(e.pos2D(), regions))
                {
                    m_scene_node_selection_chain << objects[i];
                }
            }
        }

        if (m_scene_node_selection_chain.count() == 0)
        {
            m_editor->machine()->enterState("idle");
            m_editor->shared()->setSelectedObject(nullptr);
            m_editor->emitClosure(::bind(m_editor->panelProxy(), &gui::MainPanelProxy::enterGlobalOffsetEditingState, e.pos2D()));
        }
        else
        {
            m_editor->shared()->setSelectedObject(m_scene_node_selection_chain[0]);
            m_editor->machine()->enterState("selected");
            m_editor->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
            m_scene_node_current_position = 0;
            m_editor->emitClosure( bind(this, &core::Selection::startSceneNodeNavigationTimer));
            m_editor->emitClosure( bind(this, &core::Selection::forceEditorEnterMovingState, e));
        }
    }
}

void core::Selection::trySelectWay(const sad::input::MousePressEvent& e)
{
    const double radius = 11.0; // Radius of active area for a point
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    gui::uiblocks::UIWayBlock* ui_way_block = m_editor->uiBlocks()->uiWayBlock();
    if (w != nullptr)
    {
        int row = ui_way_block->lstWayPoints->currentRow();
        if (row >= 0 && row < ui_way_block->lstWayPoints->count())
        {
            if (e.pos2D().distance(w->wayPoints()[row]) <= radius)
            {
               tryEnterToMovingStateWithWayObject(e);
               return;
            }
        }
    }

    QListWidget* lw = ui_way_block->lstWays;
    for(int i = lw->count() - 1; i > -1; i--)
    {
        QVariant v = lw->item(i)->data(Qt::UserRole);
        sad::p2d::app::Way* local_way = v.value<sad::p2d::app::Way*>();
        const sad::Vector<sad::Point2D> & pts = local_way->wayPoints();
        for(int j = pts.size() - 1; j > -1; j--)
        {
            if (e.pos2D().distance(pts[j]) <= radius)
            {
                 m_editor->machine()->enterState("ways/selected");
                 m_editor->shared()->setSelectedWay(w);
                 m_editor->emitClosure(bind(
                    this,
                    &core::Selection::commitWaySelection,
                    i,
                    j
                 ));
                 m_editor->emitClosure(bind(
                     this,
                    &core::Selection::tryEnterToMovingStateWithWayObject,
                    e
                 ));
                 return;
            }
        }
    }

    m_editor->machine()->enterState("ways/idle");
    m_editor->shared()->setSelectedWay(nullptr);
    m_editor->emitClosure(bind(this, &core::Selection::commitIdleWaySelection));

    m_editor->emitClosure(::bind(m_editor->panelProxy(), &gui::MainPanelProxy::enterGlobalOffsetEditingState, e.pos2D()));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::tryEnterToMovingStateWithWayObject(const sad::input::MousePressEvent& e)
{
    gui::uiblocks::UIWayBlock* ui_way_block = m_editor->uiBlocks()->uiWayBlock();
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w != nullptr)
    {
        int row = ui_way_block->lstWayPoints->currentRow();
        if (row >= 0 && row < ui_way_block->lstWayPoints->count())
        {
            m_editor->panelProxy()->highlightState("Moving point");
            m_editor->machine()->enterState("ways/selected/moving");
            m_editor->shared()->setPivotPoint(e.pos2D());
            m_editor->shared()->setOldPoint(w->wayPoints()[row]);
            m_editor->shared()->setWayPointPosition(row);
        }
    }
}


// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::commitWaySelection(int i, int j)
{
    gui::uiblocks::UIWayBlock* ui_way_block = m_editor->uiBlocks()->uiWayBlock();
    
    ui_way_block->lstWays->setCurrentRow(i);
    m_editor->actions()->wayActions()->updateUIForSelectedWayNow();
    ui_way_block->lstWayPoints->setCurrentRow(j);
    m_editor->actions()->wayActions()->viewPoint(j);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void core::Selection::commitIdleWaySelection()
{
    gui::uiblocks::UIWayBlock* ui_way_block = m_editor->uiBlocks()->uiWayBlock();

    ui_way_block->lstWays->clearSelection();
    ui_way_block->lstWayPoints->clearSelection();
}
