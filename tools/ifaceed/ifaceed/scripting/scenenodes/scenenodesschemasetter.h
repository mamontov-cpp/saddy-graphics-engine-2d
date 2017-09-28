/*! \file scenenodesschemasetter.h
    

    Describes a setter for schema property of scene node
 */
#pragma once
#include "scenenodessetter.h"

namespace scripting
{

namespace scenenodes
{
/*! A setter for schema property of scene node
 */
class SchemaSetter: public scripting::scenenodes::AbstractSetter<sad::String>
{
public:
    /*! Creates new setter for specified area
        \param scripting a scripting for an area
     */
    SchemaSetter(scripting::Scripting* scripting);
    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone();
    /*! Can be inherited
     */
    virtual ~SchemaSetter();
    /*! Returns command for editing a property
        \param[in] obj an object to be set
        \param[in] prop property name
        \param[in] oldvalue old value 
        \param[in] newvalue new value
        \return a command to be used
     */
    virtual history::Command* command(sad::SceneNode* obj, const sad::String& prop, sad::String oldvalue,  sad::String newvalue);
};

}

}
