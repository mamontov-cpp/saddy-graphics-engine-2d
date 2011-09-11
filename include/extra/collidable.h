/*! \file    collidable.h
    \author  HiddenSeeker

	Contains a definitions of collidable basic objects and a collision detector node
*/
#include "hxyrect.h"
#include "../scene.h"
#pragma once
/*! Bounding box
*/
typedef hst::xyrect BoundingBox;
/*! Collidable basic object
*/
class Collidable: public sad::BasicNode
{
 SAD_NODE
 protected:
		
 public:

	    virtual BoundingBox rect()=0;

		virtual ~Collidable();
};
