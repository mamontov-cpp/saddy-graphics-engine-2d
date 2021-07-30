#include "hfsm/hfsmmachine.h"

#include "db/dbtypename.h"

sad::hfsm::Machine::Machine()
: m_top(new sad::hfsm::State()), m_transitions(new sad::hfsm::TransitionRepository())
{
    m_top->addRef();
    m_top->setMachine(this);
    m_transitions->setMachine(this);
}

sad::hfsm::Machine::~Machine()
{
    m_top->delRef();
    delete m_transitions;
}

void sad::hfsm::Machine::enterState(const sad::String & state)
{
    sad::String trimmedstate = state;
    trimmedstate.trimSpaces();

    sad::hfsm::State * previousstate  = this->state(m_current_state);
    sad::hfsm::State * currentstate   = this->state(trimmedstate);
    if (trimmedstate == m_current_state || currentstate == nullptr)
        return;
    
    m_previous_state = m_current_state;
    m_current_state = state;
    
    previousstate->leave();
    m_transitions->invoke(m_previous_state, state);
    currentstate->enter();
}

sad::hfsm::State * sad::hfsm::Machine::state(const sad::String & s)
{
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
    return state(s) != nullptr;
}

sad::hfsm::TransitionRepository * sad::hfsm::Machine::transitions() const
{
    return m_transitions;
}

void sad::hfsm::Machine::addTransition(
        const sad::String & from, 
        const sad::String & to,
        sad::hfsm::Transition * t
)
{
    m_transitions->addTransition(from, to, t);
}

void sad::hfsm::Machine::removeTransition(
        const sad::String & from, 
        const sad::String & to	
)
{
    m_transitions->removeTransition(from, to);
}

sad::hfsm::Transition * sad::hfsm::Machine::transition(
        const sad::String & from, 
        const sad::String & to
)
{
    return m_transitions->transition(from, to);
}

const sad::String & sad::hfsm::Machine::currentState() const
{
    return m_current_state;
}

const sad::String & sad::hfsm::Machine::previousState() const
{
    return m_previous_state;
}

bool sad::hfsm::Machine::isInState(const sad::String & state) const
{
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

}


bool sad::hfsm::CurrentStateCheck::check(const sad::input::AbstractEvent & e)
{
    return m_machine->currentState() == m_state;
}

sad::input::AbstractHandlerCondition * sad::hfsm::CurrentStateCheck::clone()
{
    return new sad::hfsm::CurrentStateCheck(*this);
}

sad::hfsm::CurrentStateCheck::~CurrentStateCheck() = default;

sad::input::AbstractHandlerCondition * operator*(
    sad::hfsm::Machine * machine,
    const sad::String & state
)
{
    return new sad::hfsm::CurrentStateCheck(machine, state);
}


sad::hfsm::MachineStateChangeTask::MachineStateChangeTask(
        sad::hfsm::Machine * machine,
        const sad::String & state
): m_machine(machine), m_state(state)
{

}

sad::hfsm::MachineStateChangeTask::~MachineStateChangeTask() = default;

void sad::hfsm::MachineStateChangeTask::_process()
{
    m_machine->enterState(m_state);
}

DECLARE_SOBJ(sad::hfsm::Machine)
