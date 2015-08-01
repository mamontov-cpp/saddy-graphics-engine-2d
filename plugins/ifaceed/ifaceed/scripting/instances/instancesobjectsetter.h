/*! \file instancesobjectsetter.h
	

	A setter for object for animation instance
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
/*! A setter for object for animation instance
 */
class ObjectSetter: public scripting::AbstractSetter<sad::animations::Instance*, unsigned long long>
{
public:
/*! Tests, whether property is empty or references an object resource
 */
class IsObject: public scripting::AbstractCondition<unsigned long long>
{
public:
	/*! Constructs a  condition
	 */
	IsObject();
	/*! Can be inherited
	 */
	virtual ~IsObject();
	/*! Checks a condition for object
		\param[in] a an object
		\return if doesn't match -  a message
	 */
	virtual sad::Maybe<QString> check(const unsigned long long& a);
};
/*! Converts a string to major id if can
 */
class StringToObject: public scripting::AbstractToValue<unsigned long long>
{
public:
	/*! Converts a value to value
	 */
	StringToObject();
	/*! Can be inherited
	 */
	virtual ~StringToObject();
	/*! Converts a resulting value to a value
		\param[in] v value
		\return result
	 */
	virtual sad::Maybe<unsigned long long> toValue(const QScriptValue& v);
};
	/*! Constructs setter
		\param[in] e engine
	 */
	ObjectSetter(
		QScriptEngine* e
	);
	/*! Could be inherited
	 */
	virtual ~ObjectSetter();
	/*! Sets property of object, making a command
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
        \param[in] oldid old value
        \param[in] newid new value
	 */
	virtual void setProperty(sad::animations::Instance* obj, const sad::String& propertyname, unsigned long long oldid,  unsigned long long newid);
};

}

}
