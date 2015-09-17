#include "hfsm/hfsmtransitionrepository.h"
#include "hfsm/hfsmtransition.h"
#include "hfsm/hfsmhandler.h"

#include <string.h>

sad::hfsm::Transition::Transition() : m_repository(NULL)
{

}

sad::hfsm::Transition::~Transition()
{

}

void sad::hfsm::Transition::setRepository(sad::hfsm::TransitionRepository * repo)
{
    m_repository = repo;
}

sad::hfsm::AbstractHandler* sad::hfsm::Transition::handleWith(sad::hfsm::AbstractHandler * handler)
{
    if (handler)
    {
        m_handlers << handler;
    }
    return handler;
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
    for(size_t i = 0; i < m_handlers.size(); i++)
    {
        m_handlers[i]->invoke();
    }
}
