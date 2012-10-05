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
		delete m_states[statename];
		m_states.remove(statename);
	}
}

