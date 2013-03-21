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

