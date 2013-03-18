#include "marshal/actioncontext.h"
#include "../log.h"

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
	hst::log::inst()->owrite("Entering action section : ").owrite(str).owrite("\"\n");
	this->ActionContext::pushAction(str);
}

void LoggingActionContext::popAction()
{
	if (m_actions_stack.count()!=0) { 
		hst::log::inst()->owrite("Leaving action section : ").owrite(m_actions_stack[m_actions_stack.count()-1]).owrite("\"\n");
		this->ActionContext::popAction();
	}
}