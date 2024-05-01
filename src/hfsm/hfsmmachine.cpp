#include "hfsm/hfsmmachine.h"

#include "db/dbtypename.h"
#include "opticksupport.h"

sad::hfsm::Machine::Machine()
: m_top(new sad::hfsm::State()), m_transitions(new sad::hfsm::TransitionRepository())
{
    PROFILER_EVENT;
    m_top->addRef();
    m_top->setMachine(this);
    m_transitions->setMachine(this);
}

sad::hfsm::Machine::~Machine()
{
    PROFILER_EVENT;
    m_top->delRef();
    delete m_transitions;
}

void sad::hfsm::Machine::enterState(const sad::String & state)
{
    PROFILER_EVENT;
    sad::String trimmed_state = state;
    trimmed_state.trimSpaces();

    sad::hfsm::State * previous_state  = this->state(m_current_state);
    sad::hfsm::State * current_state   = this->state(trimmed_state);
    if (trimmed_state == m_current_state || current_state == nullptr)
        return;
    
    m_previous_state = m_current_state;
    m_current_state = trimmed_state;
    
    previous_state->leave();
    m_transitions->invoke(m_previous_state, trimmed_state);
    current_state->enter();
}

sad::hfsm::State * sad::hfsm::Machine::state(const sad::String & s)
{
    PROFILER_EVENT;
    sad::String statename = s;
    statename.trimSpaces();

    if (statename.length() == 0)
    {
        return m_top;
    }
    else
    {
        sad::hfsm::State * result = m_top;
        sad::StringList path = statename.split("/");
        for(unsigned int i = 0; (i < path.size()) && (result != nullptr); i++)
        {
            result = result->child(path[i]);
        }
        return result;
    }
    return nullptr;
}

bool sad::hfsm::Machine::addState(
    const sad::String& full_path, 
    sad::hfsm::State * state, 
    bool force
)
{
    PROFILER_EVENT;
    bool statecreatedinside =  false;
    bool failedtoinsert = false;
    if (state == nullptr)
    {
        state = new sad::hfsm::State();
        statecreatedinside = true;
    }

    // Perform unsafe insertion
    sad::String trimmedfullpath = full_path;
    trimmedfullpath.trimSpaces();
    if (trimmedfullpath.length() == 0)
    {
        m_top->delRef();

        m_top->addRef();
        m_top = state;
        m_top->setMachine(this);
    }
    else
    {
        sad::hfsm::State * result = m_top;
        sad::StringList path = trimmedfullpath.split("/");
        for(unsigned int i = 0; (i < path.size() - 1) && (result != nullptr); i++)
        {
            sad::hfsm::State * parent = result;
            result = result->child(path[i]);
            if (result == nullptr)
            {
                if (force)
                {
                    sad::hfsm::State * nstate = new sad::hfsm::State();
                    parent->addChild(path[i], nstate);
                    result = nstate;
                }
                else
                {
                    failedtoinsert = true;
                }
            }
        }
        if (result)
        {
            result->addChild(path[path.size() - 1], state);
        }
    }

    // Delete state if failed
    if (statecreatedinside && failedtoinsert)
    {
        delete state;
    }
    return !failedtoinsert;
}


void sad::hfsm::Machine::removeState(const sad::String& full_path)
{
    PROFILER_EVENT;
    // Perform unsafe insertion
    sad::String trimmedfullpath = full_path;
    trimmedfullpath.trimSpaces();
    if (trimmedfullpath.length() == 0)
    {
        m_top->delRef();
        m_top = new sad::hfsm::State();
        m_top->setMachine(this);
        m_top->addRef();
    }
    else
    {
        sad::hfsm::State * result = m_top;
        sad::StringList path = trimmedfullpath.split("/");
        for(unsigned int i = 0; (i < path.size() - 1) && (result != nullptr); i++)
        {
            result = result->child(path[i]);
        }
        if (result)
        {
            result->removeChild(path[path.size() - 1]);
            m_transitions->removeStateMentions(trimmedfullpath);
        }
    }
}

bool sad::hfsm::Machine::stateExists(const sad::String & s)
{
    PROFILER_EVENT;
    return state(s) != nullptr;
}

sad::hfsm::TransitionRepository * sad::hfsm::Machine::transitions() const
{
    PROFILER_EVENT;
    return m_transitions;
}

void sad::hfsm::Machine::addTransition(
        const sad::String & from, 
        const sad::String & to,
        sad::hfsm::Transition * t
)
{
    PROFILER_EVENT;
    m_transitions->addTransition(from, to, t);
}

void sad::hfsm::Machine::removeTransition(
        const sad::String & from, 
        const sad::String & to	
)
{
    PROFILER_EVENT;
    m_transitions->removeTransition(from, to);
}

sad::hfsm::Transition * sad::hfsm::Machine::transition(
        const sad::String & from, 
        const sad::String & to
)
{
    PROFILER_EVENT;
    return m_transitions->transition(from, to);
}

const sad::String & sad::hfsm::Machine::currentState() const
{
    PROFILER_EVENT;
    return m_current_state;
}

const sad::String & sad::hfsm::Machine::previousState() const
{
    PROFILER_EVENT;
    return m_previous_state;
}

bool sad::hfsm::Machine::isInState(const sad::String & state) const
{
    PROFILER_EVENT;
    sad::String currentstate = this->currentState();
    sad::StringList currentstateparts = currentstate.split("/");
    for(int i = currentstateparts.size() - 1; i > -1; --i)
    {
        sad::StringList currentstatecheckedparts;
        for(int j = 0; j <= i; ++j)
        {
            currentstatecheckedparts << currentstateparts[j];
        }
        sad::String implodedcheckedparts = sad::join(currentstatecheckedparts, "/");
        if (state == implodedcheckedparts)
        {
            return true;
        }
    }
    return false;
}


sad::hfsm::CurrentStateCheck::CurrentStateCheck(
    sad::hfsm::Machine * machine,  
    const sad::String & state
) : m_machine(machine), m_state(state) 
{
    PROFILER_EVENT;

}


bool sad::hfsm::CurrentStateCheck::check(const sad::input::AbstractEvent & e)
{
    PROFILER_EVENT;
    return m_machine->currentState() == m_state;
}

sad::input::AbstractHandlerCondition * sad::hfsm::CurrentStateCheck::clone()
{
    PROFILER_EVENT;
    return new sad::hfsm::CurrentStateCheck(*this);
}

sad::hfsm::CurrentStateCheck::~CurrentStateCheck() = default;

sad::input::AbstractHandlerCondition * operator*(
    sad::hfsm::Machine * machine,
    const sad::String & state
)
{
    PROFILER_EVENT;
    return new sad::hfsm::CurrentStateCheck(machine, state);
}


sad::hfsm::MachineStateChangeTask::MachineStateChangeTask(
        sad::hfsm::Machine * machine,
        const sad::String & state
): m_machine(machine), m_state(state)
{
    PROFILER_EVENT;

}

sad::hfsm::MachineStateChangeTask::~MachineStateChangeTask() = default;

void sad::hfsm::MachineStateChangeTask::_process()
{
    PROFILER_EVENT;
    m_machine->enterState(m_state);
}

DECLARE_SOBJ(sad::hfsm::Machine)
