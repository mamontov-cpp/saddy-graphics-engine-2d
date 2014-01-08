#include "hfsm/hfsmstate.h"
#include "hfsm/hfsmhandler.h"

sad::hfsm::State::State()
: m_enter(NULL), m_leave(NULL), m_machine(NULL)
{

}

sad::hfsm::State::~State()
{
	delete m_enter;
	delete m_leave;
	for(sad::hfsm::StateMap::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		delete it.value();
	}
}

void sad::hfsm::State::setMachine(sad::hfsm::Machine * machine)
{
	m_machine = machine;
}

sad::hfsm::Machine * sad::hfsm::State::machine() const
{
	return m_machine;
}

void sad::hfsm::State::addChild(const sad::String & name, sad::hfsm::State * state)
{
	if (state == NULL)
	{
		state = new sad::hfsm::State();
	}

	if (m_children.contains(name))
	{
		delete m_children[name];
		m_children[name] = state;
	}
	else
	{
		m_children.insert(name, state);
	}
}

sad::hfsm::State * sad::hfsm::State::child(const sad::String & name) const
{
	if (m_children.contains(name) == false)
	{
		return NULL;
	}
	return m_children[name];
}

const sad::hfsm::StateMap & sad::hfsm::State::children() const
{
	return m_children;
}

void  sad::hfsm::State::removeChild(const sad::String & name)
{
	if (m_children.contains(name))
	{
		delete m_children[name];
		m_children.remove(name);
	}
}

void sad::hfsm::State::setEnterHandler(sad::hfsm::AbstractHandler * h)
{
	delete m_enter;
	m_enter = h;
	if (h)
	{
		h->setState(this);
	}
}

void sad::hfsm::State::setLeaveHandler(sad::hfsm::AbstractHandler * h)
{
	delete m_leave;
	m_leave = h;
	if (h)
	{
		h->setState(this);
	}
}


void sad::hfsm::State::enter()
{
	if (m_enter)
	{
		m_enter->invoke();
	}
}

void sad::hfsm::State::leave()
{
	if (m_leave)
	{
		m_leave->invoke();
	}
}
