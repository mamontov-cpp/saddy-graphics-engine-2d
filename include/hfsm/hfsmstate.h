/*! \file   hfsmstate.h
	\author HiddenSeeker

	A state for hierarchical finite state machine
 */
#pragma once
#include "../sadhash.h"
#include "../sadstring.h"
#include "hfsmhandler.h"
#include "hfsmshared.h"

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
	virtual void handleEnterWith(sad::hfsm::AbstractHandler * h);
	/*! Sets handler for leaving state
		\param[in] h handler
	 */
	virtual void handleLeaveWith(sad::hfsm::AbstractHandler * h);
	/*! Sets handler for entering state as function call
		\param[in] h callable handler
	 */
	template<typename _Callable>
	void setEnterHandler(_Callable h)
	{
		handleEnterWith(new sad::hfsm::Function<_Callable>(h));
	}
	/*! Sets handler for entering state as call of method to object
		\param[in] o object
		\param[in] p pointer to method
	 */
	template<typename _Object, typename _MethodPointer>
	void setEnterHandler(_Object o, _MethodPointer p)
	{
		handleEnterWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
	}
	/*! Sets handler for entering state as call of sequential calls of method to object
		\param[in] o object
		\param[in] f first pointer to method
		\param[in] g second pointer to method
	 */
	template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
	void setEnterHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
	{
		handleEnterWith(new sad::hfsm::MethodComposition<_Object, _MethodFPointer, _MethodGPointer>(o, f, g));
	}
	/*! Sets handler for leaving state as function call
		\param[in] h callable handler
	 */
	template<typename _Callable>
	void setLeaveHandler(_Callable h)
	{
		handleLeaveWith(new sad::hfsm::Function<_Callable>(h));
	}
	/*! Sets handler for leaving state as call of method to object
		\param[in] o object
		\param[in] p pointer to method
	 */
	template<typename _Object, typename _MethodPointer>
	void setLeaveHandler(_Object o, _MethodPointer p)
	{
		handleLeaveWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
	}
	/*! Sets handler for leaving state as call of sequential calls of method to object
		\param[in] o object
		\param[in] f first pointer to method
		\param[in] g second pointer to method
	 */
	template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
	void setLeaveHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
	{
		handleLeaveWith(new sad::hfsm::MethodComposition<_Object, _MethodFPointer, _MethodGPointer>(o, f, g));
	}
	/*! Enters a state
	 */
	virtual void enter();
	/*! Leaves a state
	 */
	virtual void leave();
	/*! Sets a shared data for a state
		\param[in] shahred a shared data
	 */
	virtual void setShared(sad::hfsm::Shared * shared);
	/*! Returns a shared data from a state
		\return shared data from a state
	 */
	virtual sad::hfsm::Shared * shared() const;
	/*! Sets parent state for state
		\param[in] state a parent state
	 */
	virtual void setParent(sad::hfsm::State * state);
	/*! Returns parent state for state (NULL if it's top-level state)
		\return parent state
	 */
	virtual sad::hfsm::State * parent() const;
private:
	/*! A handler to call, when entering a state
	 */
	sad::hfsm::AbstractHandler * m_enter;
	/*! A handler to call, when leaving a state
	 */
	sad::hfsm::AbstractHandler * m_leave;
	/*! A shared data for a state
	 */
	sad::hfsm::Shared  * m_shared;
	/*! A parent state
	 */
	sad::hfsm::State  *  m_parent;
	/*! A children states in machine
	 */
	sad::hfsm::StateMap m_children;
	/*! A machine, where state belongs to
	 */
	sad::hfsm::Machine * m_machine;
};

}

}
