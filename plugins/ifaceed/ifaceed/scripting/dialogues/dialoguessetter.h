/*! \file dialoguessetter.h
	\author HiddenSeeker

	Defines a generic setter for setting a property of dialogue
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <dialogue/dialogue.h>

namespace scripting
{
	
namespace dialogues
{

/*! A common setter for setting common property
 */
template<
	typename _PropertyType,
	typename _CommandType
>
class Setter: public scripting::AbstractSetter<sad::dialogue::Dialogue*,_PropertyType>
{
public:	
	/*! Construct new setter for property
		\param[in] e engine
		\param[in] name a name for property
	 */
	Setter(
		QScriptEngine* e,
		const QString& name = ""
	) : scripting::AbstractSetter<sad::dialogue::Dialogue*,_PropertyType>(e, "set")
	{
		if (name.length()) {
			this->addMatched(name);
		}
	}
	/*! Could be inherited
	 */ 
	virtual ~Setter()
	{
		
	}

	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(sad::dialogue::Dialogue* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
	{
		QScriptValue main = this->engine()->globalObject().property("---");
		scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
		core::Editor* editor =  e->panel()->editor();

		history::Command* c =  new _CommandType(obj, oldvalue, newvalue);							
		editor->currentBatchCommand()->add(c);
		c->commit(editor);
	}
};

}

}
