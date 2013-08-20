/*! \file body.h
	\author HiddenSeeker

    Describes a body object
 */
#include "../primitives/object.h"
#include "../templates/hstring.h"
#include "collisionshape.h"
#include "force.h"
#include "ghostoptions.h"
#include "tangentialforce.h"
#include "movement.h"
#pragma once

namespace p2d
{
class World;
/*! Describes a body in physics engine
 */
class Body: public sad::Object
{
SAD_OBJECT
private:
	/*! A weight of specific body
	 */
	p2d::Weight  *   m_weight;
	/*! An options for ghost mode
	 */
	p2d::GhostOptionsFlow * m_ghost;
	/*! A world simulation
	 */
	p2d::World * m_world;
	/*! Returns a user object
		\return user object for a body
	 */
	sad::Object * m_user_object;
	/*! A central movement for body
	 */
	p2d::CentralMovement * m_central;
	/*! A tangential movement for body
	 */
	p2d::TangentialMovement * m_tangential;
	/*! A current shape of data
	 */
	CollisionShape * m_current;
	/*! A temporary shape for returning shape at specific time.
		Selected and freed here.
	 */
	CollisionShape * m_temporary;
protected:
	/*! Returns  time step for body
		\return time step
	 */
	double timeStep() const;
public:
	/*! Returns an inner user-defined object
		\return inner user-defined object
	 */
	virtual sad::Object * userObject() const;
	/*! Returns a type of user-defined object
		\return type of object
	 */
	virtual const hst::string & userType() const;
	/*! Returns a position of body at specified time.
		\param[in] time time, when position is needed
		\return what shape, body had at specified time
	 */
	virtual p2d::CollisionShape & at(double time) const;
	/*! Notifies body, that item is rotated
		\param[in] delta difference between angles 
	 */
	void notifyRotate(double delta);
	/*! Notifies body, that item is moved
		\param[in] delta difference between point
	 */
	void notifyMove(const p2d::Vector & delta);
	/*! Steps values, that must be changed at end of time step, like
		a ghost options and force
	 */
	void stepDiscreteChangingValues();
	/*! Steps positions and values at specified time
		\param[in] time specified time 
	 */
	void stepPositionsAndVelocities(double time);
	/*! Adds listener for body movement
		\param[in] listener a common listener 
	 */
	inline void addMoveListener(p2d::CentralMovement::listener_t listener)
	{
		m_central->addListener(listener);
	}
	/*! Removes listener for body movement
		\param[in] listener a common listener 
	 */
	inline void removeMoveListener(p2d::CentralMovement::listener_t listener)
	{
		m_central->removeListener(listener);
	}
	/*! Adds listener for body rotation
		\param[in] listener a common listener
	 */
	inline void addRotateListener(p2d::TangentialMovement::listener_t listener)
	{
		m_tangential->addListener(listener);
	}
	/*! Removes listener for body rotation
		\param[in] listener a common listener
	 */
	inline void removeRotateListener(p2d::TangentialMovement::listener_t listener)
	{
		m_tangential->removeListener(listener);
	}
	virtual ~Body();
};

}

