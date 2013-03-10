/*! \file selectedstate.h
	\author HiddenSeeker

	Describes an selected state of editor
 */
#include "../../editorcore/editorbehaviourstate.h"
#include <time.h>
#include <vector>
#include <templates/hstring.h>
#include <primitives/hpoint.h>
#pragma once

// Screen Object
class AbstractScreenObject;

// Amount of seconds to cooldown navigation
#define SSSS_NAVIGATION_COOLDOWN 1.5

// A substates of idle states
enum SelectedStateSubState
{
	// A simple state, when we are selected one object and working with it
	SSSS_SIMPLESELECTED,
	// A state, when we are on bunch of objects
	SSSS_SELECTEDNAVIGATION
};
// A simple substate for movement of object
enum SelectedStateMovementSubState
{
	SSMSS_NOMOVEMENT,
	SSMSS_MOVING
};


class SelectedState: public EditorBehaviourState
{
protected:
	// A substate for moving object
	SelectedStateMovementSubState m_movement_substate;
	hPointF m_picked_point;  //!< A first picked user point for moving
	hPointF m_picked_old_center;	 //!< An old objects center, used for saving command 

	// A substate for navigating through selected object set
	SelectedStateSubState m_substate;
	// When entered navigation
	clock_t m_navigationstart;
	// Navigation chain information (contains only UIDs of elements)
	// UIDs are saved, because we can remove elements and chain will be broken
	std::vector<hst::string> m_chain;
	// Navigation chain position
	int m_navposition;
	/** Navigates next object in chain
		\param[in] next next object
	 */
	void navigateOffset(bool next);
public:
	/** Action, performed when entering state
	 */
	virtual void enter();
	/** Action, performed when leaving state
	 */
	virtual void leave();
	/** Tries to select some item
		 \param[in] ev  event  data
	 */
	virtual void onMouseDown(const sad::Event & ev);
	/** Constructs new selected state
	 */
	SelectedState();
	/** Enters navigation substate
		\param[in] chain element UID chain
     */
	void enterNavigation(const std::vector<hst::string> & chain);
	/** Handles wheel  movement
		 \param[in] ev event data
	 */
	virtual void onWheel(const sad::Event & ev);

	/** Moves object if in moving object state
		\param[in] ev event data
	 */
	virtual void onMouseMove(const sad::Event & ev);

	/** Commits a mouse up event
		\param[in] ev  even  data
	 */
	 virtual void onMouseUp(const sad::Event & ev);
};
