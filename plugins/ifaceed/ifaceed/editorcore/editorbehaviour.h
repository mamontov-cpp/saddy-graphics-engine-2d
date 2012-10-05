/** \file editorbehaviour.h
	\author HiddenSeeker
	
	Describes editor behaviour, as amount of states with one state active, that handles event
 */
#include "input.h"
#include "log.h"
#include "templates/hhash.hpp"
#include "templates/hstring.h"
#pragma once

class Editor;
class EditorBehaviourState;

/*! Describes editor behaviour, as amount of states with one state active, that handles event
 */
class EditorBehaviour
{
 protected:
	 hst::hash<hst::string, EditorBehaviourState *> m_states; //!< All states in behaviour
	 hst::string m_active_state; //!< Current active state (empty if nothing active)
	 hst::string m_initial_state; //!< State, that will be choosed on initial start
	 Editor * m_parent; //!< Parent editor
 public:
	 /** Constructs default empty behaviour
		 \param[in] ed editor
		 \param[in] initial initial state
      */
	 EditorBehaviour(Editor * ed, const hst::string & initial);
	 /** Sets an editor
		 \param[in] editor
	  */
	 inline void setParent(Editor* ed)
	 {
		 m_parent = ed;
	 }
	 /** Returns an editor
		 \return editor
	  */
	 inline Editor * parent()
	 {
		 return m_parent;
	 }
	 /** Inserts new state on editor. Writes in log an error, if exists
		 \param[in] statename editor state name
		 \param[in] state     editor state
	  */
	 void addState(const hst::string & statename, EditorBehaviourState * state);
	 /** Removes state from editor	
		 \param[in] statename
	  */
	 void removeState(const hst::string & statename);
	 /** Destroys all of states
	  */
	 ~EditorBehaviour();
};