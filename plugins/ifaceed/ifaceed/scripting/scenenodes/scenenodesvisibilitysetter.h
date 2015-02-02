/*! \file scenenodesvisibilitysetter.h
	\author HiddenSeeker

	Defines a setter for  visibility property of scene node
 */
#pragma once
#include "scenenodesabstractsetter.h"


namespace scripting
{
	
namespace scenenodes
{
	 
/*! Defines a visibility setter for scene node
 */
class VisibilitySetter: public scripting::scenenodes::AbstractSetter<bool>
{
public: 
	/*! Constructs new visibility setter 
		\param[in] e engine
	 */
	VisibilitySetter(QScriptEngine* e);
	/*! Can be inherited
	 */
	virtual  ~VisibilitySetter();
	/*! Returns command for editing a property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
		\return a command to be used
	 */
	virtual history::Command* command(sad::SceneNode* obj, const sad::String& propertyname, bool oldvalue,  bool newvalue);
};


}

}