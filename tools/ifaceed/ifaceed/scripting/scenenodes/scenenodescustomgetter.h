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
    \return a getter for custom property for custom object
 */
template<
    typename T
>
dukpp03::qt::Callable* custom_getter()
{
    scripting::AbstractGetter<sad::SceneNode*, T>* result = new scripting::AbstractGetter<sad::SceneNode*, T>();
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
        "maximallinewidth",
        "overflowstrategy",
        "breaktext",
        "textellipsisposition",
        "maximallinescount",
        "overflowstrategyforlines",
        "textellipsispositionforlines",
        "hasformatting",
        NULL
    };
    result->addExcludedPropertyNames(excluded);
    result->matchAllProperties();
    return result;
}

}

}
