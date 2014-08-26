#include "editorbehaviour.h"
#include "editorbehaviourstate.h"

core::EditorBehaviour::EditorBehaviour(Editor * ed, const sad::String & initial)
{
	m_parent = ed;
	m_initial_state = initial;
}


core::EditorBehaviour::~EditorBehaviour()
{
	for (sad::Hash<sad::String, core::EditorBehaviourState *>::iterator it= m_states.begin(); 
		 it!=m_states.end();
		 it++)
	{
		delete it.value();
	}
}

void core::EditorBehaviour::addState(const sad::String & statename, core::EditorBehaviourState * state)
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

void core::EditorBehaviour::removeState(const sad::String & statename)
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



void core::EditorBehaviour::activate()
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


void core::EditorBehaviour::deactivate()
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->leave();
	}
	m_active_state = sad::String();
}

void core::EditorBehaviour::cancelState()
{
	sad::String old_state = m_previous_state; 
	this->enterState(old_state);
}

void core::EditorBehaviour::enterState(const sad::String & state)
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

void core::EditorBehaviour::onMouseMove(const sad::input::MouseMoveEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseMove(ev);
	}
}

void core::EditorBehaviour::onMouseDown(const sad::input::MousePressEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseDown(ev);
	}
}

void core::EditorBehaviour::onMouseUp(const sad::input::MouseReleaseEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onMouseUp(ev);
	}
}

void core::EditorBehaviour::onWheel(const sad::input::MouseWheelEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onWheel(ev);
	}
}




void core::EditorBehaviour::onKeyDown(const sad::input::KeyPressEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onKeyDown(ev);
	}
}


void core::EditorBehaviour::onKeyUp(const sad::input::KeyReleaseEvent & ev)
{
	if (m_active_state.length())
	{
		m_states[m_active_state]->onKeyUp(ev);
	}
}

core::EditorBehaviourState * core::EditorBehaviour::getState(const sad::String & statename) const
{
	core::EditorBehaviourState * state = NULL;
	if (m_states.contains(statename)) 
	{
		state = m_states[statename];
	}
	return state;
}
