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
        \param[in] e engine
        \param[in] widget a widget part
        \param[in] name a name for property
     */
    WidgetSetter(
        QScriptEngine* e,
        _WidgetType widget,
        const QString& name = ""
    ) : scripting::AbstractSetter<_AnimationType*,_PropertyType>(e, "set"), m_widget(widget)
    {
        if (name.length()) {
            this->addMatched(name);
        }
    }
    /*! Could be inherited
     */ 
    virtual ~WidgetSetter()
    {
        
    }

    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(_AnimationType* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
    {
        QScriptValue main = this->engine()->globalObject().property("---");
        scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
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
