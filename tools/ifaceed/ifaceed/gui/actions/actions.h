/*! \file actions.h
    
    Declares a container for actions
 */
#pragma once
namespace core
{
class Editor;
}

namespace gui
{

namespace actions
{

class SceneNodeActions;
class LabelActions;
class Sprite2DActions;
class CustomObjectActions;
class WayActions;
class DialogueActions;
class AnimationActions;
class AnimationInstanceActions;
class AnimationGroupActions;
class SceneActions;
class GridActions;

/*! A container for actions, grouped by node
 */
class Actions
{
public:
    /*! Creates new actions
     */
    Actions();
    /*! Links actions to editor
        \param[in] e editor
     */
    void setEditor(core::Editor* e);
    /*! Deletes inner actions list
     */
    ~Actions();
    /*! Returns actions for scenes
        \return actions for scenes
     */
    gui::actions::SceneActions* sceneActions() const;
    /*! Returns actions for scene nodes
        \return actions for scene nodes
     */
    gui::actions::SceneNodeActions* sceneNodeActions() const;
    /*! Returns label actions
     *  \return label actions 
     */
    gui::actions::LabelActions* labelActions() const;
    /*! Returns sprite actions
     *  \return sprite actions 
     */
    gui::actions::Sprite2DActions* sprite2DActions() const;
    /*! Returns actions for custom object
     *  \return actions for custom object 
     */
    gui::actions::CustomObjectActions* customObjectActions() const;
    /*! Returns actions for way editing
     *  \return actions for way editing
     */
    gui::actions::WayActions* wayActions() const;
    /*! Returns actions for dialogue editing
     *  \return actions for dialogue editing
     */
    gui::actions::DialogueActions* dialogueActions() const;
    /*! Returns actions for animation editing
     *  \return actions for animation editing
     */
    gui::actions::AnimationActions* animationActions() const;
    /*! Returns actions for animation instance editing
     *  \return actions for animation instance editing
     */
    gui::actions::AnimationInstanceActions* instanceActions() const;
    /*! Returns actions for animation group editing
     *  \return actions for animation group editing
     */
    gui::actions::AnimationGroupActions* groupActions() const;
    /*! Returns actions, that are linked to grids
        \return grid actions
     */
    gui::actions::GridActions* gridActions() const;
protected:
    /*! An actions, linked to scenes
     */
    gui::actions::SceneActions* m_scene_actions;
    /*! An actions, linked to scene nodes
     */
    gui::actions::SceneNodeActions* m_scene_node_actions;
    /*! An actions, linked to label editing
     */
    gui::actions::LabelActions* m_label_actions;
    /*! An actions, linked to sprite editing
     */
    gui::actions::Sprite2DActions* m_sprite2d_actions;
    /*! An actions, linked to custom object editing
     */
    gui::actions::CustomObjectActions* m_custom_object_actions;
    /*! An actions, linked to way editing 
     */
    gui::actions::WayActions* m_way_actions;
    /*! An actions, linked to dialogue editing
     */
    gui::actions::DialogueActions* m_dialogue_actions;
    /*! An actions, linked to animations editing
     */
    gui::actions::AnimationActions* m_animation_actions;
    /*! An actions, linked to animation instance editing
     */
    gui::actions::AnimationInstanceActions* m_instance_actions;
    /*! An actions, linked to animation groups editing
     */
    gui::actions::AnimationGroupActions* m_group_actions;
    /*! An actions, linked to grids (sad::layouts::Grid)
     */
    gui::actions::GridActions* m_grid_actions;
};

}

}
