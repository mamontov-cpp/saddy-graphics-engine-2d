#include "fsm/statemachine.h"
#include "log/log.h"
#include "../renderer.h"

sad::fsm::Shared::~Shared() { }

sad::fsm::AbstractCallback::~AbstractCallback() { }

void sad::fsm::FreeFunction::invoke() { m_function(); }

sad::fsm::AbstractCallback * sad::fsm::FreeFunction::clone() { return new sad::fsm::FreeFunction(*this); }




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




sad::fsm::Callback::Callback( void (*f)() )
{
	m_impl = new sad::fsm::FreeFunction(f);
}




const sad::String sad::fsm::Names::ENTER         = "enter";
const sad::String sad::fsm::Names::LEAVE         = "leave";


sad::fsm::State::State() { m_machine = NULL;}
void sad::fsm::State::setMachine(sad::fsm::Machine * machine) { m_machine = machine; }
sad::fsm::State::~State() 
{ 
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

void sad::fsm::State::invoke(const sad::String &  name)
{
	m_callbacks[name]->invoke();
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



sad::fsm::Condition::Condition(sad::fsm::Machine * machine, const sad::String & state)
: m_machine(machine), m_state(state)
{
	
}
bool sad::fsm::Condition::check(const sad::input::AbstractEvent & e)
{
	return m_machine->currentStateName() == m_state;
}

sad::input::AbstractHanderCondition * sad::fsm::Condition::clone()
{
	return new sad::fsm::Condition(*this);
}

sad::fsm::Condition::~Condition()
{
	
}

sad::input::AbstractHanderCondition * operator*(
	sad::fsm::Machine * machine,
	const sad::String & state
)
{
	return new sad::fsm::Condition(machine, state);
}
