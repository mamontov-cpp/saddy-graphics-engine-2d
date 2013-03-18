/*! \file idlestate.h
	\author HiddenSeeker

	Describes an idle state of editor
 */
#include "../../editorcore/editorbehaviourstate.h"
#include <time.h>
#include <vector>
#include <templates/hstring.h>
#pragma once


class IdleState: public EditorBehaviourState
{
public:
	/** Highlights entering a state
	 */
	virtual void enter();
	/** Tries to select some item and enter selected state
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::Event & ev);
};
