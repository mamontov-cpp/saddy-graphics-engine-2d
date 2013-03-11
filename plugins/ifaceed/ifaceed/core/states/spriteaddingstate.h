/*! \file spriteaddingstate.h
	\author HiddenSeeker

	Describes a state for sprite adding
 */
#include "../../editorcore/editorbehaviourstate.h"
#include <time.h>
#include <vector>
#include <templates/hstring.h>
#pragma once


/** A simple sprite adding state, when one click places a sprite
 */
class SimpleSpriteAddingState: public EditorBehaviourState
{
public:
	/** Highlights entering a state
	 */
	virtual void enter();
	/*! Moves a sprite
		\param[in] ev event
	  */
	virtual void onMouseMove(const sad::Event & ev);
	/*! Handles wheel  movement
		 \param[in] ev event data
	 */
	virtual void onWheel(const sad::Event & ev);
	/** Tries to select some item and enter selected state
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::Event & ev);
	/** Leveas a state
	 */
	virtual void leave();
	/*! Handles key down event
		\param[in] ev event data
	 */
	virtual void onKeyDown(const sad::Event & ev);
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
class DiagonalSpriteAddingState: public EditorBehaviourState
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
	virtual void onMouseDown(const sad::Event & ev);
	/*! Moves a sprite
		\param[in] ev event
	  */
	virtual void onMouseMove(const sad::Event & ev);
	/** Leveas a state
	 */
	virtual void leave();
	/*! Handles key down event
		\param[in] ev event data
	 */
	virtual void onKeyDown(const sad::Event & ev);
};

