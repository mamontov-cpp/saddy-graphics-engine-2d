#include "statemachine.h"

int high_score=0, current_score=0;
int player_health_point=10;
bool paused=false;


StateMachine * StateMachine::inst=NULL;

void StateMachine::quit()
{
	delete inst;
}
StateMachine::~StateMachine()
{
	for (hhash<int, StateHandler*>::iterator i=m_state_list.begin();i!=m_state_list.end();++i)
		delete i.value();
}
StateMachine::StateMachine() 
{
	m_state = -1;//because no one state exist yet
}
void StateMachine::bindState(int state, StateHandler *handler) 
{
	if (inst->m_state_list.contains(state)) delete inst->m_state_list[state];
	inst->m_state_list.insert(state,handler);
}
bool StateMachine::pushState(int state) 
{
	bool result = false;
	if (inst->m_state_list.contains(state)) 
	{
		StateHandler *handler = inst->m_state_list[state];
		if ((*handler)(inst->m_state)) 
		{
			inst->m_state = state;
			result = true;
		}
	}
	return result;
}

StateHandler::StateHandler(bool (*function)(int)) 
{
	m_operator_inner = function;
}
bool StateHandler::operator()(int prev_state) 
{
	return m_operator_inner(prev_state);
}
StateHandler::~StateHandler() 
{
}
