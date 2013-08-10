/*! \file force.h
	\author HiddenSeeker

	Describes a force, acting on object
 */
#pragma once
#include <p2d/vector.h>

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
	/*! Returns a force value
	 */
	virtual p2d::Vector value();	
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

}