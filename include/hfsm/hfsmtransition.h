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
class TransitionRepository;
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
	/*! Sets a repository for transition
		\param[in] repo a machine
	 */
	virtual void setRepository(sad::hfsm::TransitionRepository * repo);
	/*! Sets a handler for transition, destroying last handler
		\param[in] handler a used handler
	 */
	virtual void setHandler(sad::hfsm::AbstractHandler * handler);
	/*! Returns machine for transition
		\return machine for transition
	 */
	virtual sad::hfsm::Machine * machine() const;
	/*! Returns repository
		\return linked repository
	 */
	virtual sad::hfsm::TransitionRepository * repository() const;
	/*! Invokes a transition handler if can
	 */
	virtual void invoke();
protected:
	/*! An abstract handler for transition
	 */
	sad::hfsm::AbstractHandler * m_handler;
	/*! An abstract machine
	 */
	sad::hfsm::TransitionRepository * m_repository;
};

}

}
