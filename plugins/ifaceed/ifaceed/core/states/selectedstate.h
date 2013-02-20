/*! \file selectedstate.h
	\author HiddenSeeker

	Describes an selected state of editor
 */
#include "../../editorcore/editorbehaviourstate.h"
#include <time.h>
#include <vector>
#include <templates/hstring.h>
#pragma once

// Screen Object
class AbstractScreenObject;

// Amount of seconds to cooldown navigation
#define SSSS_NAVIGATION_COOLDOWN 3

// A substates of idle states
enum SelectedStateSubState
{
	// A simple state, when we are selected one object and working with it
	SSSS_SIMPLESELECTED,
	// A state, when we are on bunch of objects
	SSSS_SELECTEDNAVIGATION
};


class SelectedState: public EditorBehaviourState
{
protected:
	// A substate
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
};
