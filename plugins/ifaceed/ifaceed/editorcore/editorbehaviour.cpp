#include "editorbehaviour.h"
#include "editorbehaviourstate.h"

EditorBehaviour::EditorBehaviour(Editor * ed, const sad::String & initial)
{
	m_parent = ed;
	m_initial_state = initial;
}


EditorBehaviour::~EditorBehaviour()
{
	for (sad::Hash<sad::String, EditorBehaviourState *>::iterator it= m_states.begin(); 
		 it!=m_states.end();
		 it++)
	{
		delete it.value();
	}
}

void EditorBehaviour::addState(const sad::String & statename, EditorBehaviourState * state)
{
	SL_SCOPE("EditorBehaviour::addState()");
	if (m_states.contains(statename))
	{
		SL_CRITICAL(QString("State with name \"%1\" already exists").arg(statename.data()));
		delete m_states[statename];
	}
	state->setBehaviour(this);
	m_states.insert(statename, state);
}

void EditorBehaviour::removeState(const sad::String & statename)
{
	SL_SCOPE("EditorBehaviour::removeState()");
	if (!m_states.contains(statename))
	{
		SL_CRITICAL(QString("State with name \"%1\" does not exists").arg(statename.data()));		
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
	SL_SCOPE("EditorBehaviour::activate()");
	if (m_active_state.length())
	{
		SL_WARNING(QString("State \"%1\" is already active").arg(m_active_state.data()));
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
	m_active_state = sad::String();
}

void EditorBehaviour::cancelState()
{
	sad::String old_state = m_previous_state; 
	this->enterState(old_state);
}

void EditorBehaviour::enterState(const sad::String & state)
{
	SL_SCOPE("EditorBehaviour::enterState()");
	if (m_active_state.length())
	{
		m_states[m_active_state]->leave();
	}
	if (m_states.contains(state) == false)
	{
		m_previous_state = m_active_state;
		m_active_state.clear();
		SL_WARNING(QString("State \"%1\" is not found in behaviour").arg(state.data()));
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

EditorBehaviourState * EditorBehaviour::getState(const sad::String & statename) const
{
	EditorBehaviourState * state = NULL;
	if (m_states.contains(statename)) 
	{
		state = m_states[statename];
	}
	return state;
}
