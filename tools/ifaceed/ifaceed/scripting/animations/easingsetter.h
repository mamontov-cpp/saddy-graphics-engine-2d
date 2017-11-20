/*! \file easingsetter.h
    

    Defines a setter for easing function properties
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>


namespace scripting
{
    
namespace animations
{

/*! A setter for setting easing functions  for animation
 */
template<
    typename _PropertyType,
    typename _CommandType
>
class EasingSetter: public scripting::AbstractSetter<sad::animations::Animation*, _PropertyType>
{
public:
    /*! Represents a constructor call for a function with two arguments
        \param[in] scripting scripting part
        \param[in] name name of function call
        \param[in] getter a getter for property
     */
    EasingSetter(
        scripting::Scripting* scripting, 
        const sad::String& name,
        _PropertyType (sad::animations::easing::Function::*getter)() const
    ) : scripting::AbstractSetter<sad::animations::Animation*, _PropertyType>(scripting), m_getter(getter)
    {
        this->setPropertyName(name);
    }

    /*! Could be inherited
     */
    virtual ~EasingSetter()
    {

    }

    /*! Clones an object
         \return copy of object
    */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::animations::EasingSetter<_PropertyType, _CommandType>(*this);
    }

    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
    */
    virtual void callActions(sad::animations::Animation* obj, const sad::String& property_name, _PropertyType old_value, _PropertyType new_value)
    {
        this->scripting::AbstractSetter<sad::animations::Animation*, _PropertyType>::callActions(obj, property_name, old_value, new_value);
        this->setProperty(obj, property_name, old_value, new_value);
    }

    /*! Performs making new command and committing it
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(sad::animations::Animation* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
    {
        scripting::Scripting* e = this->m_scripting;
        core::Editor* editor =  e->editor();

        _CommandType* c = new _CommandType(obj, oldvalue, newvalue);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
    }
protected:
    /*! Checks if property exists in object
        \param[in] value object
        \param[in] property_name a name of property
        \return true, if exists
    */
    virtual bool hasProperty(sad::animations::Animation* value, const sad::String& property_name)
    {
        return true;
    }

    /*! Returns old property value
        \param[in] value object
        \param property_name a name of property
        \return property value
    */
    virtual  _PropertyType getOldPropertyValue(sad::animations::Animation* value, const sad::String& property_name)
    {
        return ((value->easing())->*m_getter)();
    }
    /*! A getter for easing function
     */
    _PropertyType (sad::animations::easing::Function::*m_getter)() const;
};

}

}
