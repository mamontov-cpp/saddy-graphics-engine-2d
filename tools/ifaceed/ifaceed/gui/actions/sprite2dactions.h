/*! \file sprite2dactions.h
    

    Describes a group of actions, linked to sprites
 */
#pragma once
#include <QObject>

#include <scenenode.h>

#include <input/events.h>

#include "abstractactions.h"

namespace history
{
class Command;
}

namespace sad
{
class SceneNode;
}

namespace gui
{

namespace actions
{

/*! A group of actions, linked to sprites
 */	
class Sprite2DActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new actions for sprites
        \param[in] parent a parent object
     */
    Sprite2DActions(QObject* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~Sprite2DActions();
    /*! Cancels adding sprite to scene
     */
    void cancelAddSprite();
    /*! Moves center of sprite to a position
        \param[in] e a sprite
     */
    void moveCenterOfSprite(const sad::input::MouseMoveEvent & e);
    /*! Commits adding a sprite
        \param[in] e event
     */
    void commitAdd(const sad::input::MousePressEvent& e);
    /*! Places first point for a sprite
        \param[in] e event
     */
    void placeFirstPointForSprite(const sad::input::MousePressEvent& e);
    /*! Moves lower point of sprite to a position
        \param[in] e a sprite
     */
    void moveLowerPointOfSprite(const sad::input::MouseMoveEvent & e);
    /*! A scriptable implementation of making an item a background
        \param[in] node a node to be edited
        \param[in] from_editor whether action is performed from editor
     */
    void setSceneNodeAsBackground(sad::SceneNode* node, bool from_editor);
public slots:
    /*! Starts placing sprites
     */
    void add();
    /*! Adds new sprite to a scene, if all parameters in form are valid
     */
    void addBySimplePlacing();
    /*! Adds new sprite to a scene by diagonal placing
     */
    void addByDiagonalPlacing();
    /*! Called, when sprite options are changed
        \param[in] s a new options for changed sprite
     */
    void spriteOptionsChanged(sad::String s);
    /*! Called, when user clicks on "Make background" button
     */
    void makeBackground();
    /*! Called, when user clicks on "Flip X" checkbox
     */
    void flipXChanged(bool state);
    /*! Called, when user clicks on "Flip Y" checkbox
     */
    void flipYChanged(bool state);
private:
    /*! Blocks first point placement for fast mode
     */ 
    bool m_block_first_point_for_fast_mode;
};

}

}
