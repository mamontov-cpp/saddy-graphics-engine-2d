/*! \file scenenodescustomsetter.h
	

	Defines a custom setter for custom property from custom objects 
 */
#pragma once
#include "scenenodessetter.h"

#include "../../history/customobject/customobjectchangeproperty.h"

namespace scripting
{

namespace scenenodes
{


/*! A setter for custom property of custom object
 */
template<
	typename T
>
class CustomSetter: public scripting::scenenodes::AbstractSetter<T>
{
public:
    /*! Creates new setter for specified area
        \param e
     */
    CustomSetter(QScriptEngine* e) : scripting::scenenodes::AbstractSetter<T>(e)
    {
	    const char* excluded[] = {
			"name",
			"texturecoordinates",
			"layer",
			"majorid",
			"minorid",
			"scene",
			"visible",
			"area",
			"angle",
			"color",
			"fontsize",
			"text",
			"linespacing",
			"font",
			"flipx",
			"flipy",
			"options",
			"schema",
			NULL
		};
		int i = 0;
		while(excluded[i] != NULL) {
			this->addExcluded(excluded[i]);
			++i;
		}
    }
    /*! Can be inherited
     */
    virtual ~CustomSetter()
    {
	    
    }
	/*! Returns command for editing a property
		\param[in] obj an object to be set
		\param[in] prop property name
		\param[in] oldvalue old value 
		\param[in] newvalue new value
		\return a command to be used
	 */
	virtual history::Command* command(
		sad::SceneNode* obj, 
		const sad::String& prop, 
		T oldvalue,  
		T newvalue
	)
    {
	    return new history::customobject::ChangeProperty<T>(obj, prop, oldvalue, newvalue);
    }
};

}

}
