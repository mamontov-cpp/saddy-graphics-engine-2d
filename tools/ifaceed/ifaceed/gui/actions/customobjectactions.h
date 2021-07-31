/*! \file customobjectactions.h
    

    Describes a group of actions, linked to custom objects
 */
#pragma once
#include <QObject>

#include <input/events.h>

#include <db/dbvariant.h>

#include <sadstring.h>

#include "abstractactions.h"

class MainPanel;

namespace sad
{

class SceneNode;

namespace db
{

class Variant;

namespace custom
{

class Object;

}

}

}

namespace gui
{

namespace table
{
class Delegate;	
}

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
    CustomObjectActions(QObject* parent = nullptr);
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
    /*! Fills list of properties for custom objects
        \param[in] node a node object
     */
    void fillCustomObjectProperties(
        sad::SceneNode* node	
    );
    /*! Cleans table of properties of custom object
     */
    void clearCustomObjectPropertiesTable();
    /*! Finds delegate for custom object property in table
     * \param[in] name a name for delegate for custom object
     * \return found delegate, or nullptr
     */
    gui::table::Delegate* delegateForCustomObjectProperty(const QString& name);
    /*! Updates custom object property value in UI, if it's selected
     *  \param[in] node a node, which must be selected
     *  \param[in] name a name of property
     *  \param[in] value a value for a property
     */
    void updateCustomObjectPropertyValue(
            sad::SceneNode* node,
            const sad::String& name,
            const sad::db::Variant& value
     ); 
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
    /*! Triggers adding a custom object from a fast mode
     */
    void triggerAddFromFastMode();
    /*! Emitted, when schema changed
        \param[in] s  a schema name
     */
    void schemaChanged(sad::String s);
    /*! Updates value for updating custom object property in UI now
     */
    void updateCustomObjectPropertyValueNow();
private:
    /*! Makes new custom object
        \return new custom object
     */
    sad::db::custom::Object* makeNewCustomObject();
    /*! Try copy custom properties for selected object
        \param[in] object an object values
     */
    void tryCopySelectedObjectCustomProperties(sad::db::custom::Object* object);

    /*! A temporary value for custom object property name slot, which accessed in
        gui::actions::CustomObjectActions::updateCustomObjectPropertyValueNow
     */
    sad::String m_custom_object_property_name;
    /*! A temporary value for custom object property value slot, which accessed in
        gui::actions::CustomObjectActions::updateCustomObjectPropertyValueNow
     */
    sad::db::Variant m_custom_object_property_value;
};

}

}
