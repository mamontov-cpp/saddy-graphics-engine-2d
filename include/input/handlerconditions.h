/*! \file   handlerconditions.h
	\author HiddenSeeker

	A conditions for running event handlers is specified here. 
 */
#pragma once
#include "events.h"

namespace sad
{

namespace input
{

class AbstractHanderCondition
{
public:
	/*! Checks a condition against abstract event
		\param[in] e event
		\return whether we should  run an event
	 */
	virtual bool check(const sad::input::AbstractEvent & e) = 0;
	/*! Clones a condition
		\return clone a condition
	 */
	virtual sad::input::AbstractHanderCondition * clone() = 0;
	/*! You can inherit condition for implementing very own conditions
	 */
	virtual ~AbstractHanderCondition();
};


}

}