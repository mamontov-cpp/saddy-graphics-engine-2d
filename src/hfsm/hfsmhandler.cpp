#include "hfsm/hfsmhandler.h"
#include "hfsm/hfsmstate.h"
#include "hfsm/hfsmtransition.h"

sad::hfsm::AbstractHandler::AbstractHandler()
: m_state(NULL), m_transition(NULL)
{

}

void sad::hfsm::AbstractHandler::setState(sad::hfsm::State * state)
{
	m_state = state;
}

void sad::hfsm::AbstractHandler::setTransition(sad::hfsm::Transition * t)
{
	m_transition = t;
}

sad::hfsm::State * sad::hfsm::AbstractHandler::state() const
{
	return m_state;
}

sad::hfsm::Transition * sad::hfsm::AbstractHandler::transition() const
{
	return m_transition;
}


sad::hfsm::Machine * sad::hfsm::AbstractHandler::machine() const
{
	sad::hfsm::State * s = state();
	if (s)
		return s->machine();
	sad::hfsm::Transition * t = transition();
	if (t)
		return t->machine();
	return NULL;
}

sad::hfsm::AbstractHandler::~AbstractHandler()
{

}
