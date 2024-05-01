#include "hfsm/hfsmstate.h"
#include "hfsm/hfsmhandler.h"

#include "db/dbtypename.h"
#include "opticksupport.h"

sad::hfsm::State::State()
: m_machine(nullptr), m_shared(nullptr), m_parent(nullptr)
{
    PROFILER_EVENT;

}

sad::hfsm::State::~State()
{
    PROFILER_EVENT;
    if (m_shared)
    {
        m_shared->delRef();
    }
    for(sad::hfsm::StateMap::iterator it = m_children.begin(); it != m_children.end(); it++)
    {
        it.value()->delRef();
    }
}

void sad::hfsm::State::setMachine(sad::hfsm::Machine * machine)
{
    PROFILER_EVENT;
    m_machine = machine;
}

sad::hfsm::Machine * sad::hfsm::State::machine() const
{
    PROFILER_EVENT;
    return m_machine;
}

void sad::hfsm::State::addChild(const sad::String & name, sad::hfsm::State * state)
{
    PROFILER_EVENT;
    if (state == nullptr)
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

    state->setMachine(m_machine);
    state->setParent(this);
    state->addRef();
}

sad::hfsm::State * sad::hfsm::State::child(const sad::String & name) const
{
    PROFILER_EVENT;
    if (m_children.contains(name) == false)
    {
        return nullptr;
    }
    return m_children[name];
}

const sad::hfsm::StateMap & sad::hfsm::State::children() const
{
    PROFILER_EVENT;
    return m_children;
}

void  sad::hfsm::State::removeChild(const sad::String & name)
{
    PROFILER_EVENT;
    if (m_children.contains(name))
    {
        m_children[name]->delRef();
        m_children.remove(name);
    }
}

sad::hfsm::AbstractHandler* sad::hfsm::State::handleEnterWith(sad::hfsm::AbstractHandler * h)
{
    PROFILER_EVENT;
    if (h)
    {
        m_enter_handlers << h;
    }
    return h;
}

sad::hfsm::AbstractHandler* sad::hfsm::State::handleLeaveWith(sad::hfsm::AbstractHandler * h)
{
    PROFILER_EVENT;
    if (h)
    {
        m_leave_handlers << h;
    }
    return h;
}


void sad::hfsm::State::enter()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_enter_handlers.size(); i++)
    {
        m_enter_handlers[i]->invoke();
    }
}

void sad::hfsm::State::leave()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_leave_handlers.size(); i++)
    {
        m_leave_handlers[i]->invoke();
    }
}

void sad::hfsm::State::setShared(sad::hfsm::Shared * shared)
{
    PROFILER_EVENT;
    if (m_shared)
    {
        m_shared->delRef();
    }
    m_shared = shared;
    if (m_shared)
    {
        m_shared->addRef();
    }
}


sad::hfsm::Shared* sad::hfsm::State::shared() const
{
    PROFILER_EVENT;
    return m_shared;
}

void sad::hfsm::State::setParent(sad::hfsm::State * state)
{
    PROFILER_EVENT;
    m_parent = state;
}

sad::hfsm::State * sad::hfsm::State::parent() const
{
    PROFILER_EVENT;
    return m_parent;
}

void sad::hfsm::State::removeEnterHandler(sad::hfsm::AbstractHandler * f)
{
    PROFILER_EVENT;
    m_enter_handlers.removeAll(f);
    delete f;
}

void sad::hfsm::State::removeLeaveHandler(sad::hfsm::AbstractHandler * f)
{
    PROFILER_EVENT;
    m_leave_handlers.removeAll(f);
    delete f;
}

void sad::hfsm::State::setVariable(const sad::String& name, const sad::db::Variant& v)
{
    PROFILER_EVENT;
    m_variables.insert(name, v);
}

sad::Maybe<sad::db::Variant> sad::hfsm::State::getVariable(const sad::String& name)
{
    PROFILER_EVENT;
    if (m_variables.contains(name))
    {
        return sad::Maybe<sad::db::Variant>(m_variables[name]);
    }
    return sad::Maybe<sad::db::Variant>();
}


DECLARE_SOBJ(sad::hfsm::State)
