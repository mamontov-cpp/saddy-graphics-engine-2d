/*! \file idlestate.h
	\author HiddenSeeker

	Describes an idle state of editor
 */
#include "ifacestate.h"
#include <time.h>
#include <vector>
#include <sadstring.h>
#pragma once


class IdleState: public IFaceState
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
