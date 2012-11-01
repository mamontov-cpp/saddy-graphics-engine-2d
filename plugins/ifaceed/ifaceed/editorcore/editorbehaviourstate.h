/** \file editorbehaviourstate
	\author HiddenSeeker

	Describes an editor behaviour state data
 */
#include "input.h"
#include "log.h"
#pragma once

class EditorBehaviour;


/** Describes an editor behaviour state, used to handle some events
 */
class EditorBehaviourState
{
 protected:
	 /** Describes a behaviour for data
	  */
	 EditorBehaviour * m_behaviour;
 public:
	 EditorBehaviourState();
	 /** Sets a behaviour for state
	  */
	 inline void setBehaviour(EditorBehaviour * behaviour)
	 {
		 m_behaviour = behaviour;
	 }
	 /** Returns a behaviour
	  */
	 inline EditorBehaviour * behaviour()
	 {
		 return m_behaviour;
	 }
	 /** Action, performed when entering state
	  */
	 virtual void enter();
	 /** Action, performed when leaving state
	  */
	 virtual void leave();
	 /** Handles mouse movement
		 \param[in] ev event data
	  */
	 virtual void onMouseMove(const sad::Event & ev);
	 /** Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseDown(const sad::Event & ev);
	 /** Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseUp(const sad::Event & ev);
	 /** Handles wheel  movement
		 \param[in] ev event data
	  */
	 virtual void onWheel(const sad::Event & ev);
	

	 /** Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyDown(const sad::Event & ev);
	 /** Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyUp(const sad::Event & ev);
	 /** Does nothing
	  */
	 virtual ~EditorBehaviourState();

	
};