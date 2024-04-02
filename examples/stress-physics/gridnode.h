/*! \file gridnode.h
    

    Describes a basic grid node
 */
#pragma once
#include <phy52d/object.h>
#pragma once

/*! As a default, grid node is a node
 */
class GridNode: public sad::phy52d::Object
{
 SAD_OBJECT
 public:
     GridNode();
};
