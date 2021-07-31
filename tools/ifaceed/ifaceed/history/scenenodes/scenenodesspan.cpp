#include "scenenodesspan.h"

#include <db/save.h>

#include "../../core/editor.h"

#include ".././gui/actions/actions.h"
#include ".././gui/actions/scenenodeactions.h"

// ====================================== PUBLIC METHODS ======================================

history::scenenodes::Span::Span(
    sad::SceneNode* d,
    const sad::Rect2D& old_area,
    double old_angle,
    const sad::Rect2D& new_area,
    double new_angle
) : m_node(d),
m_old_area(old_area),
m_old_angle(old_angle),
m_new_area(new_area),
m_new_angle(new_angle)
{
    
}

history::scenenodes::Span::~Span()
{
    
}

void history::scenenodes::Span::commit(core::Editor * ob)
{
    this->performUpdate(ob, m_new_area, m_new_angle);
}

void history::scenenodes::Span::rollback(core::Editor * ob)
{
    this->performUpdate(ob, m_old_area, m_old_angle);
}


void history::scenenodes::Span::performUpdate(
    core::Editor* e,
    const sad::Rect2D& rect,
    double angle
)
{
    if (e && m_node)
    {
        m_node->setProperty("area", rect);
        m_node->setProperty("angle", angle);
        gui::actions::SceneNodeActions * actions = e->actions()->sceneNodeActions();
        if (e->shared()->selectedObject() == m_node) {
            actions->updateRegionForNode();
            actions->updateAngleForNode();
        }
    }
}

