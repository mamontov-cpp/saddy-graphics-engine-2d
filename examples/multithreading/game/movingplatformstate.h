/*!   \file movingplatformstate.h

      A state for moving platform
 */
#pragma once
#include <p2d/body.h>
#include <way.h>

namespace game
{

/*! A state for moving platform, used to determine behaviour
 *  in registry
 */
struct MovingPlatformState
{
    /*! A platform which, should be tracked
     */
    sad::p2d::Body* Platform;
    /*! A way, where should platform go
     */
    sad::Way* Way;
    /*! A local time for determining new location
     */
    double Time;
    /*! Whether we should substract time instead of adding
     */
    bool Downward;
};

}

