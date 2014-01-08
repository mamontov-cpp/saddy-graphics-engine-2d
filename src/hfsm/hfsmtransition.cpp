#include "hfsm/hfsmtransition.h"
#include "hfsm/hfsmhandler.h"

#include <string.h>

sad::hfsm::Transition::Transition() : m_machine(NULL), m_handler(NULL)
{

}

sad::hfsm::Transition::~Transition()
{
	delete m_handler;
}

void sad::hfsm::Transition::setMachine(sad::hfsm::Machine * machine)
{
	m_machine = machine;
}

void sad::hfsm::Transition::setHandler(sad::hfsm::AbstractHandler * handler)
{
	delete m_handler;
	m_handler = handler;
}

sad::hfsm::Machine * sad::hfsm::Transition::machine() const
{
	return m_machine;	
}
