/*! \file idlestate.h
	\author HiddenSeeker

	Describes an idle state of editor
 */
#pragma once
#include "../state.h"

#include <sadstring.h>

#include <time.h>
#include <vector>


class IdleState: public core::State
{
public:
	/** Highlights entering a state
	 */
	virtual void enter();
	/** Tries to select some item and enter selected state
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::input::MousePressEvent & ev);
};
