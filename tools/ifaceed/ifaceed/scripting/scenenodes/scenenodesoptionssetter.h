/*! \file scenenodesoptionssetter.h
    

    Describes a setter for options property of scene node (used for sad::Sprite2D and custom objects)
 */
#pragma once
#include "scenenodessetter.h"

namespace scripting
{

namespace scenenodes
{
/*! A setter for options property of sprite
 */
class OptionsSetter: public scripting::scenenodes::AbstractSetter<sad::String>
{
public:
    /*! Creates new setter for setting options property of object
        \param scripting a scripting object
     */
    OptionsSetter(scripting::Scripting* scripting);
    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone();
    /*! Could be inherited
     */
    virtual ~OptionsSetter();
    /*! Returns command for editing a property
        \param[in] obj an object to be set
        \param[in] prop property name
        \param[in] oldvalue old value 
        \param[in] newvalue new value
        \return a command to be used
     */
    virtual history::Command* command(sad::SceneNode* obj, const sad::String& prop, sad::String oldvalue, sad::String newvalue);
};

}

}
