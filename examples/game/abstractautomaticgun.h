/*! \file abstractautomaticgun.h
    

    Describes a basic automatic gun, that shoots to enemy or specified 
    direction by emitting some bullet with specified speed
 */

#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "constants.h"

#include <phy52d/objectemitter.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <ctime>


class GameObject;
/*! A basic automatic gun can shoot. It's knows about position of object
    and tries to shoot by a bullet every period of time
 */
class AbstractAutomaticGun: public sad::phy52d::AbstractObjectEmitter
{
  protected:
      /*! A game object, which has a gun
       */
      GameObject * m_object;
      /*! A delta between game object angle and automatic gun's angle.
          For most parts it's need to be zero because sprites are facing right,
          and cos of zero will face bullets into opposite direction
       */
      double m_dangle;
  public:
      /*! As a default creates a enabled gun with M_PI, interval is 50 milliseconds
       */
      AbstractAutomaticGun();
      /*! Tries to shoot as fast as it can
       */
      void tryShoot();

      /*! Returns an application
            \return application
        */
      virtual sad::phy52d::App * app() override;
      /*! Sets difference between angle of object and shooting angle
          \param[in] delta delta of angle
       */
      void setAngleDifference(double delta);
      /*! Sets a game object
          \param[in] o game object
       */
      void setObject(GameObject * o);
      /*! Returns angle for created object (0 in our case)
          \return angle
       */
      virtual double angle() override;
      virtual ~AbstractAutomaticGun() override;
};
