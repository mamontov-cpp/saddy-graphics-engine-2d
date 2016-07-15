/*! \file instancesstarttimesetter.h
    

    A setter for start time for animation instance
 */
#pragma once
#include "../abstractsetter.h"

#include <animations/animationsinstance.h>

namespace scripting
{
    
namespace instances
{
/*! A setter for start time for animation instance
 */
class StartTimeSetter: public scripting::AbstractSetter<sad::animations::Instance*, double>
{
public:
    /*! Constructs setter
        \param[in] e engine
     */
    StartTimeSetter(
        QScriptEngine* e
    );
    /*! Could be inherited
     */
    virtual ~StartTimeSetter();
    /*!Sets property of object, making a command
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
     */
    virtual void setProperty(sad::animations::Instance* obj, const sad::String& propertyname, double oldvalue,  double newvalue);
};

}

}
