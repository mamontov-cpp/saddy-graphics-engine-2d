/*! \file wayssetter.h
    

    Defines a generic setter for setting a property of way
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <way.h>

namespace scripting
{
    
namespace ways
{

/*! A generic setter for setting property of way
 */
template<
    typename _PropertyType,
    typename _CommandType
>
class Setter: public scripting::AbstractSetter<sad::Way*,_PropertyType>
{
public:	
    /*! Construct new setter for specified property of way
        \param[in] scripting a scripting part
        \param[in] name a name for property
     */
    Setter(
        scripting::Scripting* scripting,
        const sad::String& name
    ) : scripting::AbstractSetter<sad::Way*,_PropertyType>(scripting)
    {
        this->setPropertyName(name);
    }

    /*! Could be inherited
     */ 
    virtual ~Setter()
    {
        
    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone() override
    {
        return new scripting::ways::Setter<_PropertyType, _CommandType>(*this);
    }

    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
     */
    virtual void callActions(sad::Way* obj, const sad::String& property_name, _PropertyType old_value, _PropertyType new_value) override
    {
        this->scripting::AbstractSetter<sad::Way*, _PropertyType>::callActions(obj, property_name, old_value, new_value);
        this->setProperty(obj, property_name, old_value, new_value);
    }

    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value 
        \param[in] new_value new value
     */
    virtual void setProperty(sad::Way* obj, const sad::String& property_name, _PropertyType old_value,  _PropertyType new_value)
    {
        core::Editor* editor = this->m_scripting->editor();

        history::Command* c =  new _CommandType(obj, old_value, new_value);
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
};

}

}
