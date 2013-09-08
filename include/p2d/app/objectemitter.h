/*! \file objectemitter.h
	\author HiddenSeeker

	Describes a simple emitter for objects, which emits objects
	in specified direction
 */
#include "object.h"
#include "app.h"
#include "../../extra/periodicalevent.h"
#pragma once

namespace p2d
{

namespace app
{


/*! A basic object emitter
 */
class AbstractObjectEmitter: public PeriodicalEvent
{
 protected:
		p2d::app::App * m_app;
 public:
		/*! Sets application for emitter
			\param[in] app application
		 */
		inline AbstractObjectEmitter(p2d::app::App * app = NULL) : m_app(app) {}
		/*! An emitter does not own application
		 */
		virtual ~AbstractObjectEmitter() {}
		/*! Sets an application
			\param[in] app application
		 */
		inline void setApp(p2d::app::App * app) { m_app = app; }
		/*! Returns an application
			\return application
		 */
		virtual p2d::app::App * app() { return m_app; }
		/*! Returns angular velocity
			\return angular velocity
		 */
		virtual double angularVelocity() = 0;
		/*! Sets a tangential velocity
			\param[out] v velocity
		 */
		virtual void tangentialVelocity(p2d::Vector & v) = 0;
		/*! Returns a position
			\param[out] a position
		 */
		virtual void position(p2d::Point & p) = 0;
		/*! Produces an object
			\return onject
		 */
		virtual p2d::app::Object * produce() = 0;
		/*! Emits object
		 */
		virtual void perform();
};

}

}

