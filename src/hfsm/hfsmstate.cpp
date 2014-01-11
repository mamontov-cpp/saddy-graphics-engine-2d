#include "hfsm/hfsmstate.h"
#include "hfsm/hfsmhandler.h"

sad::hfsm::State::State()
: m_enter(NULL), m_leave(NULL), m_machine(NULL), m_shared(NULL), m_parent(NULL)
{

}

sad::hfsm::State::~State()
{
	delete m_enter;
	delete m_leave;
	delete m_shared;
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

	if (state)
	{
		state->setMachine(m_machine);
		state->setParent(this);
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

void sad::hfsm::State::handleEnterWith(sad::hfsm::AbstractHandler * h)
{
	delete m_enter;
	m_enter = h;
	if (h)
	{
		h->setState(this);
	}
}

void sad::hfsm::State::handleLeaveWith(sad::hfsm::AbstractHandler * h)
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

void sad::hfsm::State::setShared(sad::hfsm::Shared * shared)
{
	delete m_shared;
	m_shared = shared;
}


sad::hfsm::Shared * sad::hfsm::State::shared() const
{
	return m_shared;
}

void sad::hfsm::State::setParent(sad::hfsm::State * state)
{
	m_parent = state;
}

sad::hfsm::State * sad::hfsm::State::parent() const
{
	return m_parent;
}

