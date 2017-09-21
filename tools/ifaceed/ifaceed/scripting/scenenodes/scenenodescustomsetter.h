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
    /*! Creates new setter family for specified property type
        \param scripting a scripting object
     */
    CustomSetter(scripting::Scripting* scripting) : scripting::scenenodes::AbstractSetter<T>(scripting)
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
        this->matchAllProperties();
        this->addExcludedPropertyNames(excluded);
    }
    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::scenenodes::CustomSetter<T>(*this);
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
