/*! \file uncoloredbullet.h
    

    Describes a uncolored bullet
 */
#pragma once
#include <p2d/app/object.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <p2d/app/constants.h>


/*! As a default, grid node is a node
 */
class UncoloredBullet: public sad::p2d::app::Object
{
 SAD_OBJECT
 public:
     UncoloredBullet();
};
