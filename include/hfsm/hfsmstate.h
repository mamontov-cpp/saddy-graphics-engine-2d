/*! \file   hfsmstate.h
	\author HiddenSeeker

	A state for hierarchical finite state machine
 */
#pragma once
#include "../sadhash.h"
#include "../sadstring.h"
#include "hfsmhandler.h"

namespace sad
{

namespace hfsm
{

class Machine;
class State;

typedef sad::Hash<sad::String, sad::hfsm::State *> StateMap;

/*! A state for finite state machine
 */
class State
{
public:
	/*! Constructs default state
	 */
	State();
	/*! Erases all of children states, freeing their memory
	 */
	virtual ~State();
	/*! Sets machine for state
		\param[in] machine
	 */
	void setMachine(sad::hfsm::Machine * machine);
	/*! Returns hfsm machine for state
		\return machine
	 */
	sad::hfsm::Machine * machine() const;
	/*! Adds new children state. Deletes old state, if state with such name already exists
		\param[in] name name for a state
		\param[in] state  a state (NULL to create default sad::hfsm::State)
	 */
	void addChild(const sad::String & name, sad::hfsm::State * state = NULL);
	/*! Returns child state by it's name
		\return state data or NULL if not found
	 */
	sad::hfsm::State * child(const sad::String & name) const;
	/*! Returns a children for state map
		\return children states
	 */
	const sad::hfsm::StateMap & children() const;
	/*! Removes child state from parent state. Nothing is done, if
		state is absent. Also it will destroy state as object
		\param[in] name name of child state
	 */
	void removeChild(const sad::String & name);
	/*! Sets handler for entering state
		\param[in] h handler
	 */
	virtual void setEnterHandler(sad::hfsm::AbstractHandler * h);
	/*! Sets handler for leaving state
		\param[in] h handler
	 */
	virtual void setLeaveHandler(sad::hfsm::AbstractHandler * h);
	/*! Enters a state
	 */
	virtual void enter();
	/*! Leaves a state
	 */
	virtual void leave();
private:
	/*! A handler to call, when entering a state
	 */
	sad::hfsm::AbstractHandler * m_enter;
	/*! A handler to call, when leaving a state
	 */
	sad::hfsm::AbstractHandler * m_leave;
	/*! A children states in machine
	 */
	sad::hfsm::StateMap m_children;
	/*! A machine, where state belongs to
	 */
	sad::hfsm::Machine * m_machine;
};

}

}
