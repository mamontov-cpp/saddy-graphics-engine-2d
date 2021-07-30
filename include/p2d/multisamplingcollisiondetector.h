/*! \file multisamplingcollisiondetector.h
    

    Determines a collision detector, that performs a sequence of test
    to determine whether two bodies collide
 */
#pragma once
#include "collisiondetector.h"

namespace sad
{

namespace p2d
{
/*! A simple collision detector, which tests collision shapes at specified
    time, defined as proportion to the time limit
 */
class MultisamplingCollisionDetector: public p2d::CollisionDetector
{
SAD_OBJECT
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
         if (m_tests == 0) m_tests =1;
     }

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
                                      double limit) override;

      /*! Returns a sample  count, needed to fetch samples for detection
       */
      virtual int sampleCount() const override;

     ~MultisamplingCollisionDetector() override;
private:
     p2d::CollisionTest * m_tester; // !< A tester, which tests shapes for collisions
     unsigned int  m_tests;  //!< An amount of tests to be performed
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::MultisamplingCollisionDetector)