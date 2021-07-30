/*! \file collisiontest.h
    

    Defines a collision tester, which can determine, whether two
    shapes are colliding
 */
#pragma once
#include "collisionmultimethod.h"
#include "rectangle.h"
#include "circle.h"
#include "line.h"
#include "bounds.h"

namespace sad
{

namespace p2d
{
/*! A collision tester can determine, whether two shapes are colliding
 */
class CollisionTest: public p2d::CollisionMultiMethod<bool>
{
public:
     static bool collidesBtoS(p2d::Bound * p1, CollisionShape * p2);
protected:
     /*! This function initializes all callbacks. 
      */
     virtual void init() override;
private:
     static bool collidesRtoR(p2d::Rectangle * p1, p2d::Rectangle * p2);
     static bool collidesRtoC(p2d::Rectangle * p1, p2d::Circle * p2);
     static bool collidesRtoL(p2d::Rectangle * p1, p2d::Line * p2);
     static bool collidesCtoC(p2d::Circle * p1, p2d::Circle * p2);
     static bool collidesCtoL(p2d::Circle * p1, p2d::Line * p2);
     static bool collidesLtoL(p2d::Line * p1, p2d::Line * p2);

     static bool collidesBtoR(p2d::Bound * p1, p2d::Rectangle * p2);
     static bool collidesBtoC(p2d::Bound * p1, p2d::Circle * p2);
     static bool collidesBtoL(p2d::Bound * p1, p2d::Line * p2);
     static bool collidesBtoB(p2d::Bound * p1, p2d::Bound * p2);
};

}

}

