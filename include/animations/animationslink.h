/*! \file animations/animationslink.h
    \author HiddenSeeker

    Defines an animations link as link to both db and resource tree
 */
#pragma once
#include "../treedblink.h"

#include "animationsanimation.h"

namespace sad
{

namespace animations
{

typedef sad::TreeDbLink<sad::animations::Animation> Link;

}

}
