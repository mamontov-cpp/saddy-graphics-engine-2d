/*! \file ghostoptions.h
	\author HiddenSeeker

	Describes a weight of object
 */
#pragma once

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
	virtual bool ghost();
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

}

