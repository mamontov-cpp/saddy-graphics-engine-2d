/*! \file animationswaysetter.h
    

    A basic setter for setting way
 */
#pragma once
#include "../scripting.h"
#include "animationssetter.h"

#include "../../history/animations/animationschangewaymovingway.h"

#include <animations/animationswaymoving.h>

namespace scripting
{

namespace animations
{

class WaySetter: public scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>
{
public:
    /*! Construct new setter for property
        \param[in] scripting a scripting part
     */
    WaySetter(scripting::Scripting* scripting);
    
    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone();
    
    /*! Could be inherited
     */
    virtual ~WaySetter();
};

}

}
