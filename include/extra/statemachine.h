/*! \file   statemachine.h
    \author HiddenSeeker

	File, containing a definition of statemachine and global game state
*/
#include "../types.h"
#include <stdlib.h>
#pragma once



/* Class of handler of state change. 
*/
class StateHandler 
{
private:
	bool (*m_operator_inner)(int);
public:
	/*! Function of changing state. Must prepare new state of machine
	*	\param[in] prev_state previous state
	*	\return true on successful state change
	*/
	virtual bool operator()(int prev_state);
	/*! Constructor
	     \param[in] function function, that will be called in operator()
	*/
	StateHandler(bool (*function)(int));
	/*! Destructor
	*/
	virtual ~StateHandler();
};

/*! Class of state machine, that provides binding of states
*/
class StateMachine 
{
protected:
	int m_state;//code of state -1 if not state now, else positive number
	hhash<int, StateHandler*> m_state_list;
	static StateMachine * inst; //!< Instance
	
	static void quit();
	inline static void init() { if (!inst) { inst=new StateMachine(); atexit(StateMachine::quit); }}
	
	StateMachine();
	StateMachine(const StateMachine &);
public:
	/*!	Binds a state handler
	*	\param[in] state state
	*	\param[in] handler handler
	*/
	static void bindState(int state, StateHandler *handler);
	/*!	Setting of state
	*	\param[in] state new state
	*	\return true on success
	*/
	static bool pushState(int state);	
	/*! Returns current state
	*/
	static inline int state()  { return inst->m_state;}
	/*! Destructor
	*/
	~StateMachine();
};
