#include "hfsm/hfsmtransitionrepository.h"
#include "opticksupport.h"

sad::hfsm::TransitionRepository::TransitionRepository()
: m_machine(nullptr)
{
    PROFILER_EVENT;
}

sad::hfsm::TransitionRepository::~TransitionRepository()
{
    PROFILER_EVENT;
    for(TransitionMap::iterator it = m_transitions.begin(); it != m_transitions.end(); it++)
    {
        clearTransitions(it.value());
    }
}

void sad::hfsm::TransitionRepository::setMachine(sad::hfsm::Machine * machine)
{
    PROFILER_EVENT;
    m_machine = machine;
}

sad::hfsm::Machine * sad::hfsm::TransitionRepository::machine() const
{
    PROFILER_EVENT;
    return m_machine;
}

void sad::hfsm::TransitionRepository::clearTransitions(
    const sad::Hash<sad::String, sad::hfsm::Transition *> & t
)
{
    PROFILER_EVENT;
    for(sad::Hash<sad::String, sad::hfsm::Transition *>::const_iterator it = t.const_begin(); 
        it != t.const_end(); 
        it++)
    {
        it.value()->delRef();
    }
}

void sad::hfsm::TransitionRepository::addTransition(
        const sad::String & from, 
        const sad::String & to,
        sad::hfsm::Transition * t
)
{
    PROFILER_EVENT;
    if (m_transitions.contains(from) == false)
    {
        m_transitions.insert(from, sad::Hash<sad::String, sad::hfsm::Transition *>());
    }
    
    sad::Hash<sad::String, sad::hfsm::Transition *> & c = m_transitions[from];
    if (c.contains(to))
    {
        delete c[to];
        c[to] = t;
    }
    else
    {
        c.insert(to, t); 
    }
    if (t) 
    {
        t->setRepository(this);
        t->addRef();
    }
}

 void sad::hfsm::TransitionRepository::removeTransition(
        const sad::String & from, 
        const sad::String & to	
    )
 {
    if (m_transitions.contains(from))
    {
        sad::Hash<sad::String, sad::hfsm::Transition *> & c = m_transitions[from];
        if (c.contains(to))
        {
            c[to]->delRef();
            c.remove(to);
        }
    }
 }

sad::hfsm::Transition * sad::hfsm::TransitionRepository::transition(
        const sad::String & from, 
        const sad::String & to
)
{
    PROFILER_EVENT;
    if (m_transitions.contains(from))
    {
        sad::Hash<sad::String, sad::hfsm::Transition *> & c = m_transitions[from];
        if (c.contains(to))
        {
            return c[to];
        }
    }
    return nullptr;
}

void sad::hfsm::TransitionRepository::invoke(
        const sad::String & from, 
        const sad::String & to
)
{
    PROFILER_EVENT;
    sad::hfsm::Transition * t = transition(from, to);
    if (t)
        t->invoke();
}


void sad::hfsm::TransitionRepository::removeStateMentions(
        const sad::String & state
)
{
    PROFILER_EVENT;
    if (m_transitions.contains(state))
    {
        clearTransitions(m_transitions[state]);
        m_transitions.remove(state);
    }
    for(TransitionMap::iterator it = m_transitions.begin(); it != m_transitions.end(); it++)
    {
        if (it.value().contains(state))
        {
            delete it.value()[state];
            it.value().remove(state);
        }
    }
}
