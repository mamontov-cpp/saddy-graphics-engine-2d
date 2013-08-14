/*! \file collisiontest.h
	\author HiddenSeeker

	Defines a collision tester, which can determine, whether two
	shapes are colliding
 */
#pragma once
#include "collisionmultimethod.h"
#include "rectangle.h"
#include "circle.h"
#include "line.h"

namespace p2d
{
/*! A collision tester can determine, whether two shapes are colliding
 */
class CollisionTest: public p2d::CollisionMultiMethod<bool>
{
 private:
	 static bool collidesRtoR(p2d::Rectangle * p1, p2d::Rectangle * p2);
	 static bool collidesRtoC(p2d::Rectangle * p1, p2d::Circle * p2);
	 static bool collidesRtoL(p2d::Rectangle * p1, p2d::Line * p2);
	 static bool collidesCtoC(p2d::Circle * p1, p2d::Circle * p2);
	 static bool collidesCtoL(p2d::Circle * p1, p2d::Line * p2);
	 static bool collidesLtoL(p2d::Line * p1, p2d::Line * p2);
 protected:
	 /*! This function inits all callbacks. 
	  */
	 virtual void init();
};

}


