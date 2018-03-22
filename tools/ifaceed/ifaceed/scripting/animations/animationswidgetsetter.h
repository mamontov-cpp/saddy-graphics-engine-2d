/*! \file animationswidgetsetter.h
    

    Defines a generic setter for setting a property of animations, where it's binded 
    to a widget
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

namespace scripting
{
    
namespace animations
{

/*! A common setter for setting common property of animations, where it's binded 
    to a widget
 */
template<
    typename _AnimationType,
    typename _WidgetType,
    typename _PropertyType,
    typename _CommandType
>
class WidgetSetter: public scripting::AbstractSetter<_AnimationType*,_PropertyType>
{
public:	
    /*! Construct new setter for property
        \param[in] scripting a scripting party
        \param[in] widget a widget part
        \param[in] name a name for property
     */
    WidgetSetter(
        scripting::Scripting* scripting,
        _WidgetType widget,
        const sad::String& name = ""
    ) : scripting::AbstractSetter<_AnimationType*,_PropertyType>(scripting), m_widget(widget)
    {
        this->setPropertyName(name);
    }

    /*! Clones an object
        \return copy of object
    */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::animations::WidgetSetter<_AnimationType, _WidgetType,  _PropertyType, _CommandType>(*this);
    }

    /*! Could be inherited
     */ 
    virtual ~WidgetSetter()
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
        scripting::Scripting* e = this->m_scripting;
        core::Editor* editor =  e->editor();

        history::Command* c =  new _CommandType(obj, propertyname, m_widget, oldvalue, newvalue);
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
protected:
    /*! Widget part
     */
    _WidgetType m_widget;
};

}

}
