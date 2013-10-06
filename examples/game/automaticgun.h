/*! \file automaticgun.h
	\author HiddenSeeker

	Describes an concrete automatic gun, that shoots to enemy or specified 
	direction by emitting some bullet with specified speed
 */
#pragma once

#include "abstractautomaticgun.h"
#include "gameobject.h"

#include <p2d/app/constants.h>

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
		  this->sad::p2d::app::AbstractObjectEmitter::perform();
		 }
	 }
 public:
	 /*! Sets interval from game constants
	  */
	 AutomaticGun() : AbstractAutomaticGun()
	 {
		 setInterval(sad::p2d::app::Constants<_Bullet>::interval());
	 }

	 /*! Produces an object
		 \return onject
	  */
	 virtual sad::p2d::app::Object * produce()
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
		 \param[out] p position
	  */
	 virtual void position(sad::p2d::Point & p)
	 {
		 p = m_object->position();
	 }
	 /*! Sets a tangential velocity
		 \param[out] v velocity
	  */
	 virtual void tangentialVelocity(sad::p2d::Vector & v)
	 {
		 double angle = m_object->angle() + m_dangle;
		 double speed = sad::p2d::app::Constants<_Bullet>::velocity();
		 v.setX(speed * cos(angle));
		 v.setY(speed * sin(angle));
	 }
};
