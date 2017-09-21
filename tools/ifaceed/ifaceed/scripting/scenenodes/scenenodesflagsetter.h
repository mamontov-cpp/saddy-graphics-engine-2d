/*! \file scenenodesflagsetter.h
    

    Defines a setter for  visibility property of scene node
 */
#pragma once
#include "scenenodesabstractsetter.h"

namespace core
{
class Editor;
}

namespace scripting
{
    
namespace scenenodes
{
     
/*! Defines a visibility setter for scene node
 */
class FlagSetter: public scripting::scenenodes::AbstractSetter<bool>
{
public: 
    /*! A callback for building command 
     */
    typedef history::Command* (*Callback)(core::Editor*, sad::SceneNode*, bool, bool);

    /*! Constructs new setter, which receives flag
        \param[in] scripting a scripting
        \param[in] property_name a property name, that will be handled by setter
        \param[in] f a callback for getting a command to be applied
     */
    FlagSetter(
        scripting::Scripting* scripting,
        const sad::String& property_name,
        scripting::scenenodes::FlagSetter::Callback f
    );

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone();

    /*! Can be inherited
     */
    virtual  ~FlagSetter();

    /*! Returns command for editing a property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
        \return a command to be used
     */
    virtual history::Command* command(sad::SceneNode* obj, const sad::String& propertyname, bool oldvalue,  bool newvalue);
protected:
    /*! A callback
     */
    Callback m_f;
};


}

}
