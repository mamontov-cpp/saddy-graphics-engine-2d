#include "actions.h"

#include "sceneactions.h"
#include "scenenodeactions.h"
#include "labelactions.h"
#include "sprite2dactions.h"
#include "customobjectactions.h"
#include "wayactions.h"
#include "dialogueactions.h"
#include "animationactions.h"
#include "animationinstanceactions.h"
#include "animationgroupactions.h"
#include "gridactions.h"

gui::actions::Actions::Actions() : m_scene_actions(new gui::actions::SceneActions()),
m_scene_node_actions(new gui::actions::SceneNodeActions()),
m_label_actions(new gui::actions::LabelActions()),
m_sprite2d_actions(new gui::actions::Sprite2DActions()),
m_custom_object_actions(new gui::actions::CustomObjectActions()),
m_way_actions(new gui::actions::WayActions()),
m_dialogue_actions(new gui::actions::DialogueActions()),
m_animation_actions(new gui::actions::AnimationActions()),
m_instance_actions(new gui::actions::AnimationInstanceActions()),
m_group_actions(new gui::actions::AnimationGroupActions()),
m_grid_actions(new gui::actions::GridActions())
{
    
}


void gui::actions::Actions::setEditor(core::Editor* e)
{
    m_scene_actions->setEditor(e);
    m_scene_node_actions->setEditor(e);
    m_label_actions->setEditor(e);
    m_sprite2d_actions->setEditor(e);
    m_custom_object_actions->setEditor(e);
    m_way_actions->setEditor(e);
    m_dialogue_actions->setEditor(e);
    m_animation_actions->setEditor(e);
    m_instance_actions->setEditor(e);
    m_group_actions->setEditor(e);
    m_grid_actions->setEditor(e);
}

gui::actions::Actions::~Actions()
{
    delete m_scene_actions;
    delete m_scene_node_actions;
    delete m_label_actions;
    delete m_sprite2d_actions;
    delete m_custom_object_actions;
    delete m_way_actions;
    delete m_animation_actions;
    delete m_dialogue_actions;
    delete m_instance_actions;
    delete m_group_actions;
}

gui::actions::SceneActions* gui::actions::Actions::sceneActions() const
{
    return m_scene_actions;
}

gui::actions::SceneNodeActions* gui::actions::Actions::sceneNodeActions() const
{
    return m_scene_node_actions;
}

gui::actions::LabelActions* gui::actions::Actions::labelActions() const
{
    return m_label_actions;
}

gui::actions::Sprite2DActions* gui::actions::Actions::sprite2DActions() const
{
    return m_sprite2d_actions;
}

gui::actions::CustomObjectActions* gui::actions::Actions::customObjectActions() const
{
    return m_custom_object_actions;
}

gui::actions::WayActions* gui::actions::Actions::wayActions() const
{
    return m_way_actions;
}

gui::actions::DialogueActions* gui::actions::Actions::dialogueActions() const
{
    return m_dialogue_actions;
}

gui::actions::AnimationActions* gui::actions::Actions::animationActions() const
{
    return m_animation_actions;
}

gui::actions::AnimationInstanceActions* gui::actions::Actions::instanceActions() const
{
    return m_instance_actions;
}

gui::actions::AnimationGroupActions*  gui::actions::Actions::groupActions() const
{
    return m_group_actions;
}

gui::actions::GridActions*  gui::actions::Actions::gridActions() const
{
    return m_grid_actions;
}
