#include "statemachine.h"

StateMachine::StateMachine() {
	m_state = -1;//because no one state exist yet
}
void StateMachine::bindState(int state, StateHandler *handler) {
	m_state_list[state] = handler;
}
bool StateMachine::pushState(int state) {
	bool result = false;
	if (m_state_list.contains(state)) {
		StateHandler *handler = m_state_list[state];
		if (handler->_operator(m_state)) {
			m_state = state;
			result = true;
		}
	}
	return result;
}

StateHandler::StateHandler(bool (*function)(int)) {
	m_operator_inner = function;
}
bool StateHandler::_operator(int prev_state) {
	return m_operator_inner(prev_state);
}
StateHandler::~StateHandler() {
}