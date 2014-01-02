/*! \file   statemachine.h
    \author HiddenSeeker

	File, containing a definition of statemachine and global game state
*/
#pragma once

#include "../sadhash.h"
#include "../input/handlerconditions.h"

#include <stdlib.h>


namespace sad
{

namespace fsm
{

/*! Defines a shared information between any states
 */
class Shared
{
public:
	 template<typename T> inline T & as() { return static_cast<T&>(*this); }
	 template<typename T> inline const T & as() const{ return static_cast<const T&>(*this); }
	 virtual ~Shared();
};

class Machine;
/*! Defines a callback, which can be invoked in state
 */
class AbstractCallback
{
 public:
	/*! Clones a callback
	 */
	 virtual AbstractCallback * clone()=0;
	 /*! Invocation method, used when StateMachine invokes a callback
	  */
	 virtual void invoke()=0;
	 /*! Destructor
	  */
	 virtual ~AbstractCallback();
};
/*! Defines a free function invocation method denoted with type  void (*)()
 */
class FreeFunction: public AbstractCallback
{
public:
	 inline FreeFunction(void (*function)()) : m_function(function) {}
 	/*! Clones a callback
	 */
	 virtual AbstractCallback * clone();
	 /*! Invocation method, used when StateMachine invokes a callback
	  */
	 virtual void invoke();
private:
	 void (*m_function)(); //!< A called callback
};

/*! Defines a class method function invocation as void(*)()
 */
template<
typename _Class
>
class MethodCallback: public AbstractCallback
{
public:
	 inline MethodCallback(_Class * o, void (_Class::*function)() ) : m_o(o), m_function(function) 
	 {}
	 /*! Clones a callback
	 */
	 virtual AbstractCallback * clone() { return new MethodCallback<_Class>(*this); }
	 /*! Not invoked, since this is saddy event callback
	  */
	 virtual void invoke() { (m_o->*m_function)(); }
private:
	 _Class * m_o;
	 void (_Class::*m_function)();
};

/*! A callback with delegated method invocation
 */
template<
typename _DelegatingClass,
typename _InvokedClass
>
class DelegatedMethodCallback: public AbstractCallback
{
public:
	 inline DelegatedMethodCallback(
		 _DelegatingClass * o, 
		 _InvokedClass * (_DelegatingClass::*getter)(),
		 void (_InvokedClass::*function)()
		 ) : 
	 m_o(o), m_getter(getter), m_function(function) 
	 {}
	 /*! Clones a callback
	 */
	 virtual AbstractCallback * clone() { return new DelegatedMethodCallback<_DelegatingClass, _InvokedClass>(*this); }
	 /*! Not invoked, since this is saddy event callback
	  */
	 virtual void invoke() 
	 { 
		 _InvokedClass * p = (m_o->*m_getter)();
		 (p->*m_function)();
	 }
private:
	 _DelegatingClass * m_o;
	 _InvokedClass * (_DelegatingClass::*m_getter)();
	 void (_InvokedClass::*m_function)();
};

class Callback: public AbstractCallback
{
public:
	inline Callback() { m_impl = NULL; }
	Callback(const Callback & o);
	Callback & operator=(const Callback &o);
	/*! Clones a callback
	 */
	virtual AbstractCallback * clone();
	/*! Not invoked, since this is saddy event callback
	 */
	virtual void invoke();

	inline ~Callback() { delete m_impl; }

	Callback( void (*f)() );


	template<typename _Class>
	Callback( _Class * o, void (_Class::*f)() )  
	{ m_impl =  new fsm::MethodCallback<_Class>(o, f); }



	template<typename _DelegatingClass, typename _InvokingClass>
	Callback( 
		_DelegatingClass * o, 
		_InvokingClass * (_DelegatingClass::*f)(), 
		void (_InvokingClass::*g)() 
	)  
	{ m_impl =  new fsm::DelegatedMethodCallback<_DelegatingClass, _InvokingClass>(o, f, g); }

private:
	AbstractCallback * m_impl; //!< Inner implementation of callback
	/*! Copies another callback
	 */
	void copy(const fsm::Callback & other);
};

/*! A names for callbacks from State Lists
 */
class Names
{
public:
	/*! Enter state callback name
	 */
	static const sad::String ENTER;
	/*! Leave state callback name
	 */
	static const sad::String LEAVE;
};

/*! An interface for handling state transitions
 */
class State
{
public:
	State();
	virtual void setMachine(fsm::Machine * machine);
	/*! Adds a new callback for event. Class takes ownership on callback 
		\param[in] eventType type of event for state
		\param[in] callback a callback data
	 */
    virtual void addCallback(const sad::String & eventType, fsm::AbstractCallback * callback);
	/*! Adds new callback on enter
		\param[in] f function
	 */
	inline void addEnterCallback(void (*f)())
	{ this->addCallback(fsm::Names::ENTER, new fsm::Callback(f)); }
	/*! Adds new callback on enter
		\param[in] o object
		\param[in] f function
	 */
	template<typename _Class>
	void addEnterCallback(_Class * o, void (_Class::*f)())
	{ this->addCallback(fsm::Names::ENTER, new fsm::Callback(o, f)); }
	/*! Adds new callback on leave
		\param[in] f function
	 */
	inline void addLeaveCallback(void (*f)())
	{ this->addCallback(fsm::Names::LEAVE, new fsm::Callback(f)); }
	/*! Adds new callback on leave
		\param[in] o object
		\param[in] f function
	 */
	template<typename _Class>
	void addLeaveCallback(_Class * o, void (_Class::*f)())
	{ this->addCallback(fsm::Names::LEAVE, new fsm::Callback(o, f)); }
	/*! Adds a new callback for event. Class takes ownership on callback 
		\param[in] eventType type of event for state
		\param[in] o class object
		\param[in] f a function for getting second object
		\param[in] g a callback
	 */
	template<typename _DelegatingClass, typename _InvokedClass>
	inline void addEventCallback(
		const sad::String & eventType, 
		_DelegatingClass * o,  
		_InvokedClass * (_DelegatingClass::*f)(),
		void (_InvokedClass::*g)()
	)
	{ this->addCallback(eventType, new fsm::Callback(o, f, g)); }
	/*! Removes a callback for eventtype
		\param[in] eventType type of event
	*/
	virtual void removeCallback(const sad::String & eventType);

	/*! Invokes names for the callback
		\param[in] name an event name
	 */
	virtual void invoke(const sad::String & name);

	virtual ~State();

	template<typename T> inline T & as() { return static_cast<T&>(*this); }
	template<typename T> inline const T & as() const{ return static_cast<const T&>(*this); }
private:
	typedef sad::Hash<sad::String, AbstractCallback *> CallbackList;
	typedef sad::Hash<int, AbstractCallback *> CharCallbackList;

	/*! Callbacks for hashes data
	 */
	CallbackList m_callbacks;
	/*! Link to machine, where action is stored
	 */
	fsm::Machine * m_machine;
	/*! Erase a callback list
		\param[in] c list
	 */
	void eraseCallbackList(CallbackList & c);
		/*! Erase a callback list
		\param[in] c list
	 */
	void eraseCallbackList(CharCallbackList & c);
};

class Machine;



/*! A machine data information
 */
class Machine
{
public:
	/*! Initial state machine name
	 */
	static const sad::String INITIAL;
public:
	Machine();

	/*! Returns a current state name. A fsm::Machine::INITIAL at start
		\return a current name of state
	 */
	inline const sad::String & currentStateName() const { return m_current_state_name;}
	/*! At start both of previous and current state are set to fsm::Machine::INITIAL
		\return previous name of state
	 */
	inline const sad::String & previousStateName()  { return m_previous_state_name;}
	/*! Returns a shared data of state machine 
		\return a shared data of machine
	 */
	inline fsm::Shared * sharedData() { return m_shared; }
	/*! A shared data
		\param[in] data data items
	 */
	void setSharedData(fsm::Shared * data);
	/*! A current state data. Null if not found
		\return a current state
	 */
	fsm::State * currentState();

	/*! Adding a state
		\param[in] name name of state
		\param[in] state state of object
	 */
	void addState(const sad::String & name, fsm::State * state);
	/*! A removes a state for data
		\param[in] name state to remove
	 */
	void removeState(const sad::String & name);
	/*! Pushes a new state for data
		\param[in] newState a state for new data
	 */
	void pushState(const sad::String & newState);
	/*! Returns a machine to an old state
	 */
	void cancelState();

	~Machine();
private:
	typedef sad::Hash<sad::String, fsm::State *> StateList;
	/*! Callbacks for hashes data
	 */
	StateList m_states;
	/*! A shared data for state machine
	 */
	fsm::Shared * m_shared;
	/*! A current state name
	 */ 
	sad::String m_current_state_name;
	/*! A previous state name
	 */
	sad::String m_previous_state_name;

	/*! Called on entering to a new state
	 */
	void enterCurrentState();
	/*! Called on leaving a previous state
	 */
	void leavePreviousState();
};
/*! Creates new condition of comparing current state of machine with handler
 */
class Condition: public sad::input::AbstractHanderCondition
{
public:
	/*! Constructs a condition for handlers
		\param[in] machine, which states will be tested
		\param[in] state a state, which current state of machine will be compared with
	 */
	Condition(sad::fsm::Machine * machine, const sad::String & state);
	/*! Tests current state of finite state machine against state name
		\param[in] e event
		\return whether we should  run an event
	 */
	virtual bool check(const sad::input::AbstractEvent & e);
	/*! Clones a condition
		\return new condition, which should be exact copy of current
	 */
	virtual sad::input::AbstractHanderCondition * clone();
	/*! You can inherit condition for implementing your very own conditions
	 */
	virtual ~Condition();
protected:
	/*! A machine, which condition goes to
	 */
	sad::fsm::Machine * m_machine;
	/*! A state name for conditions
	 */
	sad::String m_state;
};

}

}

/*! Creates new condition for comparing name of current state of machine with state name
	\param[in] machine  a machine, which current state is being compared
	\param[in] state    a state name
 */
sad::input::AbstractHanderCondition * operator*(
	sad::fsm::Machine * machine,
	const sad::String & state
);
