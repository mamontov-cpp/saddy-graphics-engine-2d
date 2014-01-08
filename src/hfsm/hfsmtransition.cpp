#include "hfsm/hfsmtransitionrepository.h"
#include "hfsm/hfsmtransition.h"
#include "hfsm/hfsmhandler.h"

#include <string.h>

sad::hfsm::Transition::Transition() : m_repository(NULL), m_handler(NULL)
{

}

sad::hfsm::Transition::~Transition()
{
	delete m_handler;
}

void sad::hfsm::Transition::setRepository(sad::hfsm::TransitionRepository * repo)
{
	m_repository = repo;
}

void sad::hfsm::Transition::setHandler(sad::hfsm::AbstractHandler * handler)
{
	delete m_handler;
	m_handler = handler;
}

sad::hfsm::Machine * sad::hfsm::Transition::machine() const
{
	if (m_repository)
		return m_repository->machine();
	return NULL;	
}


sad::hfsm::TransitionRepository * sad::hfsm::Transition::repository() const
{
	return m_repository;
}


void sad::hfsm::Transition::invoke()
{
	if (m_handler)
		m_handler->invoke();
}
