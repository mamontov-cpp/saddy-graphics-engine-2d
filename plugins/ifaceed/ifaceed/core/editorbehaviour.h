/** \file editorbehaviour.h
	\author HiddenSeeker
	
	Describes editor behaviour, as amount of states with one state active, that handles event
 */
#include <input/events.h>
#include <sadhash.h>
#include <sadstring.h>
#include <log/log.h>
#pragma once

class IFaceEditor;

namespace core
{

class EditorBehaviourState;

/*! Describes editor behaviour, as amount of states with one state active, that handles event
 */
class EditorBehaviour
{
 protected:
	 sad::Hash<sad::String, core::EditorBehaviourState *> m_states; //!< All states in behaviour
	 sad::String m_previous_state; //!< Current previous state
	 sad::String m_active_state; //!< Current active state (empty if nothing active)
	 sad::String m_initial_state; //!< State, that will be choosed on initial start
	 IFaceEditor * m_parent; //!< Parent editor
 public:
	 /** Constructs default empty behaviour
		 \param[in] ed editor
		 \param[in] initial initial state
      */
	 EditorBehaviour(IFaceEditor* ed, const sad::String& initial);
	 /** Sets an editor
		 \param[in] ed a linked editor to behaviour
	  */
	 inline void setParent(IFaceEditor* ed)
	 {
		 m_parent = ed;
	 }
	 /** Returns an editor
		 \return editor
	  */
	 inline IFaceEditor * parent()
	 {
		 return m_parent;
	 }
	 /** Activates editor behaviour
	  */
	 void activate();
	 /** Deactivates a behaviour
	  */
	 void deactivate();
	 /** Switch to previous state with calling leave and enter new state
	  */
	 void cancelState();
	 /** Enters a state data
		 \param[in] state a state data
	  */
	 void enterState(const sad::String & state);
	 /** Returns a state data
		 \return current state of behaviour
	  */
	 inline const sad::String & state() { return m_active_state; }
	 /** Inserts new state on editor. Writes in log an error, if exists
		 \param[in] statename editor state name
		 \param[in] state     editor state
	  */
	 void addState(const sad::String & statename, EditorBehaviourState * state);
	 /** Removes state from editor	
		 \param[in] statename
	  */
	 void removeState(const sad::String & statename);
	 /** Returns a state from behaviour
		 \param[in] statename of state
		 \return state
	  */
	 EditorBehaviourState * getState(const sad::String & statename) const;
	/** Handles mouse movement
		 \param[in] ev event data
	  */
	 virtual void onMouseMove(const sad::input::MouseMoveEvent & ev);
	 /** Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseDown(const sad::input::MousePressEvent & ev);
	 /** Handles mouse down event
		 \param[in] ev  even  data
	  */
	 virtual void onMouseUp(const sad::input::MouseReleaseEvent & ev);
	 /** Handles wheel up movement
		 \param[in] ev event data
	  */
	 virtual void onWheel(const sad::input::MouseWheelEvent & ev);


	 /** Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyDown(const sad::input::KeyPressEvent & ev);
	 /** Handles key down event
		 \param[in] ev event data
	  */
	 virtual void onKeyUp(const sad::input::KeyReleaseEvent & ev);
	 /** Destroys all of states
	  */
    virtual  ~EditorBehaviour();
};

}
