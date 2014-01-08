/*! \file   hfsmtransition.h
	\author HiddenSeeker

	Defines a transition for hierarchical finite state transformation
 */
#pragma once
#include "hfsmhandler.h"

namespace sad
{

namespace hfsm
{

class Machine;

/*! Defines a transition for hierarchical finite state machine
 */ 
class Transition
{
public:
	/*! Creates default transition
	 */
	Transition();
	/*! Frees memory from linked handler
	 */
	virtual ~Transition();
	/*! Sets a machine for transition
		\param[in] machine a machine
	 */
	virtual void setMachine(sad::hfsm::Machine * machine);
	/*! Sets a handler for transition, destroying last handler
		\param[in] handler a used handler
	 */
	virtual void setHandler(sad::hfsm::AbstractHandler * handler);
	/*! Returns machine for transition
		\return machine for transition
	 */
	virtual sad::hfsm::Machine * machine() const;
protected:
	/*! An abstract handler for transition
	 */
	sad::hfsm::AbstractHandler * m_handler;
	/*! An abstract machine
	 */
	sad::hfsm::Machine * m_machine;
};

}

}
