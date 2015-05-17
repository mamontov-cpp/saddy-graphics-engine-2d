/*! \file instanceswaysetter.h
	\author HiddenSeeker

	A setter for way for way animation instance
 */
#pragma once
#include "../abstractsetter.h"
#include "../abstractcondition.h"
#include "../abstracttovalue.h"

#include <animations/animationswayinstance.h>

namespace scripting
{
	
namespace instances
{
/*!A setter for way for way animation instance
 */
class WaySetter: public scripting::AbstractSetter<sad::animations::WayInstance*, unsigned long long>
{
public:
/*! Tests, whether property is empty or references an way resource
 */
class IsWay: public scripting::AbstractCondition<unsigned long long>
{
public:
	/*! Constructs a  condition
	 */
	IsWay();
	/*! Can be inherited
	 */
	virtual ~IsWay();
	/*! Checks a condition for object
		\param[in] a an object
		\return if doesn't match -  a message
	 */
	virtual sad::Maybe<QString> check(const unsigned long long& a);
};
/*! Converts a string to major id if can
 */
class StringToWay: public scripting::AbstractToValue<unsigned long long>
{
public:
	/*! Converts a value to value
	 */
	StringToWay();
	/*! Can be inherited
	 */
	virtual ~StringToWay();
	/*! Converts a resulting value to a value
		\param[in] v value
		\return result
	 */
	virtual sad::Maybe<unsigned long long> toValue(const QScriptValue& v);
};
	/*! Constructs setter
		\param[in] e engine
	 */
	WaySetter(
		QScriptEngine* e
	);
	/*! Could be inherited
	 */
	virtual ~WaySetter();
	/*! Sets property of object, making a command
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
        \param[in] oldid old value
        \param[in] newid new value
	 */
	virtual void setProperty(sad::animations::WayInstance* obj, const sad::String& propertyname, unsigned long long oldid,  unsigned long long newid);
};

}

}
