/*! \file tickable.h
	

	Describes a tickable object which can be stepped
 */
#pragma once
#include "..//maybe.h"

#include <cstdlib>

namespace sad
{

namespace p2d
{

/*! A class for getting default value, for tickable, used also in
	movement. Should return something like zero.
 */
template<
	typename _Value
>
class TickableDefaultValue
{
 public:
	static _Value get();
	/*! Returns a zero for current type
	 */
	static _Value zero();
};

/*! A class for value, that can change through time
 */
template<
	typename _Value
>
class TickableState
{
public:
	/*! Computes new force
		\param[in] value value of force
		\param[in] persistent whether force is persistent
		\param[in] ticks whether it runs for specified amount of time
	 */
	TickableState(const _Value &  value = p2d::TickableDefaultValue<_Value>::get(), 
		  bool persistent = false,
		  int ticks  = 0
		 )
	{
		m_value = value;
		m_persistent = persistent;
		m_ticks = ticks;
	}
	virtual ~TickableState() { }
	/*! Returns value for next step for iteration.
		Callled when next iteration is called in p2d::Body's step
		\return force for next iteration
	 */
	virtual typename p2d::TickableState<_Value> * step() const
	{
		typename p2d::TickableState<_Value> * result = new p2d::TickableState<_Value>();
		if (m_persistent)
		{
			*result = p2d::TickableState<_Value>(m_value, true, 0);
		}
		else
		{
			if (m_ticks > 0)
			{
				*result = p2d::TickableState<_Value>(m_value,  false, m_ticks - 1);
			}
		}
		return result;
	}
	/*! Returns a value
	 */
	virtual _Value value() const { return m_value; }	
	/*! Sets a value
		\param[in] v vector
	 */
	virtual void setValue(const _Value & v) { m_value = v; }
	/*! Makes a value persistent
	 */
	void  makePersistent() 
	{
			m_persistent = true;
			m_ticks = 0;
	}
	/*! Makes  an impulse for specified amount of time lapses
		\param[in] ticks a tickss
	 */
	void  makeImpulseFor(int ticks)
	{
		m_persistent = false;
		m_ticks = ticks;
	}
	/*! Disables a TickableState, switching to default value
	 */
	virtual void disable()
	{
		m_persistent = false;
		m_ticks = 0;
		m_value = p2d::TickableDefaultValue<_Value>::get();
	}
	/*! Returns a a TickableState with default value
		\return a zero-value force
	 */
	static p2d::TickableState<_Value> * zero()
	{
		_Value v = p2d::TickableDefaultValue<_Value>::get();
		return new p2d::TickableState<_Value>(v, false, 0);
	}
	/*! Returns a persistent value
		\param[in] v value of force
		\return a force object
	 */
	static p2d::TickableState<_Value> *  persistent(const _Value & v)
	{
		return new p2d::TickableState<_Value>(v, true);
	}
	/*! An impulse is value that acts for first time step
		\param[in] v value of force
		\return a force object
	 */
	static  p2d::TickableState<_Value> *   impulse(const _Value & v)
	{
		return new p2d::TickableState<_Value>(v, false, 1);
	}
protected:
	bool m_persistent;
	int m_ticks;
	_Value m_value;
};

/*! A simulation flow with specified step
 */
template<
typename _Value
>
class TickableFlow
{
public:
	typedef TickableState<_Value> * state; 
	/*! Creates new force state with no force
		 */
		TickableFlow()
		{
			m_current = TickableState<_Value>::zero();
		}
		/*! Destroys inner values (next value is also freed).
		 */
		~TickableFlow()
		{
			delete m_current;
			if (m_next.exists())
			{
				delete m_next.value();
			}
		}
		/*! Sets current state. Will affect cureent iteration as well, so
			consider using push if you want to apply force at next step
			\param[in] s new state
		 */
		void setCurrent(state s)
		{
			delete m_current;
			m_current = s;
		}
		/*! Sets next state. Consider this function if you want force to be
			applied at next iteration
			\param[in] s a new state
		 */
		void push(state  s)
		{
			if (m_next.exists())
			{
				delete m_next.value();
			}
			m_next.setValue(s);
		}
		/*! Performs next iteration, moving to next time step
		 */
		void step()
		{
			if (m_next.exists())
			{
				delete m_current;
				m_current = m_next.value();
				m_next.clear();
			}
			else
			{
				state  s = m_current->step();
				delete m_current;
				m_current = s;
			}
		}
		/*! Determines, whether user has changed a state by calling
		    push()
			\return whether state changed
		 */
		bool userChanged() const
		{
			return m_next.exists();
		}
		/*! Returns a user value for tickable for next iteration, NULL if not set
			\return next value 
		 */
		state  next()
		{
			state  p = NULL;
			if (m_next.exists())
			{
				p = m_next.value();
			}
			return p;
		}
		/*! Returns a current value 
			\return a value 
		 */
		_Value value() const
		{
			return m_current->value();
		}
 protected:
	 state  m_current; //!< A cuurent state
	 sad::Maybe<state> m_next; //!< A next state
};

}

}
