/*! \file force.h
	\author HiddenSeeker

	Describes a force, acting on object
 */
#pragma once
#include <p2d/vector.h>
#include "../templates/maybe.hpp"

namespace p2d
{

/*! Describes force, acting on an object
 */
class Force
{
protected:
	bool m_persistent;
	int m_ticks;
	p2d::Vector m_value;
public:
	/*! Computes new force
		\param[in] value value of force
		\param[in] persistent whether force is persistent
		\param[in] ticks whether it runs for specified amount of time
	 */
	Force(const p2d::Vector &  value = p2d::Vector(0, 0), 
		  bool persistent = false,
		  int ticks  = 0
		 );
	/*! Returns force for next step for iteration.
		Callled when next iteration is called in p2d::Body's step
		\return force for next iteration
	 */
	virtual p2d::Force * step() const;
	/*! Returns a force value
	 */
	virtual p2d::Vector value() const;	
	/*! Sets a value for a vector
		\param[in] v vector
	 */
	void setValue(const p2d::Vector & v);
	/*! Makes a force persistent
	 */
	void  makePersistent();
	/*! Makes force an impulse for specified amount of time lapses
		\param[in] ticks a tickss
	 */
	void  makeImpulseFor(int ticks);
	/*! Disables a force
	 */
	virtual void disable();
	/*! Returns a zero force
		\return a zero-value force
	 */
	static Force zero();
	/*! Returns a persistent force
		\param[in] v value of force
		\return a force object
	 */
	static Force persistent(const p2d::Vector & v);
	/*! An impulse is force that acts for first timelapse
		\param[in] v value of force
		\return a force object
	 */
	static Force impulse(const p2d::Vector & v);
};

/*! Determines a force as a state for a body
 */
class ForceState
{
	private:
		p2d::Force  * m_force;		 //!< Inner force value
		hst::Maybe<p2d::Force *> m_next_value; //!< Next force state, if user changed value. Contained by value
	public:
		/*! Creates new force state with no force
		 */
		ForceState();
		/*! Destroys inner values (next value is also freed).
		 */
		~ForceState();
		/*! Sets current force. Will affect cureent iteration as well, so
			consider using push if you want to apply force at next step
			\param[in] force new current force
		 */
		void setCurrent(p2d::Force * force);
		/*! Sets next force. Consider this function if you want force to be
			applied at next iteration
			\param[in] force a new force
		 */
		void push(p2d::Force * force);
		/*! Performs next iteration on force stet, moving to next time step
		 */
		void step();
		/*! Determines, whether user has changed a force state by calling
		    push()
			\return whether state changed
		 */
		bool userChanged() const;
		/*! Returns a user value for force for next iteration, NULL if not set
			\return next value for force
		 */
		p2d::Force * nextForce();
		/*! Returns a current value for force
			\return a value of force
		 */
		p2d::Vector value() const;
};

}
