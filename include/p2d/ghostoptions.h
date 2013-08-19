/*! \file ghostoptions.h
	\author HiddenSeeker

	Describes an options for object, that defines it as a ghost, i.e.
	an object, which does not collide with any other objects
 */
#pragma once
#include "../templates/maybe.hpp"

namespace p2d
{

/*! Class, that describes an options for object to became a ghost, 
	i.e. the object which can not collide with other objects
 */
class GhostOptions
{
protected:
	bool m_persistent;
	int  m_ticks;
public:
	/*! A new options. By default object is not a ghost.
		\param[in] whether ghost is persistent (default - no)
		\param[in] ticks whether ghost is persistent for specified count of ghost (0 is never)
	 */
	GhostOptions(bool persistent = false, int ticks = 0);
	/*! Whether object is a ghost 
		\return object
	 */
	virtual bool ghost() const;
	/*! Creates a copy for options for next iteration, returning new options
		\return new options
	 */
	virtual GhostOptions * step() const;
	/*! Creates a common disabled ghost options
		\return a disabled ghost options
	 */
	static GhostOptions disabled();
	/*! Creates a forever ghost options
		\return a forever ghost options
	 */
	static GhostOptions forever();
	/*! Creates a ghost for one specified time lapse
		\return a ghost options
	 */
	static GhostOptions oneTime();
};



/*! Determines a ghost options as a state for a body
 */
class GhostOptionsState
{
	private:
		p2d::GhostOptions  * m_inner;		 //!< Inner current options
		hst::Maybe<p2d::GhostOptions *> m_next_value; //!< Next options, if user changed value. Contained by value
	public:
		/*! Creates new force state with no force
		 */
		GhostOptionsState();
		/*! Destroys inner values (next value is also freed).
		 */
		~GhostOptionsState();
		/*! Sets current options. Will affect cureent iteration as well, so
			consider using push if you want to apply force at next step
			\param[in] options new current options
		 */
		void setCurrent(p2d::GhostOptions * opts);
		/*! Sets next options. Consider this function if you want force to be
			applied at next iteration
			\param[in] options a new options
		 */
		void push(p2d::GhostOptions * force);
		/*! Performs next iteration on force stet, moving to next time step
		 */
		void step();
		/*! Determines, whether user has changed a state by calling
		    push()
			\return whether state changed
		 */
		bool userChanged() const;
		/*! Returns a user value for options for next iteration, NULL if not set
			\return next value for optionss
		 */
		p2d::GhostOptions * nextOptions();
		/*! Returns a current value for options
			\return a value of options
		 */
		bool value() const;
};

}
