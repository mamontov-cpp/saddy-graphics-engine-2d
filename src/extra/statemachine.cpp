#include "statemachine.h"

#include "../log/log.h"

using namespace sad;

fsm::Shared::~Shared() { }

fsm::AbstractCallback::~AbstractCallback() { }

void fsm::FreeFunction::invoke() { m_function(); }
void fsm::FreeFunction::invoke(const sad::Event & o) { m_function(); }
fsm::AbstractCallback * fsm::FreeFunction::clone() { return new fsm::FreeFunction(*this); }


void fsm::FreeEventFunction::invoke() { m_function(sad::Event()); }
void fsm::FreeEventFunction::invoke(const sad::Event & o) { m_function(o); }
fsm::AbstractCallback * fsm::FreeEventFunction::clone() { return new fsm::FreeEventFunction(*this); }


void fsm::Callback::copy(const fsm::Callback & other)
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

fsm::Callback::Callback(const Callback & o)
{
	this->copy(o);
}

fsm::Callback & fsm::Callback::operator=(const fsm::Callback &o)
{
	this->copy(o);
	return *this;
}

fsm::AbstractCallback * fsm::Callback::clone()
{
	return new fsm::Callback(*this);
}

void fsm::Callback::invoke()
{
 if (m_impl)
 {
	m_impl->invoke();
 }
}

void fsm::Callback::invoke(const sad::Event & o)
{
 if (m_impl)
 {
	m_impl->invoke(o);
 }
}


fsm::Callback::Callback( void (*f)() )
{
	m_impl = new fsm::FreeFunction(f);
}

fsm::Callback::Callback( void (*f)(const sad::Event &) )
{
	m_impl = new fsm::FreeEventFunction(f);
}


const hst::string fsm::Names::ENTER         = "enter";
const hst::string fsm::Names::LEAVE         = "leave";
const hst::string fsm::Names::MOUSEDOWN     = "mousedown";
const hst::string fsm::Names::MOUSEUP       = "mouseup";
const hst::string fsm::Names::MOUSECLICK    = "mouseclick";
const hst::string fsm::Names::MOUSEDBLCLICK = "mousedblclick";
const hst::string fsm::Names::MOUSEMOVE     = "mousemove";
const hst::string fsm::Names::WHEEL         = "wheel";
const hst::string fsm::Names::KEYDOWN       = "keydown";
const hst::string fsm::Names::KEYUP         = "keyup";
const hst::string fsm::Names::QUIT          = "quit";

fsm::State::State() { m_machine = NULL;}
void fsm::State::setMachine(fsm::Machine * machine) { m_machine = machine; }
fsm::State::~State() 
{ 
	eraseCallbackList(m_keyup);
	eraseCallbackList(m_keydown);
	eraseCallbackList(m_callbacks);
}

void fsm::State::eraseCallbackList(fsm::State::CallbackList & c)
{
	for(fsm::State::CallbackList::iterator it = c.begin(); it != c.end(); it++ )
	{
		delete it.value();
	}
}

void fsm::State::eraseCallbackList(fsm::State::CharCallbackList & c)
{
	for(fsm::State::CharCallbackList::iterator it = c.begin(); it != c.end(); it++ )
	{
		delete it.value();
	}
}

void fsm::State::addCallback(const hst::string & eventType, fsm::AbstractCallback * callback)
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

void fsm::State::removeCallback(const hst::string & eventType)
{
	if (m_callbacks.contains(eventType))
	{
		delete m_callbacks[eventType];
		m_callbacks.remove(eventType);
	}
}


void fsm::State::invoke(const hst::string & eventType)
{
	if (m_callbacks.contains(eventType))
	{
		m_callbacks[eventType]->invoke();
	}
}


void fsm::State::invoke(const hst::string & eventType,  const sad::Event & o)
{
	if (eventType == fsm::Names::KEYUP && m_keyup.contains(o.key))
	{
		m_keyup[o.key]->invoke(o);
	}
	if (eventType == fsm::Names::KEYDOWN && m_keydown.contains(o.key))
	{
		m_keydown[o.key]->invoke(o);
	}
	if (m_callbacks.contains(eventType))
	{
		m_callbacks[eventType]->invoke(o);
	}
}
void fsm::State::removeKeyUpCallback(int key)
{
	if (m_keyup.contains(key))
	{
		delete m_keyup[key];
	}
}
void fsm::State::removeKeyDownCallback(int key)
{
	if (m_keydown.contains(key))
	{
		delete m_keydown[key];
	}
}
void fsm::State::addKeyUpCallback(int key, fsm::AbstractCallback * c)
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
void fsm::State::addKeyDownCallback(int key, fsm::AbstractCallback * c)
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




bool fsm::MachineEventCallback::empty()
{
	return false;
}

void fsm::MachineEventCallback::operator()(const sad::Event & o)
{
	this->m_machine->invokeEvent(m_event_type, o);	
}



const hst::string fsm::Machine::INITIAL = "previous";

fsm::Machine::~Machine()
{
	for(StateList::iterator it = m_states.begin(); it != m_states.end(); it++ )
	{
		delete it.value();
	}
}

fsm::State * fsm::Machine::currentState()
{
	fsm::State * r = NULL;
	if (m_states.contains(m_current_state_name))
	{
		r = m_states[m_current_state_name];
	}
	return r;
}

fsm::Machine::Machine()
{
	m_previous_state_name = fsm::Machine::INITIAL;
	m_current_state_name = fsm::Machine::INITIAL;
	m_shared = NULL;
}

void fsm::Machine::setSharedData(fsm::Shared * data)
{
	m_shared = data;
}

void fsm::Machine::addState(const hst::string & name, fsm::State * state)
{
	SL_SCOPE(fmt::Format("fsm::Machine::addState({0}, ...)") << name);
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

void fsm::Machine::removeState(const hst::string & name)
{
	SL_SCOPE(fmt::Format("fsm::Machine::removeState({0})") << name);
	if (m_states.contains(name))
	{
		delete m_states[name];
		m_states.remove(name);
	}
}

void fsm::Machine::invokeEvent(const hst::string & eventName, const sad::Event & o)
{
	fsm::State * s = this->currentState();
	if (s)
	{
		s->invoke(eventName, o);
	}
}

void fsm::Machine::enterCurrentState()
{
	SL_SCOPE("fsm::Machine::enterCurrentState()");
	if (m_states.contains(m_current_state_name))
	{
		m_states[m_current_state_name]->invoke(fsm::Names::ENTER);
	}
}

void fsm::Machine::leavePreviousState()
{
	SL_SCOPE("fsm::Machine::leavePreviousState()");
	if (m_states.contains(m_previous_state_name))
	{
		m_states[m_previous_state_name]->invoke(fsm::Names::LEAVE);
	}
}

void fsm::Machine::pushState(const hst::string & newState)
{
	SL_SCOPE(fmt::Format("fsm::Machine::pushState({0})") << newState);
	SL_DEBUG(fmt::Format("Current state is {0}") << m_current_state_name);
	m_previous_state_name = m_current_state_name;
	m_current_state_name = newState;
	leavePreviousState();
	enterCurrentState();
}

void fsm::Machine::cancelState()
{
	SL_SCOPE("fsm::Machine::cancelState()");
	SL_DEBUG(fmt::Format("Prevous state is {0}") << m_previous_state_name);
	pushState(m_previous_state_name);
}

fsm::MachineEventCallback * fsm::Machine::callbackFor(const hst::string & type)
{
	return new fsm::MachineEventCallback(this, type);
}

void fsm::Machine::addCallbacks(sad::Input * controls)
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

	hst::string names[length] = {
		fsm::Names::KEYDOWN,
		fsm::Names::KEYUP,
		fsm::Names::MOUSECLICK,

		fsm::Names::MOUSEDBLCLICK,
		fsm::Names::MOUSEDOWN,
		fsm::Names::MOUSEUP,

		fsm::Names::MOUSEMOVE,
		fsm::Names::WHEEL,
		fsm::Names::QUIT,
	};

	for(int i = 0; i < length; i++)
	{
		(controls->*(callbacks[i]))(this->callbackFor(names[i]));
	}
}
