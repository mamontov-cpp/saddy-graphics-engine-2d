/*! \file body.h
	\author HiddenSeeker

    Describes a body object
 */
#include "../primitives/object.h"
#include "../templates/hstring.h"
#include "collisionshape.h"
#include "force.h"
#include "ghostoptions.h"
#include "angularforce.h"
#include "movement.h"
#pragma once

/*! A special point, which can be added to time or other values to make objects
	not collide
 */
#define COLLISION_PRECISION 1.0E-4

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
	/*! A tangential movement for body
	 */
	p2d::TangentialMovement * m_tangential;
	/*! A angular movement for body
	 */
	p2d::AngularMovement * m_angular;
	/*! A current shape of data
	 */
	CollisionShape * m_current;
	/*! A temporary shape for returning shape at specific time.
		Selected and freed here.
	 */
	CollisionShape * m_temporary;
	/*! A type size of shape in body
	 */
	size_t  m_shapesize;
public:
	/*! Construct new body line with zero width at (0,0) with zero speed
	 */
	Body();
	/*! Returns current shape for a body
	 */
	p2d::CollisionShape * currentShape();
	/*! Sets user object for a body
	 */
	virtual void setUserObject(sad::Object * o);
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
	void notifyRotate(const double & delta);
	/*! Notifies body, that item is moved
		\param[in] delta difference between point
	 */
	void notifyMove(const p2d::Vector & delta);
	/*! Steps values, that must be changed at end of time step, like
		a ghost options and force
		\param[in] time specified time
	 */
	void stepDiscreteChangingValues(double time);
	/*! Steps positions and values at specified time
		\param[in] time specified time 
	 */
	void stepPositionsAndVelocities(double time);
	/*! Adds listener for body movement
		\param[in] listener a common listener 
	 */
	inline void addMoveListener(p2d::TangentialMovement::listener_t listener)
	{
		m_tangential->addListener(listener);
	}
	/*! Removes listener for body movement
		\param[in] listener a common listener 
	 */
	inline void removeMoveListener(p2d::TangentialMovement::listener_t listener)
	{
		m_tangential->removeListener(listener);
	}
	/*! Adds listener for body rotation
		\param[in] listener a common listener
	 */
	inline void addRotateListener(p2d::AngularMovement::listener_t listener)
	{
		m_angular->addListener(listener);
	}
	/*! Removes listener for body rotation
		\param[in] listener a common listener
	 */
	inline void removeRotateListener(p2d::AngularMovement::listener_t listener)
	{
		m_angular->removeListener(listener);
	}
	/*! Tries to set transformer for current shape of body
	 */
	void trySetTransformer();
	/*! Sets new weight for body
		\param[in] weight new weight
	 */
	void setWeight(p2d::Weight * weight);
	/*! Sets new weight for body
		\param[in] weight new weight
	 */
	void setWeight(const p2d::Weight & weight);
	/*! Returns current weight for body
		\return  weight
	 */
	const p2d::Weight & weight() const;
	/*! Returns current weight for body
		\return  weight
	 */
	p2d::Weight & weight();
	/*! Sets current ghost options
		\param[in] ghost ghost options
	 */
	void setCurrentGO(p2d::GhostOptions * ghost);
	/*! Schedule changing ghost options on next iteration
		\param[in] next a new ghost options
	 */
	void sheduleGO(p2d::GhostOptions * next);
	/*! Tests whether body is ghost
		\return whether body is ghost
	 */
	bool isGhost() const;
	/*! Sets new world for body
		\param[in] world world
	 */
	void setWorld(p2d::World * world);
	/*! Returns a world for body
		\return world
	 */
	p2d::World * world();
	/*! Sets new shape for a body. Shape must have center at (0,0)
		and rotated by zero angle. It will move automatically to current 
		points and rotate by specified angle
		\param[in] shape new shape
	 */
	void setShape(p2d::CollisionShape * shape);
	virtual ~Body();

	typedef p2d::MovementDeltaListener<p2d::Body, p2d::Vector> move_t;
	typedef p2d::MovementDeltaListener<p2d::Body, double> rotate_t;
	/*! Sets current position for object
		\param[in] p point
	 */
	void setCurrentPosition(const p2d::Point & p);
	/*! Sets next position for object
		\param[in] p point
	 */
	void shedulePosition(const p2d::Point & p);
	/*! Shedules specific position at specified time
		\param[in] p point
		\param[in] time time when position should change
     */
	void shedulePositionAt(const p2d::Point & p, double time);
	/*! Returns current position of body
		\return current position of body
	 */
	const p2d::Vector & position() const;
	/*! Determines, whether body will teleport at end of current time step
		\return whether position will change
	 */
	bool willPositionChange() const;
	/*! Returns next position, where body will be teleported
		\return next position
	 */
	p2d::Vector nextPosition() const;
	/*! Sets current tangential velocity
		\param[in] v velocity
	 */
	void setCurrentTangentialVelocity(const p2d::Vector & v);
	/*! Shedules new velocity
		\param[in] v velocity
	 */
	void sheduleTangentialVelocity(const p2d::Vector & v);
	/*! Shedules new velocity at specified time
		\param[in] v velocity
		\param[in] time time when velocity should be applied
	 */
	void sheduleTangentialVelocityAt(const p2d::Vector & v, double time);
	/*! A tangential velocity
		\return tangential velocity
	 */
	const p2d::Vector & tangentialVelocity() const;
	/*! Determines, whether body will leap to other velocity at end of current time step
		\return whether position will change
	 */
	bool willTangentialVelocityChange() const;
	/*! Returns next velocity, where body will change speed due to user call
		\return next position
	 */
	p2d::Vector nextTangentialVelocity() const;
	/*! Sets current angle for object
		\param[in] angle new angle
	 */
	void setCurrentAngle(double angle);
	/*! Sets next angle for object
		\param[in] angle specified angle
	 */
	void sheduleAngle(double angle);
	/*! Shedules angle for object at specified time
		\param[in] angle specified angle
		\param[in] time  a specified time
	 */
	void sheduleAngleAt(double angle, double time);
	/*! Returns current angle of body
		\return current angle of body
	 */
	double angle() const;
	/*! Determines, whether body will change angle at end of current time step
		\return whether position will change
	 */
	bool willAngleChange() const;
	/*! Returns next angle, which body will be set , due to user call of shedule
		\return next angle
	 */
	double nextAngle() const;
	/*! Sets current tangential velocity
		\param[in] v velocity
	 */
	void setCurrentAngularVelocity(double v);
	/*! Shedules new angular velocity
		\param[in] v velocity
	 */
	void sheduleAngularVelocity(double v);
	/*! Shedules new angular velocity at specified time
		\param[in] v velocity
		\param[in] time time of specified velocity
	 */
	void sheduleAngularVelocityAt(double v, double time);
	/*! A angular velocity
		\return tangential velocity
	 */
	double angularVelocity() const;
	/*! A angular velocity
		\return tangential velocity
	 */
	double angularVelocityAt(double time) const;
	/*! Determines, whether body will leap to other velocity at end of current time step
		\return whether position will change
	 */
	bool willAngularVelocityChange() const;
	/*! Returns next velocity, where body will change speed due to user call
		\return next position
	 */
	double nextAngularVelocity() const;
	/*! Moves body by specified vector
		\param[in] v vector
	 */
	void move(const p2d::Vector & v);
	/*! Rotates body by specified angle
	 */
	void rotate(double delta);
	/*! Clears all of listeners for body
	 */
	void clearListeners();
	/*! Returns a tangential forces, acting on body
	 */
	p2d::TangentialActingForces & tangentialForces();
	/*! Returns an angular forces, acting on body
	 */
	p2d::AngularActingForces  & angularForces();
	/*! Returns an average velocity
		\return average velocity
	 */
	p2d::Vector averageChangeIndependentTangentialVelocity(); 
	/*! Returns a tangential velocity at specified time
		\param[in] time a time
		\return velocity
	 */
	p2d::Vector tangentialVelocityAt(double time);
	/*! Builds an acceleration cache for any of bodies
	 */
	void buildAccelerationCache();
	/*! Returns  time step for body
		\return time step
	 */
	double timeStep() const;
	/*! If next position is scheduled, places object between two positions,
		otherwise schedules new position. Note, that instead a position, a distance
		between current position and new is passed
		\param[in] distance
	 */
	void correctPosition(const p2d::Vector & distance);
	/*! If next velocity is scheduled, computes middle of two velocities, otherwise
		schedules new velocity. 
		\param[in] v new velocity
	 */
	void correctTangentialVelocity(const p2d::Vector & v);
};

}

