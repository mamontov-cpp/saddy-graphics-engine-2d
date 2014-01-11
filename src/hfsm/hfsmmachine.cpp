#include "hfsm/hfsmmachine.h"

#include "hfsm/hfsmmachine.h"

sad::hfsm::Machine::Machine()
: m_top(new sad::hfsm::State()), m_transitions(new sad::hfsm::TransitionRepository())
{
	m_top->setMachine(this);
	m_transitions->setMachine(this);
}

sad::hfsm::Machine::~Machine()
{
	delete m_top;
	delete m_transitions;
}

void sad::hfsm::Machine::enterState(const sad::String & state)
{
	sad::String trimmedstate = state;
	trimmedstate.trimSpaces();

	sad::hfsm::State * previousstate  = this->state(m_currentstate);
	sad::hfsm::State * currentstate   = this->state(trimmedstate);
	if (trimmedstate == m_currentstate || currentstate == NULL)
		return;
	
	m_previousstate = m_currentstate;
	m_currentstate = state;
	
	previousstate->leave();
	m_transitions->invoke(m_previousstate, state);
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
		for(int i = 0; (i < path.size()) && (result != NULL); i++)
		{
			result = result->child(path[i]);
		}
		return result;
	}
	return NULL;
}

bool sad::hfsm::Machine::addState(
	const sad::String fullpath, 
	sad::hfsm::State * state, 
	bool force
)
{
	bool statecreatedinside =  false;
	bool failedtoinsert = false;
	if (state == NULL)
	{
		state = new sad::hfsm::State();
		statecreatedinside = true;
	}

	// Perform unsafe insertion
	sad::String trimmedfullpath = fullpath;
	trimmedfullpath.trimSpaces();
	if (trimmedfullpath.length() == 0)
	{
		delete m_top;
		m_top = state;
		m_top->setMachine(this);
	}
	else
	{
		sad::hfsm::State * result = m_top;
		sad::StringList path = trimmedfullpath.split("/");
		for(int i = 0; (i < path.size() - 1) && (result != NULL); i++)
		{
			sad::hfsm::State * parent = result;
			result = result->child(path[i]);
			if (result == NULL)
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


void sad::hfsm::Machine::removeState(const sad::String fullpath)
{
	// Perform unsafe insertion
	sad::String trimmedfullpath = fullpath;
	trimmedfullpath.trimSpaces();
	if (trimmedfullpath.length() == 0)
	{
		delete m_top;
		m_top = new sad::hfsm::State();
		m_top->setMachine(this);
	}
	else
	{
		sad::hfsm::State * result = m_top;
		sad::StringList path = trimmedfullpath.split("/");
		for(int i = 0; (i < path.size() - 1) && (result != NULL); i++)
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
	return state(s) != NULL;
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
	return m_currentstate;
}

const sad::String & sad::hfsm::Machine::previousState() const
{
	return m_previousstate;
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

sad::input::AbstractHanderCondition * sad::hfsm::CurrentStateCheck::clone()
{
	return new sad::hfsm::CurrentStateCheck(*this);
}

sad::hfsm::CurrentStateCheck::~CurrentStateCheck()
{

}

sad::input::AbstractHanderCondition * operator*(
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

sad::hfsm::MachineStateChangeTask::~MachineStateChangeTask()
{

}

void sad::hfsm::MachineStateChangeTask::_process()
{
	m_machine->enterState(m_state);
}

