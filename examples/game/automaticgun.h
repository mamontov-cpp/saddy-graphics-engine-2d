/*! \file automaticgun.h
	\author HiddenSeeker

	Describes an concrete automatic gun, that shoots to enemy or specified 
	direction by emitting some bullet with specified speed
 */
#include <p2d/app/constants.h>
#include "abstractautomaticgun.h"
#include "gameobject.h"
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
		  this->p2d::app::AbstractObjectEmitter::perform();
		 }
	 }
 public:
	 /*! Sets interval from game constants
	  */
	 AutomaticGun() : AbstractAutomaticGun()
	 {
		 setInterval(p2d::app::Constants<_Bullet>::interval());
	 }

	 /*! Produces an object
		 \return onject
	  */
	 virtual p2d::app::Object * produce()
	 {
		 return new _Bullet();
	 }
	 /*! Returns angular velocity
		 \return angular velocity
	  */
	 virtual double angularVelocity()
	 {
		 return 1.0;
	 }
	
	 /*! Returns a position
		 \param[out] a position
	  */
	 virtual void position(p2d::Point & p)
	 {
		 p = m_object->position();
	 }
	 /*! Sets a tangential velocity
		 \param[out] v velocity
	  */
	 virtual void tangentialVelocity(p2d::Vector & v)
	 {
		 double angle = m_object->angle() + m_dangle;
		 double speed = p2d::app::Constants<_Bullet>::velocity();
		 v.setX(speed * cos(angle));
		 v.setY(speed * sin(angle));
	 }
};
