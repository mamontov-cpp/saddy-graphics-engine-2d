/*! \file instancesnamesetter.h
	

	A setter for name for scene objects
 */
#pragma once
#include "../abstractsetter.h"
#include "../abstractcondition.h"

#include <animations/animationsinstance.h>

namespace scripting
{
	
namespace instances
{
/*! A setter for name animation resource name
 */
class AnimationSetter: public scripting::AbstractSetter<sad::animations::Instance*, sad::String>
{
public:
/*! Tests, whether property is empty or references an animation resource
 */
class IsAnimationResource: public scripting::AbstractCondition<sad::String>
{
public:
	/*! Constructs a  condition
	 */
	IsAnimationResource();
	/*! Can be inherited
	 */
	virtual ~IsAnimationResource();
	/*! Checks a condition for object
		\param[in] a an object
		\return if doesn't match -  a message
	 */
	virtual sad::Maybe<QString> check(const sad::String& a);
};
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
	 */
	AnimationSetter(
		QScriptEngine* e
	);
	/*! Could be inherited
	 */
	virtual ~AnimationSetter();
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(sad::animations::Instance* obj, const sad::String& propertyname, sad::String oldvalue,  sad::String newvalue);
};

}

}
