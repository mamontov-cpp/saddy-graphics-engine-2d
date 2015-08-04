#include "selection.h"

#include "editor.h"

#include "history/scenenodes/scenenodesremove.h"

#include "borders/selectionborder.h"

#include "gui/wayactions.h"

#include "../mainpanel.h"
#include "../closuremethodcall.h"

#include <geometry2d.h>

Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

// =========================================== PUBLIC METHODS =========================================== 

core::Selection::Selection() : m_editor(NULL), m_selection_change(false), m_current_position(0)
{
	 connect(&m_timer, SIGNAL(timeout()), this, SLOT(disableSelectionNavigation()));
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
        this->trySelectObject(e);
    }
}

void core::Selection::navigateSelection(const sad::input::MouseWheelEvent& e)
{
    // Do not allow changing item, when object is being edited
    if (this->m_editor->isInEditingState())
    {
        return;
    }

	int last_position = m_current_position;
	if (e.Delta > 0)
	{
		if (m_current_position == m_selection_chain.size() - 1)
		{
			m_current_position = 0;
		}
		else
		{
			m_current_position++;
		}
	}
	else
	{
		if (m_current_position == 0)
		{
			m_current_position = m_selection_chain.size() - 1;
		}
		else
		{
			m_current_position--;
		}
	}

	if (last_position != m_current_position)
	{
		m_editor->shared()->setSelectedObject(m_selection_chain[m_current_position]);
		m_editor->machine()->enterState("selected");
		m_editor->panel()->updateUIForSelectedItem();
		m_editor->emitClosure( bind(this, &core::Selection::startTimer));
	}
}

bool core::Selection::isSelectionPending() const
{
	return m_selection_change;
}

// =========================================== PROTECTED SLOTS ===========================================

void core::Selection::disableSelectionNavigation()
{
	m_selection_change = false;
}

void core::Selection::startTimer()
{
	if (m_selection_change)
	{
		m_timer.stop();
	}
	m_timer.setSingleShot(true);
	m_timer.setInterval(core::Selection::TIMEOUT);
	m_timer.start();
	m_selection_change = true;
}

bool core::Selection::forceEditorEnterMovingState(const sad::input::MousePressEvent& e)
{
	bool result = false;
	sad::Vector<sad::Rect2D> regions;
	m_editor->shared()->selectedObject()->regions(regions);
	if (sad::isWithin(e.pos2D(), regions))
	{			
		sad::Maybe<sad::Rect2D> oldarea = m_editor->shared()->selectedObject()->getProperty<sad::Rect2D>("area");
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

void core::Selection::removeItem()
{
	sad::SceneNode* node = m_editor->shared()->selectedObject();
	if (node)
	{
		int row = m_editor->panel()->findSceneNodeInList(node);
		if (row == -1)
		{
			row = static_cast<int>(node->scene()->findLayer(node));
		}
		
		history::Command* c = new history::scenenodes::Remove(node, row);
		m_editor->history()->add(c);
		c->commit(m_editor);
	}
}

// =========================================== PROTECTED METHODS ===========================================

void core::Selection::trySelectObject(const sad::input::MousePressEvent& e)
{
    // Stop selection
    m_selection_change = false;
    m_editor->emitClosure( bind(&m_timer, &QTimer::stop) );

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
    if (m_editor->panel()->currentScene())
    {
        m_selection_chain.clear();
        const sad::Vector<sad::SceneNode*>& objects = m_editor->panel()->currentScene()->objects();
        for(int i = objects.size() - 1; i > -1; i--)
        {
            if (objects[i]->active() && objects[i]->visible())
            {
                sad::Vector<sad::Rect2D> regions;
                objects[i]->regions(regions);
                if (sad::isWithin(e.pos2D(), regions))
                {
                    m_selection_chain << objects[i];
                }
            }
        }

        if (m_selection_chain.count() == 0)
        {
            m_editor->machine()->enterState("idle");
            m_editor->shared()->setSelectedObject(NULL);
        }
        else
        {
            m_editor->shared()->setSelectedObject(m_selection_chain[0]);
            m_editor->machine()->enterState("selected");
            m_editor->panel()->updateUIForSelectedItem();
            m_current_position = 0;
            m_editor->emitClosure( bind(this, &core::Selection::startTimer));
            m_editor->emitClosure( bind(this, &core::Selection::forceEditorEnterMovingState, e));
        }
    }
}

void core::Selection::trySelectWay(const sad::input::MousePressEvent& e)
{
    const double radius = 11.0; // Radius of active area for a point
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w != NULL)
    {
        int row = m_editor->panel()->UI()->lstWayPoints->currentRow();
        if (row >= 0 && row < m_editor->panel()->UI()->lstWayPoints->count())
        {
            if (e.pos2D().distance(w->wayPoints()[row]) <= radius)
            {
               tryEnterToMovingStateWithWayObject(e);
               return;
            }
        }
    }

    QListWidget* lw = m_editor->panel()->UI()->lstWays;
    for(int i = lw->count() - 1; i > -1; i--)
    {
        QVariant v = lw->item(i)->data(Qt::UserRole);
        sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
        const sad::Vector<sad::Point2D> & pts = w->wayPoints();
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
    m_editor->shared()->setSelectedWay(NULL);
    m_editor->emitClosure(bind(this, &core::Selection::commitIdleWaySelection));
}

void core::Selection::tryEnterToMovingStateWithWayObject(const sad::input::MousePressEvent& e)
{
    sad::p2d::app::Way* w = m_editor->shared()->selectedWay();
    if (w != NULL)
    {
        int row = m_editor->panel()->UI()->lstWayPoints->currentRow();
        if (row >= 0 && row < m_editor->panel()->UI()->lstWayPoints->count())
        {
            m_editor->panel()->highlightState("Moving point");
            m_editor->machine()->enterState("ways/selected/moving");
            m_editor->shared()->setPivotPoint(e.pos2D());
            m_editor->shared()->setOldPoint(w->wayPoints()[row]);
            m_editor->shared()->setWayPointPosition(row);
        }
    }
}


void core::Selection::commitWaySelection(int i, int j)
{
    m_editor->panel()->UI()->lstWays->setCurrentRow(i);
    m_editor->panel()->updateUIForSelectedWayNow();
    m_editor->panel()->UI()->lstWayPoints->setCurrentRow(j);
    m_editor->panel()->wayActions()->viewPoint(j);
}

void core::Selection::commitIdleWaySelection()
{
    m_editor->panel()->UI()->lstWays->clearSelection();
    m_editor->panel()->UI()->lstWayPoints->clearSelection();
}
