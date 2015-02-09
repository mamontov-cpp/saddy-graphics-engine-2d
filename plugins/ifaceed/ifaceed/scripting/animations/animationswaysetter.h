/*! \file animationswaysetter.h
    \author HiddenSeeker

    A basic setter for setting way
 */
#pragma once
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
        \param[in] e engine
        \param[in] name a name for property
     */
    WaySetter(
        QScriptEngine* e,
    );
    /*! Could be inherited
     */
    virtual ~WaySetter();
    /*! Determines, whether it can be called with this context
        \param[in] ctx context
        \return whether it could be called, or error
     */
    virtual sad::Maybe<QString> canBeCalled(QScriptContext* ctx);
};

}

}
