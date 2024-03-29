/*! \file animationspoint2dsetter.h
    

    Defines a generic setter for setting a property of animations, which is stored as sad::Point2D
    and displayed, as two QDoubleSpinBox
 */
#pragma once
#include <QDoubleSpinBox>
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"


namespace scripting
{
    
namespace animations
{

/*! A common setter for setting a property of animation, which is stored as sad::Point2D and
    displayed as two QDoubleSpinBox
 */
template<
    typename _AnimationType,
    typename _CommandType
>
class Point2DSetter: public scripting::AbstractSetter<_AnimationType*,sad::Point2D>
{
public:
    /*! Construct new setter for property
        \param[in] scripting a scripting part
        \param[in] name a name for property
        \param[in] x_widget a widget for storing x coordinate
        \param[in] y_widget a widget for storing y coordinate
     */
    Point2DSetter(
        scripting::Scripting* scripting,
        const sad::String& name,
        QDoubleSpinBox* x_widget,
        QDoubleSpinBox* y_widget
    ) : scripting::AbstractSetter<_AnimationType*,sad::Point2D>(scripting), m_x_widget(x_widget), m_y_widget(y_widget)
    {
        this->setPropertyName(name);
    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone() override
    {
        return new scripting::animations::Point2DSetter<_AnimationType, _CommandType>(*this);
    }

    /*! Could be inherited
     */ 
    virtual ~Point2DSetter() override
    {
        
    }

    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
    */
    virtual void callActions(_AnimationType* obj, const sad::String& property_name, sad::Point2D old_value, sad::Point2D new_value) override
    {
        this->scripting::AbstractSetter<_AnimationType*, sad::Point2D>::callActions(obj, property_name, old_value, new_value);
        this->setProperty(obj, property_name, old_value, new_value);
    }


    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value 
        \param[in] new_value new value
     */
    virtual void setProperty(_AnimationType* obj, const sad::String& property_name, sad::Point2D old_value,  sad::Point2D new_value)
    {
        core::Editor* editor =  this->m_scripting->editor();

        history::Command* c =  new _CommandType(obj, property_name, old_value, new_value, m_x_widget, m_y_widget);
        editor->currentBatchCommand()->add(c);
        c->commit(editor);
    }
protected:
    /*! A widget for displaying x coordinate
     */
    QDoubleSpinBox* m_x_widget;
    /*! A widget for displaying y coordinate
     */
    QDoubleSpinBox* m_y_widget;
};

}

}
