/*! \file customobjectactions.h
    

    Describes a group of actions, linked to sprited
 */
#pragma once
#include <QObject>

#include <input/events.h>

#include <sadstring.h>

#include "abstractactions.h"

class MainPanel;

namespace sad
{

namespace db
{

namespace custom
{

class Object;

}

}

}

namespace gui
{

namespace actions
{

/*! A group of actions, linked to custom objects
 */	
class CustomObjectActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new actions for sprites
        \param[in] parent a parent object
     */
    CustomObjectActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~CustomObjectActions();
    /*! Cancels adding sprite to scene
     */
    void cancelAdd();
    /*! Moves center of sprite to a position
        \param[in] e a sprite
     */
    void moveCenterOfObject(const sad::input::MouseMoveEvent & e);
    /*! Commits adding a sprite
        \param[in] e event
     */
    void commitAdd(const sad::input::MousePressEvent& e);
    /*! Places first point for a object
        \param[in] e event
     */
    void placeFirstPoint(const sad::input::MousePressEvent& e);
    /*! Moves lower point of object 
        \param[in] e a sprite
     */
    void moveLowerPoint(const sad::input::MouseMoveEvent & e);
public slots:
    /*! Starts placing custom objects
     */
    void add();
    /*! Adds new custom object to a scene, if all parameters in form are valid
     */
    void addBySimplePlacing();
    /*! Adds new custom object to a scene by diagonal placing
     */
    void addByDiagonalPlacing();
    /*! Emitted, when schema changed
        \param[in] s  a schema name
     */
    void schemaChanged(sad::String s);
private:
    /*! Makes new custom object
        \return new custom object
     */
    sad::db::custom::Object* makeNewCustomObject();
    /*! Try copy custom properties for selected object
        \param[in] object an object values
     */
    void tryCopySelectedObjectCustomProperties(sad::db::custom::Object* object);
};

}

}
