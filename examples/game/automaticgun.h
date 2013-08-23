/*! \file automaticgun.h
	\author HiddenSeeker

	Describes an concrete automatic gun, that shoots to enemy or specified 
	direction by emitting some bullet with specified speed
 */
#include "abstractautomaticgun.h"
#include "gameobject.h"
#include "gameobjectconstants.h"
#pragma once
/*! An automatic gun, which shoots bullets of specified type
 */
template<
	typename _Bullet
>
class AutomaticGun: public AbstractAutomaticGun
{
 protected:
	 /*! Shoots bullets of specified bullet type
	  */
	 virtual void perform()
	 {
		 // Check paused flag
		 if (m_object->game()->isPaused() == false)
		 {
		  _Bullet * bullet = new _Bullet();
		  m_object->game()->addObject(bullet);
		  p2d::Point p = m_object->position();
		  double angle = m_object->angle() + m_dangle;
		  bullet->setPosition(p);
		  bullet->setAngularVelocity(1.0);
		  double speed = GameObjectConstants<_Bullet>::velocity();
		  bullet->setHorizontalSpeed(speed * cos(angle));
		  bullet->setVerticalSpeed(speed * sin(angle));
		 }
	 }
 public:
	 /*! Sets interval from game constants
	  */
	 AutomaticGun() : AbstractAutomaticGun()
	 {
		 setInterval(GameObjectConstants<_Bullet>::interval());
	 }
};
