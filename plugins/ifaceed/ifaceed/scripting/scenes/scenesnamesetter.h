/*! \file scenesnamesetter.h
	\author HiddenSeeker

	A setter for name for scene objects
 */
#pragma once
#include "../abstractsetter.h"

namespace scripting
{
	
namespace scenes
{
	
class NameSetter: public scripting::AbstractSetter<sad::Scene*, sad::String>
{
public:
	/*! Represents a constructor call for a function with two arguments
		\param[in] e engine
	 */
	NameSetter(
		QScriptEngine* e
	);
	/*! Could be inherited
	 */
	virtual ~NameSetter();
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(sad::Scene* obj, const sad::String& propertyname, sad::String oldvalue,  sad::String newvalue);
};

}

}
