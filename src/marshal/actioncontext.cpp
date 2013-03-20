#include "marshal/actioncontext.h"
#include "../log/log.h"

ActionContext::~ActionContext()
{
}

void ActionContext::pushAction(const hst::string & str)
{
	m_actions_stack<<str;
}

void ActionContext::popAction()
{
	if (m_actions_stack.count()!=0)
		m_actions_stack.removeAt(m_actions_stack.count()-1);
}

void LoggingActionContext::pushAction(const hst::string & str)
{
	SL_DEBUG( hst::string("Entering action section : ") + str);
	this->ActionContext::pushAction(str);
}

void LoggingActionContext::popAction()
{
	if (m_actions_stack.count()!=0) { 
		SL_DEBUG( hst::string("Leaving action section : ") + m_actions_stack[m_actions_stack.count()-1]);
		this->ActionContext::popAction();
	}
}