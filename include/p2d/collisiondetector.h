/*! \file collisiondetector.h
	\author HiddenSeeker

	Describes a basic collision detector, used to determine, whether
	bodies collide and when does it occured.
 */
#include "body.h"
#include "collisiontest.h"
#include "../templates/maybe.hpp"
#pragma once

namespace p2d
{
typedef hst::Maybe<double> MaybeTime;
/*! A basic collision detector, used to determine, whether bodies
	collide and when does it ocurred.
 */
class CollisionDetector
{
  public:
	  /*! Tests, whether two bodies collide within specified limit
		  testing their movement in interval [0, limit] and returning
		  time if possible
		  \param[in] b1 first body
		  \param[in] b2 second body
		  \param[in] time time limit
		  \return time of impact if possible
	   */
	  virtual p2d::MaybeTime collides(p2d::Body * b1, 
									  p2d::Body * b2, 
									  double limit) = 0;	  
	  virtual ~CollisionDetector();
};

}
