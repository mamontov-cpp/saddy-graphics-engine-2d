/*! \file    collidable.h
    \author  HiddenSeeker

	Contains a definitions of collidable basic objects and a collision detector node
*/
#include "hrect.h"
#include "scene.h"
#pragma once
/*! Bounding box
*/
typedef hRectF BoundingBox;
/*! Collidable basic object
*/
class Collidable: public sad::BasicNode
{
 protected:
		
 public:

	    virtual BoundingBox rect()=0;

		virtual ~Collidable();
};
