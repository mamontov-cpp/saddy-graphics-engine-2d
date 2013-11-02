#include "fsm/statemachine.h"
#include "log/log.h"
#include "../renderer.h"

sad::fsm::Shared::~Shared() { }

sad::fsm::AbstractCallback::~AbstractCallback() { }

void sad::fsm::FreeFunction::invoke() { m_function(); }
void sad::fsm::FreeFunction::invoke(const sad::Event & o) { m_function(); }
sad::fsm::AbstractCallback * sad::fsm::FreeFunction::clone() { return new sad::fsm::FreeFunction(*this); }


void sad::fsm::FreeEventFunction::invoke() { m_function(sad::Event()); }
void sad::fsm::FreeEventFunction::invoke(const sad::Event & o) { m_function(o); }
sad::fsm::AbstractCallback * sad::fsm::FreeEventFunction::clone() { return new sad::fsm::FreeEventFunction(*this); }


void sad::fsm::Callback::copy(const sad::fsm::Callback & other)
{
	if (other.m_impl != NULL)
	{	
		m_impl = other.m_impl->clone();
	}
	else 
	{
		m_impl = NULL;
	}
}

sad::fsm::Callback::Callback(const Callback & o)
{
	this->copy(o);
}

sad::fsm::Callback & sad::fsm::Callback::operator=(const sad::fsm::Callback &o)
{
	this->copy(o);
	return *this;
}

sad::fsm::AbstractCallback * sad::fsm::Callback::clone()
{
	return new sad::fsm::Callback(*this);
}

void sad::fsm::Callback::invoke()
{
 if (m_impl)
 {
	m_impl->invoke();
 }
}

void sad::fsm::Callback::invoke(const sad::Event & o)
{
 if (m_impl)
 {
	m_impl->invoke(o);
 }
}


sad::fsm::Callback::Callback( void (*f)() )
{
	m_impl = new sad::fsm::FreeFunction(f);
}

sad::fsm::Callback::Callback( void (*f)(const sad::Event &) )
{
	m_impl = new sad::fsm::FreeEventFunction(f);
}


const sad::String sad::fsm::Names::ENTER         = "enter";
const sad::String sad::fsm::Names::LEAVE         = "leave";
const sad::String sad::fsm::Names::MOUSEDOWN     = "mousedown";
const sad::String sad::fsm::Names::MOUSEUP       = "mouseup";
const sad::String sad::fsm::Names::MOUSECLICK    = "mouseclick";
const sad::String sad::fsm::Names::MOUSEDBLCLICK = "mousedblclick";
const sad::String sad::fsm::Names::MOUSEMOVE     = "mousemove";
const sad::String sad::fsm::Names::WHEEL         = "wheel";
const sad::String sad::fsm::Names::KEYDOWN       = "keydown";
const sad::String sad::fsm::Names::KEYUP         = "keyup";
const sad::String sad::fsm::Names::QUIT          = "quit";

sad::fsm::State::State() { m_machine = NULL;}
void sad::fsm::State::setMachine(sad::fsm::Machine * machine) { m_machine = machine; }
sad::fsm::State::~State() 
{ 
	eraseCallbackList(m_keyup);
	eraseCallbackList(m_keydown);
	eraseCallbackList(m_callbacks);
}

void sad::fsm::State::eraseCallbackList(sad::fsm::State::CallbackList & c)
{
	for(sad::fsm::State::CallbackList::iterator it = c.begin(); it != c.end(); it++ )
	{
		delete it.value();
	}
}

void sad::fsm::State::eraseCallbackList(sad::fsm::State::CharCallbackList & c)
{
	for(sad::fsm::State::CharCallbackList::iterator it = c.begin(); it != c.end(); it++ )
	{
		delete it.value();
	}
}

void sad::fsm::State::addCallback(const sad::String & eventType, sad::fsm::AbstractCallback * callback)
{
	if (m_callbacks.contains(eventType))
	{
		delete m_callbacks[eventType];
		m_callbacks[eventType] = callback;
	}
	else 
	{
		m_callbacks.insert(eventType, callback);
	}
}

void sad::fsm::State::removeCallback(const sad::String & eventType)
{
	if (m_callbacks.contains(eventType))
	{
		delete m_callbacks[eventType];
		m_callbacks.remove(eventType);
	}
}


void sad::fsm::State::invoke(const sad::String & eventType)
{
	if (m_callbacks.contains(eventType))
	{
		m_callbacks[eventType]->invoke();
	}
}


void sad::fsm::State::invoke(const sad::String & eventType,  const sad::Event & o)
{
	if (eventType == sad::fsm::Names::KEYUP && m_keyup.contains(o.key))
	{
		m_keyup[o.key]->invoke(o);
	}
	if (eventType == sad::fsm::Names::KEYDOWN && m_keydown.contains(o.key))
	{
		m_keydown[o.key]->invoke(o);
	}
	if (m_callbacks.contains(eventType))
	{
		m_callbacks[eventType]->invoke(o);
	}
}
void sad::fsm::State::removeKeyUpCallback(int key)
{
	if (m_keyup.contains(key))
	{
		delete m_keyup[key];
	}
}
void sad::fsm::State::removeKeyDownCallback(int key)
{
	if (m_keydown.contains(key))
	{
		delete m_keydown[key];
	}
}
void sad::fsm::State::addKeyUpCallback(int key, sad::fsm::AbstractCallback * c)
{
	if (m_keyup.contains(key))
	{
		delete m_keyup[key];
		m_keyup[key] = c;
	}
	else
	{
		m_keyup.insert(key, c);
	}
}
void sad::fsm::State::addKeyDownCallback(int key, sad::fsm::AbstractCallback * c)
{
	if (m_keydown.contains(key))
	{
		delete m_keydown[key];
		m_keydown[key] = c;
	}
	else
	{
		m_keydown.insert(key, c);
	}
}




bool sad::fsm::MachineEventCallback::empty()
{
	return false;
}

void sad::fsm::MachineEventCallback::operator()(const sad::Event & o)
{
	this->m_machine->invokeEvent(m_event_type, o);	
}



const sad::String sad::fsm::Machine::INITIAL = "previous";

sad::fsm::Machine::~Machine()
{
	for(StateList::iterator it = m_states.begin(); it != m_states.end(); it++ )
	{
		delete it.value();
	}
}

sad::fsm::State * sad::fsm::Machine::currentState()
{
	sad::fsm::State * r = NULL;
	if (m_states.contains(m_current_state_name))
	{
		r = m_states[m_current_state_name];
	}
	return r;
}

sad::fsm::Machine::Machine()
{
	m_previous_state_name = sad::fsm::Machine::INITIAL;
	m_current_state_name = sad::fsm::Machine::INITIAL;
	m_shared = NULL;
}

void sad::fsm::Machine::setSharedData(sad::fsm::Shared * data)
{
	m_shared = data;
}

void sad::fsm::Machine::addState(const sad::String & name, sad::fsm::State * state)
{
	SL_SCOPE(fmt::Format("sad::fsm::Machine::addState({0}, ...)") << name);
	if (m_states.contains(name))
	{
		delete m_states[name];
		m_states[name] = state;
		state->setMachine(this);
	} 
	else 
	{
		m_states.insert(name, state);
		state->setMachine(this);
	}
}

void sad::fsm::Machine::removeState(const sad::String & name)
{
	SL_INTERNAL_SCOPE(fmt::Format("sad::fsm::Machine::removeState({0})") << name, 
					  *(sad::Renderer::ref()));
	if (m_states.contains(name))
	{
		delete m_states[name];
		m_states.remove(name);
	}
}

void sad::fsm::Machine::invokeEvent(const sad::String & eventName, const sad::Event & o)
{
	sad::fsm::State * s = this->currentState();
	if (s)
	{
		s->invoke(eventName, o);
	}
}

void sad::fsm::Machine::enterCurrentState()
{
	SL_INTERNAL_SCOPE("sad::fsm::Machine::enterCurrentState()", *(sad::Renderer::ref()));
	if (m_states.contains(m_current_state_name))
	{
		m_states[m_current_state_name]->invoke(sad::fsm::Names::ENTER);
	}
}

void sad::fsm::Machine::leavePreviousState()
{
	SL_INTERNAL_SCOPE("sad::fsm::Machine::leavePreviousState()", *(sad::Renderer::ref()));
	if (m_states.contains(m_previous_state_name))
	{
		m_states[m_previous_state_name]->invoke(sad::fsm::Names::LEAVE);
	}
}

void sad::fsm::Machine::pushState(const sad::String & newState)
{
	SL_INTERNAL_SCOPE(fmt::Format("sad::fsm::Machine::pushState({0})") << newState,
					  *(sad::Renderer::ref()));
	SL_INTERNAL(fmt::Format("Current state is {0}") << m_current_state_name);
	m_previous_state_name = m_current_state_name;
	m_current_state_name = newState;
	leavePreviousState();
	enterCurrentState();
}

void sad::fsm::Machine::cancelState()
{
	SL_INTERNAL_SCOPE("sad::fsm::Machine::cancelState()", *(sad::Renderer::ref()));
	SL_INTERNAL(fmt::Format("Prevous state is {0}") << m_previous_state_name);
	pushState(m_previous_state_name);
}

sad::fsm::MachineEventCallback * sad::fsm::Machine::callbackFor(const sad::String & type)
{
	return new sad::fsm::MachineEventCallback(this, type);
}

void sad::fsm::Machine::addCallbacks(sad::Input * controls)
{
	const int length  = 9;
	void (sad::Input::*callbacks[length])(sad::EventHandler *) = {
		&sad::Input::setKeyDownHandler,
		&sad::Input::setKeyUpHandler,
		&sad::Input::setMouseClickHandler,

		&sad::Input::setMouseDblClickHandler,
		&sad::Input::setMouseDownHandler,
		&sad::Input::setMouseUpHandler,
		
		&sad::Input::setMouseMoveHandler,
		&sad::Input::setMouseWheelHandler,
		&sad::Input::setQuitHandler
	};

	sad::String names[length] = {
		sad::fsm::Names::KEYDOWN,
		sad::fsm::Names::KEYUP,
		sad::fsm::Names::MOUSECLICK,

		sad::fsm::Names::MOUSEDBLCLICK,
		sad::fsm::Names::MOUSEDOWN,
		sad::fsm::Names::MOUSEUP,

		sad::fsm::Names::MOUSEMOVE,
		sad::fsm::Names::WHEEL,
		sad::fsm::Names::QUIT,
	};

	for(int i = 0; i < length; i++)
	{
		(controls->*(callbacks[i]))(this->callbackFor(names[i]));
	}
}
