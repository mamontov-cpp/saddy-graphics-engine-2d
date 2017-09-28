/*! \file scenenodessetter.h
    

    Defines a generic setter for setting a common property
 */
#pragma once
#include "scenenodesabstractsetter.h"

namespace scripting
{
    
namespace scenenodes
{

/*! A common setter for setting a property of scene node
 */
template<
    typename _PropertyType,
    typename _CommandType
>
class Setter: public scripting::scenenodes::AbstractSetter<_PropertyType>
{
public:	
    /*! Constructs new setter for property
        \param[in] scripting a scripting object
        \param[in] property_name a name for property to be handled by setter
     */
    Setter(
        scripting::Scripting* scripting,
        const sad::String& property_name
    ) : scripting::scenenodes::AbstractSetter<_PropertyType>(scripting, property_name)
    {

    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::scenenodes::Setter<_PropertyType, _CommandType>(*this);
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
