/*! \file automaticgun.h
	\author HiddenSeeker

	Describes an automatic gun, that shoots to enemy or specified 
	direction by emitting some bullet with specified speed
 */
#include <time.h>
#include "gameobjectconstants.h"
#pragma once

class GameObject;
/*! A basic automatic gun can shoot. It's knows about position of object
	and tries to shoot by a bullet every period of time
 */
class AbstractAutomaticGun
{
  protected:
	  /*! A game object, which has a gun
	   */
	  GameObject * m_object;
	  /*! A delta between game object angle and automatic gun's angle.
		  For most parts it's need to be M_PI because sprites are facing right,
		  and cos of zero will face bullets into opposite direction
	   */
	  double m_dangle;
	  /*! A time when gun last time shoot
	   */
	  clock_t m_last_shot;
	  /*! A interval of periodic time
	   */
	  clock_t m_interval;
	  /*! Whether automatic gun is enabled. A gun shoots periodically,
		  when enabled
	   */
	  bool m_enabled;
	  /*! Real shooting function, means that gun emits a bullet
	   */
	  virtual void shoot()=0;
  public:
	  /*! As a default creates a enabled gun with M_PI, interval is 50 milliseconds
	   */
	  AbstractAutomaticGun();
	  /*! Tries to shoot as fast as it can
	   */
	  void tryShoot();
      /*! Sets difference between angle of object and shooting angle
		  \param[in] delta delta of angle
	   */
	  void setAngleDifference(double delta);
	  /*! Enables a gun
	   */
	  void enable();
	  /*! Disables a gun
	   */
	  void disable();
	  /*! Sets a game object
		  \param[in] o game object
	   */
	  void setObject(GameObject * o);
	  virtual ~AbstractAutomaticGun();
};

template<
	typename _Bullet
>
class AutomaticGun
{
 protected:
	 /*! Shoots with specified bullet
	  */
	 virtual void shoot()
	 {
		 _Bullet * bullet = new _Bullet();
		 m_object->game()->addObject(b);
		 p2d::Point p = m_object->position();
		 double angle = m_object->angle() + m_dangle;
		 bullet->setPosition(p);
		 bullet->setAngularVelocity(1.0);
		 double speed = GameObjectConstants<_Bullet>::velocity();
		 bullet->setHorizontalSpeed(speed * cos(angle));
		 bullet->setVeticalSpeed(speed * sin(angle));
	 }
 public:
	 AutomaticGun() : AbstractAutomaticGun()
	 {
		 m_interval = GameObjectConstants<_Bullet>::interval();
	 }
};
