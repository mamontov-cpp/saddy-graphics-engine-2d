/*! \file labeladdingstate.h
	\author HiddenSeeker

	Describes an label adding state of editor
 */
#include "ifacestate.h"
#pragma once

/*! \class LabelAddingState
	A state, when user added a label
 */
class LabelAddingState: public IFaceState
{
 public:
	 /*! Moves a label
		 \param[in] ev event
	  */
	 virtual void onMouseMove(const sad::input::MouseMoveEvent & ev);
	 /*! Action, performed when entering state
	  */
	 virtual void enter();
	 /*! Action, performed when leaving state
	  */
	 virtual void leave();
	 /*! Handles wheel  movement
		 \param[in] ev event data
	  */
	 virtual void onWheel(const sad::input::MouseWheelEvent & ev);
	 /*! Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseDown(const sad::input::MousePressEvent & ev);
	 /*! Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyDown(const sad::input::KeyPressEvent & ev);
};