/*! \file groupssequentialsetter.h
    

    A setter for sequential property for animation group, which responds, whether instances in group should be played sequentially or parallel
 */
#pragma once
#include "../abstractsetter.h"

#include <animations/animationsgroup.h>

namespace scripting
{
    
namespace groups
{
/*! A setter for sequential property for animation group, which responds, whether instances in group should be played sequentially or parallel
 */
class SequentialSetter: public scripting::AbstractSetter<sad::animations::Group*, bool>
{
public:
    /*! Constructs setter
        \param[in] e engine
     */
    SequentialSetter(
        QScriptEngine* e
    );
    /*! Could be inherited
     */
    virtual ~SequentialSetter();
    /*! Performs actually setting property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(sad::animations::Group* obj, const sad::String& propertyname, bool oldvalue,  bool newvalue);
};

}

}
