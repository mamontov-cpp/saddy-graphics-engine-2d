/*! \file scenenodescustomgetter.h
	

	Defines a custom getter for custom property from custom objects 
 */
#pragma once
#include "../abstractgetter.h"

namespace scripting
{

namespace scenenodes
{

/*! Returns a getter for custom property for custom objects
 */
template<
	typename T
>
scripting::Callable* custom_getter(QScriptEngine* engine)
{
	scripting::AbstractGetter<sad::SceneNode*, T>* result = new scripting::AbstractGetter<sad::SceneNode*, T>(engine);
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
		result->addExcluded(excluded[i]);
		++i;
	}
	return result;
}

}

}
