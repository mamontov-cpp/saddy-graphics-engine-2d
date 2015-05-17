/*! \file scenenodesoptionssetter.h
    \author HiddenSeeker

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
    /*! Creates new setter for specified area
        \param e
     */
    OptionsSetter(QScriptEngine* e);
    /*! Can be inherited
     */
    virtual ~OptionsSetter();
    /*! Determines, whether it can be called with this context
        Additionally checks, whether font exists
        \param[in] ctx context
        \return whether it could be called, or error
     */
    virtual scripting::MatchResult canBeCalled(QScriptContext* ctx);
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
