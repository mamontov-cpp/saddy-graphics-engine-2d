/*! \file broadcollisiondetector.h
    

    Determines a collision detector, based on building an extended figure
    out of shapes at beginning and end. Returns half of time limit if
    collision is detected.
 */
#pragma once
#include "collisiondetector.h"
#include "collisiontest.h"


namespace sad
{

namespace p2d
{
class SimpleCollisionDetector;
/*! Collision detector, based on building an extended figure
    out of shapes at beginning and end. Returns half of time limit if
    collision is detected.
 */
class BroadCollisionDetector: public p2d::CollisionDetector
{
SAD_OBJECT
public:
     BroadCollisionDetector(p2d::CollisionTest * test = new p2d::CollisionTest());
     ~BroadCollisionDetector();
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
protected:
     p2d::CollisionTest * m_tester; //!< A collision tester, used to determining a data
     p2d::SimpleCollisionDetector * m_detector; //!< Inner detector for simplified computing of collisions
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::BroadCollisionDetector)