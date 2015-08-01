/*! \file scenenodessetter.h
	

	Defines a generic setter for setting a common property
 */
#pragma once
#include "scenenodesabstractsetter.h"

namespace scripting
{
	
namespace scenenodes
{

/*! A common setter for setting common property
 */
template<
	typename _PropertyType,
	typename _CommandType
>
class Setter: public scripting::scenenodes::AbstractSetter<_PropertyType>
{
public:	
	/*! Construct new setter for property
		\param[in] e engine
		\param[in] name a name for property
	 */
	Setter(
		QScriptEngine* e,
		const QString& name = ""
	) : scripting::scenenodes::AbstractSetter<_PropertyType>(e)
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
	/*! Returns command for editing a property
		\param[in] obj an object to be set
		\param[in] oldvalue old value 
		\param[in] newvalue new value
		\return a command to be used
	 */
	virtual history::Command* command(sad::SceneNode* obj, const sad::String&, _PropertyType oldvalue,  _PropertyType newvalue)
	{
		return new _CommandType(obj, oldvalue, newvalue);	
	}
};

}

}
