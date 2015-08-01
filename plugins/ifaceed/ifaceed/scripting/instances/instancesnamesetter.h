/*! \file instancesnamesetter.h
	

	A setter for name for animation instance
 */
#pragma once
#include "../abstractsetter.h"

#include <animations/animationsinstance.h>

namespace scripting
{
	
namespace instances
{
/*! A setter for name property name
 */
class NameSetter: public scripting::AbstractSetter<sad::animations::Instance*, sad::String>
{
public:
	/*! Constructs setter
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
	virtual void setProperty(sad::animations::Instance* obj, const sad::String& propertyname, sad::String oldvalue,  sad::String newvalue);
};

}

}
