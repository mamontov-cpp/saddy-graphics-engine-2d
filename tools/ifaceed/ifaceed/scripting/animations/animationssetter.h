/*! \file animationssetter.h
    

    Defines a generic setter for setting a property of animations
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

namespace scripting
{
    
namespace animations
{

/*! A common setter for setting common property of animations
 */
template<
    typename _AnimationType,
    typename _PropertyType,
    typename _CommandType
>
class Setter: public scripting::AbstractSetter<_AnimationType*,_PropertyType>
{
public:	
    /*! Construct new setter for property
        \param[in] s scripting part
        \param[in] name a name for property
     */
    Setter(
        scripting::Scripting* s,
        const sad::String& name = ""
    ) : scripting::AbstractSetter<_AnimationType*,_PropertyType>(s)
    {
        this->setPropertyName(name);
    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::animations::Setter<_AnimationType, _PropertyType, _CommandType>(*this);
    }

    /*! Could be inherited
     */ 
    virtual ~Setter()
    {
        
    }


    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
    */
    virtual void callActions(_AnimationType* obj, const sad::String& property_name, _PropertyType old_value, _PropertyType new_value)
    {
        this->scripting::AbstractSetter<_AnimationType*, _PropertyType>::callActions(obj, property_name, old_value, new_value);
        this->setProperty(obj, property_name, old_value, new_value);
    }

    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(_AnimationType* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
    {
        core::Editor* editor =  this->m_scripting->editor();

        history::Command* c =  new _CommandType(obj, oldvalue, newvalue);
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
};

}

}
