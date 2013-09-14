#include "editorbehaviourstate.h"

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

EditorBehaviourState::EditorBehaviourState()
{
	m_behaviour = NULL;
}

EditorBehaviourState::~EditorBehaviourState()
{
}

void EditorBehaviourState::enter()
{
}

void EditorBehaviourState::leave()
{
}

void EditorBehaviourState::onMouseMove(UNUSED const sad::Event & ev)
{

}

void EditorBehaviourState::onMouseDown(UNUSED const sad::Event & ev)
{

}

void EditorBehaviourState::onMouseUp(UNUSED const sad::Event & ev)
{

}

void EditorBehaviourState::onWheel(UNUSED const sad::Event & ev)
{

}


void EditorBehaviourState::onKeyDown(UNUSED const sad::Event & ev)
{

}

void EditorBehaviourState::onKeyUp(UNUSED const sad::Event & ev)
{

}

