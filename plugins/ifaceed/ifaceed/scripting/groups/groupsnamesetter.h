/*! \file groupsnamesetter.h
	

	A setter for name for animation group
 */
#pragma once
#include "../abstractsetter.h"

#include <animations/animationsgroup.h>

namespace scripting
{
	
namespace groups
{
/*! A setter for name property name
 */
class NameSetter: public scripting::AbstractSetter<sad::animations::Group*, sad::String>
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
	virtual void setProperty(sad::animations::Group* obj, const sad::String& propertyname, sad::String oldvalue,  sad::String newvalue);
};

}

}
