/*! \file gridnode.h
    

    Describes a basic grid node
 */
#pragma once
#include <p2d/app/object.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <p2d/app/constants.h>
#pragma once

/*! As a default, grid node is a node
 */
class GridNode: public sad::p2d::app::Object
{
 SAD_OBJECT
 public:
     GridNode();
};
