/*! \file abstractautomaticgun.h
	\author HiddenSeeker

	Describes a basic automatic gun, that shoots to enemy or specified 
	direction by emitting some bullet with specified speed
 */

#include <time.h>
#include <p2d/app/objectemitter.h>
#include "constants.h"
#pragma once

class GameObject;
/*! A basic automatic gun can shoot. It's knows about position of object
	and tries to shoot by a bullet every period of time
 */
class AbstractAutomaticGun: public p2d::app::AbstractObjectEmitter
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
	  virtual p2d::app::App * app();
      /*! Sets difference between angle of object and shooting angle
		  \param[in] delta delta of angle
	   */
	  void setAngleDifference(double delta);
	  /*! Sets a game object
		  \param[in] o game object
	   */
	  void setObject(GameObject * o);
	  virtual ~AbstractAutomaticGun();
};
