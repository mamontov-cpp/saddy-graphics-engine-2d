#include "editorbehaviour.h"
#include "editorbehaviourstate.h"

EditorBehaviour::EditorBehaviour(Editor * ed, const hst::string & initial)
{
	m_parent = ed;
	m_initial_state = initial;
}


EditorBehaviour::~EditorBehaviour()
{
	for (hst::hash<hst::string, EditorBehaviourState *>::iterator it= m_states.begin(); 
		 it!=m_states.end();
		 it++)
	{
		delete it.value();
	}
}

void EditorBehaviour::addState(const hst::string & statename, EditorBehaviourState * state)
{
	if (m_states.contains(statename))
	{
		hst::log::inst()->owrite("Error: in behaviour  some state \"").owrite(statename).owrite("already exists\n");
		delete m_states[statename];
	}
	state->setBehaviour(this);
	m_states.insert(statename, state);
}

void EditorBehaviour::removeState(const hst::string & statename)
{
	if (!m_states.contains(statename))
	{
		hst::log::inst()->owrite("Error: in behaviour  some state \"").owrite(statename).owrite("not found\n");
		
	} 
	else 
	{
		m_states[statename]->setBehaviour(NULL);
		delete m_states[statename];
		m_states.remove(statename);
	}
}



void EditorBehaviour::activate()
{
	if (m_active_state.length())
	{
		hst::log::inst()->owrite("Warning: tried to activate behaviour, which is already active ( applied state ")
			             .owrite(m_active_state)
						 .owrite(")\n");
	}
	else
	{
		m_active_state = m_initial_state;
		m_states[m_active_state]->enter();
	}
}


void EditorBehaviour::deactivate()
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->leave();
	}
	m_active_state = hst::string();
}

void EditorBehaviour::cancelState()
{
	hst::string old_state = m_previous_state; 
	this->enterState(old_state);
}

void EditorBehaviour::enterState(const hst::string & state)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->leave();
	}
	if (m_states.contains(state) == false)
	{
		m_previous_state = m_active_state;
		m_active_state.clear();
		hst::log::inst()->owrite("Warning: not found a state in editor behavior with name ")
			             .owrite(state)
						 .owrite("\n");
	}
	else
	{
		m_previous_state = m_active_state;
		m_active_state = state;
		m_states[m_active_state]->enter();
	}
}

void EditorBehaviour::onMouseMove(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseMove(ev);
	}
}

void EditorBehaviour::onMouseDown(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseDown(ev);
	}
}

void EditorBehaviour::onMouseUp(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseUp(ev);
	}
}

void EditorBehaviour::onWheel(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onWheel(ev);
	}
}




void EditorBehaviour::onKeyDown(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onKeyDown(ev);
	}
}


void EditorBehaviour::onKeyUp(const sad::Event & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onKeyUp(ev);
	}
}
