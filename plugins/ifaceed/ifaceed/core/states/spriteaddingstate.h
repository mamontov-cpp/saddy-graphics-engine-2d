/*! \file spriteaddingstate.h
	\author HiddenSeeker

	Describes a state for sprite adding
 */
#include "ifacestate.h"
#include <time.h>
#include <vector>
#include <sadstring.h>
#pragma once


/** A simple sprite adding state, when one click places a sprite
 */
class SimpleSpriteAddingState: public IFaceState
{
public:
	/** Highlights entering a state
	 */
	virtual void enter();
	/*! Moves a sprite
		\param[in] ev event
	  */
	virtual void onMouseMove(const sad::input::MouseMoveEvent & ev);
	/*! Handles wheel  movement
		 \param[in] ev event data
	 */
	virtual void onWheel(const sad::input::MouseWheelEvent & ev);
	/** Tries to select some item and enter selected state
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::input::MousePressEvent & ev);
	/** Leveas a state
	 */
	virtual void leave();
	/*! Handles key down event
		\param[in] ev event data
	 */
	virtual void onKeyDown(const sad::input::KeyPressEvent & ev);
};

enum DiagonalSpriteAddingSubState
{
	DSAS_INITIAL = 0,    //!< Initial state, no clicks were performed
	DSAS_FIRSTCLICK = 1, //!< First click was performed
	// There are no state for second click, since we will left a state to a selected, when second
	// click was performed
};

/** A sprite adding state, when First click places Upper Left corner, 
	This state ignores an angle
	Second places bottom right corner if angle equals zero
 */
class DiagonalSpriteAddingState: public IFaceState
{
protected:
	DiagonalSpriteAddingSubState m_substate;
public:
	/** Highlights entering a state
	 */
	virtual void enter();
	/** Tries to select some item and enter selected state
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::input::MousePressEvent & ev);
	/*! Moves a sprite
		\param[in] ev event
	  */
	virtual void onMouseMove(const sad::input::MouseMoveEvent & ev);
	/** Leveas a state
	 */
	virtual void leave();
	/*! Handles key down event
		\param[in] ev event data
	 */
	virtual void onKeyDown(const sad::input::KeyPressEvent & ev);
};

