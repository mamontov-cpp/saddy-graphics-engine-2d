/*! \file multisamplingcollisiondetector.h
	\author HiddenSeeker

	Determines a collision detector, that performs a sequence of test
	to determine whether two bodies collide
 */
#include "collisiondetector.h"
#pragma once

namespace p2d
{
/*! A simple collision detector, which tests collision shapes at specified
	time, defined as proportion to the time limit
 */
class MultisamplingCollisionDetector: public p2d::CollisionDetector
{
 private:
	 p2d::CollisionTest * m_tester; // !< A tester, which tests shapes for collisions
	 unsigned int  m_tests;  //!< An amount of tests to be performed
 public:
	 /*! Creates new detector
		 \param[in] tests amount of tests to be performed on each iteration
		 \param[in] t     a shape collision testing callbacks
	  */
	 inline MultisamplingCollisionDetector(
		 unsigned int tests  = 4,
		 p2d::CollisionTest * t = new p2d::CollisionTest()
	 )
	 : m_tester(t), m_tests(tests)
	 {
		 if (tests == 0) tests =1;
	 }

	 /*! Tests, whether two bodies collide within specified limit
		  testing their movement in interval [0, limit] and returning
		  time if possible
		  \param[in] b1 first body
		  \param[in] b2 second body
		  \param[in] time time limit
		  \return if detected collision - moment * limit
	   */
	  virtual p2d::MaybeTime collides(p2d::Body * b1, 
									  p2d::Body * b2, 
									  double limit);

	 ~MultisamplingCollisionDetector();
};

}
