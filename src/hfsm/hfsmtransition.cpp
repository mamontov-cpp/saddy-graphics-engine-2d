#include "hfsm/hfsmtransitionrepository.h"
#include "hfsm/hfsmtransition.h"
#include "hfsm/hfsmhandler.h"

#include "db/dbtypename.h"

#include <string.h>
#include "opticksupport.h"

sad::hfsm::Transition::Transition() : m_repository(nullptr)
{
    PROFILER_EVENT;

}

sad::hfsm::Transition::~Transition()
{
    PROFILER_EVENT;

}

void sad::hfsm::Transition::setRepository(sad::hfsm::TransitionRepository * repo)
{
    PROFILER_EVENT;
    m_repository = repo;
}

sad::hfsm::AbstractHandler* sad::hfsm::Transition::handleWith(sad::hfsm::AbstractHandler * handler)
{
    PROFILER_EVENT;
    if (handler)
    {
        m_handlers << handler;
    }
    return handler;
}

sad::hfsm::Machine * sad::hfsm::Transition::machine() const
{
    PROFILER_EVENT;
    if (m_repository)
        return m_repository->machine();
    return nullptr;	
}


sad::hfsm::TransitionRepository * sad::hfsm::Transition::repository() const
{
    PROFILER_EVENT;
    return m_repository;
}


void sad::hfsm::Transition::invoke()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_handlers.size(); i++)
    {
        m_handlers[i]->invoke();
    }
}

DECLARE_SOBJ(sad::hfsm::Transition)
