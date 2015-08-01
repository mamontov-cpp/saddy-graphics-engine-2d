/*! \file scenenodesabstractsetter.h
	

	Defines an abstract setter for properties of scene node
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"

#include <scenenode.h>



namespace scripting
{
	
namespace scenenodes
{

/*! Defines an abstract setter for properties of scene node
 */
template<
	typename _PropertyType
>
class AbstractSetter: public scripting::AbstractSetter<sad::SceneNode*, _PropertyType>
{
public:
	/*! Construct new setter for property
		\param[in] e engine for property
	 */
	AbstractSetter(
		QScriptEngine* e
	) : scripting::AbstractSetter<sad::SceneNode*, _PropertyType>(e, "set")
	{
		
	}
	/*! Could be inherited
	 */
	virtual ~AbstractSetter()
	{
		
	}
	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(sad::SceneNode* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
	{
		QScriptValue main = this->engine()->globalObject().property("---");
		scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
		core::Editor* editor =  e->panel()->editor();

		history::Command* c = this->command(obj, propertyname, oldvalue, newvalue);								
		editor->currentBatchCommand()->add(c);
		c->commit(editor);
	}
	/*! Returns command for editing a property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
		\return a command to be used
	 */
	virtual history::Command* command(sad::SceneNode* obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue) = 0;
};

}

}
