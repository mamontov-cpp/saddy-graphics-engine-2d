/*! \file instancesanimationdbsetter.h
    

    A setter for animation from database
 */
#pragma once
#include "../abstractsetter.h"
#include "../abstractcondition.h"
#include "../abstracttovalue.h"

#include <animations/animationsinstance.h>

namespace scripting
{
    
namespace instances
{
/*! A setter for name animation resource name
 */
class AnimationDBSetter: public scripting::AbstractSetter<sad::animations::Instance*, unsigned long long>
{
public:
/*! Tests, whether property is empty or references an animation resource
 */
class IsAnimation: public scripting::AbstractCondition<unsigned long long>
{
public:
    /*! Constructs a  condition
     */
    IsAnimation();
    /*! Can be inherited
     */
    virtual ~IsAnimation();
    /*! Checks a condition for object
        \param[in] a an object
        \return if doesn't match -  a message
     */
    virtual sad::Maybe<QString> check(const unsigned long long& a);
};
/*! Converts a string to major id if can
 */
class StringToAnimation: public scripting::AbstractToValue<unsigned long long>
{
public:
    /*! Converts a value to value
     */
    StringToAnimation();
    /*! Can be inherited
     */
    virtual ~StringToAnimation();
    /*! Converts a resulting value to a value
        \param[in] v value
        \return result
     */
    virtual sad::Maybe<unsigned long long> toValue(const QScriptValue& v);
};
    /*! Represents a constructor call for a function with two arguments
        \param[in] e engine
     */
    AnimationDBSetter(
        QScriptEngine* e
    );
    /*! Could be inherited
     */
    virtual ~AnimationDBSetter();
    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldid old value
        \param[in] newid new value
     */
    virtual void setProperty(sad::animations::Instance* obj, const sad::String& propertyname, unsigned long long oldid,  unsigned long long newid);
};

}

}
