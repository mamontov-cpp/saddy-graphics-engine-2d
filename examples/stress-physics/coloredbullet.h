/*! \file coloredbullet.h
    

    Describes a colored bullet
 */
#pragma once
#include <p2d/app/object.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <p2d/app/constants.h>
#pragma once

/*! As a default, grid node is a node
 */
class ColoredBullet: public sad::p2d::app::Object
{
 SAD_OBJECT
 public:
     ColoredBullet();
};
