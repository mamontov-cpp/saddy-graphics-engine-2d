/*! \file labeladdingstate.h
	\author HiddenSeeker

	Describes an label adding state of editor
 */
#include "../../editorcore/editorbehaviourstate.h"
#pragma once

/*! \class LabelAddingState
	A state, when user added a label
 */
class LabelAddingState: public EditorBehaviourState
{
 public:
	 /*! Moves a label
		 \param[in] ev event
	  */
	 virtual void onMouseMove(const sad::Event & ev);
	 /*! Action, performed when entering state
	  */
	 virtual void enter();
	 /*! Action, performed when leaving state
	  */
	 virtual void leave();
	 /** Handles wheel  movement
		 \param[in] ev event data
	  */
	 virtual void onWheel(const sad::Event & ev);
	 /** Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseDown(const sad::Event & ev);
};