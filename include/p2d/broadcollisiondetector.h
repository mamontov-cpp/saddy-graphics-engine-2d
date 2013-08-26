/*! \file broadcollisiondetector.h
	\author HiddenSeeker

	Determines a collision detector, based on building an extended figure
	out of shapes at beginning and end. Returns half of time limit if
	collision is detected.
 */
#include "collisiondetector.h"
#pragma once


namespace p2d
{
/*! Collision detector, based on building an extended figure
	out of shapes at beginning and end. Returns half of time limit if
	collision is detected.
 */
class BroadCollisionDetector: public p2d::CollisionDetector
{
 private:
 public:
	 /*! Tests, whether two bodies collide within specified limit
		  testing their movement in interval [0, limit] and returning
		  time if possible
		  \param[in] b1 first body
		  \param[in] b2 second body
		  \param[in] limit a time limit for finding a position of bodies
		  \return if detected collision - moment * limit
	   */
	  virtual p2d::MaybeTime collides(p2d::Body * b1, 
									  p2d::Body * b2, 
									  double limit);
};

}

