/*! \file groupsloopedsetter.h
	

	A setter for looped property for animation group
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
class LoopedSetter: public scripting::AbstractSetter<sad::animations::Group*, bool>
{
public:
	/*! Constructs setter
		\param[in] e engine
	 */
	LoopedSetter(
		QScriptEngine* e
	);
	/*! Could be inherited
	 */
	virtual ~LoopedSetter();
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(sad::animations::Group* obj, const sad::String& propertyname, bool oldvalue,  bool newvalue);
};

}

}
