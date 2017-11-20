/*! \file easinggetter.h

    A getter for property of easing function for animation
 */
#pragma once
#include "../abstractgetter.h"

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>


namespace scripting
{

namespace animations
{

/*! Defines a getter for getting a property from easing function of animation
 */
template<
    typename _PropertyType
>
class EasingGetter: public scripting::AbstractGetter<sad::animations::Animation*, _PropertyType>
{
public:
    /*! Constructs new getter for getting a property from easing function of animation
        \param[in] property_name name of property
        \param[in] getter a getter
     */
    EasingGetter(
        const sad::String& property_name,
        _PropertyType (sad::animations::easing::Function::*getter)() const
    )  : scripting::AbstractGetter<sad::animations::Animation*, _PropertyType>(), m_getter(getter)
    {
        this->setPropertyName(property_name);
    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::animations::EasingGetter<_PropertyType>(*this);
    }

    /*! Could be inherited
     */
    virtual ~EasingGetter()
    {
    
    }
protected:
    /*! Tries to return property value
        \param[in] value object
        \param[in] property_name a name of property
        \return property value if it exists, nothing otherwise
    */
    virtual  sad::Maybe<_PropertyType> tryGetPropertyValue(sad::animations::Animation* value, const sad::String& property_name)
    {
        return  sad::Maybe<_PropertyType>(((value->easing())->*m_getter)());
    }
    /*! A getter for easing
     */
    _PropertyType (sad::animations::easing::Function::*m_getter)() const;
};

}

}
