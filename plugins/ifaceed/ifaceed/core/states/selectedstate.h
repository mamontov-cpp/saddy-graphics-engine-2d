/*! \file selectedstate.h
	\author HiddenSeeker

	Describes an selected state of editor
 */
#include "ifacestate.h"
#include <time.h>
#include <vector>
#include <templates/hstring.h>
#include <primitives/hpoint.h>
#include <extra/geometry2d.h>
#pragma once

// Screen Object
class AbstractScreenObject;

// Amount of seconds to cooldown navigation
#define SSSS_NAVIGATION_COOLDOWN 1.5

/*! Describes an action, which should be taken to perform 
	a resizing of rect, angle dependent
 */
class ResizingStateAction
{
public:
	int p0;
	int p1;
	int p2;
	int p3;

	/*! Creates a new incorrect action
	 */
	inline ResizingStateAction()
	{
	}
	/*! Creates a new action as tuple of values
		\param[in] _1
		\param[in] _2
		\param[in] _3
		\param[in] _4
	 */
	inline ResizingStateAction(int _1, int _2, int _3, int _4)
	{
		p0 = _1;
		p1 = _2;
		p2 = _3;
		p3 = _4;
	}
	/*! Applies an action, resizing rectangle
		\param[in] x rectangle
		\param[in] xd distance
		\returns new rectangle
	 */
	hRectF apply(const hRectF & x, const hPointF & xd);
};

class ResizingSubState
{
 public:
		hRectF oldRect;
		hPointF oldPoint;
		ResizingStateAction action;
};


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
	SSMSS_MOVING,
	SSMSS_RESIZING
};


class SelectedState: public IFaceState
{
protected:
	// A substate for moving object
	SelectedStateMovementSubState m_movement_substate;
	hPointF m_picked_point;  //!< A first picked user point for moving
	hPointF m_picked_old_center;	 //!< An old objects center, used for saving command 

	// A state data
	ResizingSubState    m_resizingsubstate;
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
	 /*! Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyDown(const sad::Event & ev);
};
