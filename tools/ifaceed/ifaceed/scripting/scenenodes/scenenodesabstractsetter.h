/*! \file scenenodesabstractsetter.h
    

    Defines an abstract setter for properties of scene node
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <scenenode.h>



namespace scripting
{
    
namespace scenenodes
{

/*! Defines an abstract setter for properties of scene node
 */
template<
    typename _PropertyType
>
class AbstractSetter: public scripting::AbstractSetter<sad::SceneNode*, _PropertyType>
{
public:
    /*! Construct new setter as callable value
        \param[in] scripting a scripting link
     */
    AbstractSetter(scripting::Scripting* scripting) : scripting::AbstractSetter<sad::SceneNode*, _PropertyType>(scripting)
    {
        
    }

    /*! Makes setter for a specified property
        \param[in] scripting a scripting part
        \param[in] property_name a name of property
     */
    AbstractSetter(scripting::Scripting* scripting, const sad::String& property_name) : scripting::AbstractSetter<sad::SceneNode*, _PropertyType>(scripting)
    {
        this->setPropertyName(property_name);
    }

    /*! Could be inherited
     */
    virtual ~AbstractSetter()
    {
        
    }

    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
     */
    virtual void callActions(sad::SceneNode* obj, const sad::String& property_name, _PropertyType old_value,  _PropertyType new_value)
    {
        this->scripting::AbstractSetter<sad::SceneNode*, _PropertyType>::callActions(obj, property_name, old_value, new_value);
        this->setProperty(obj, property_name, old_value, new_value);
    }

    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] old_value old value
        \param[in] new_value new value
     */
    virtual void setProperty(sad::SceneNode* obj, const sad::String& propertyname, _PropertyType old_value,  _PropertyType new_value)
    {
        core::Editor* editor =  this->m_scripting->editor();

        history::Command* c = this->command(obj, propertyname, old_value, new_value);
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
    /*! Returns command for editing a property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
        \return a command to be used
     */
    virtual history::Command* command(sad::SceneNode* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue) = 0;
};

}

}
