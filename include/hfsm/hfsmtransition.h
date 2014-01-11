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
	virtual void handleWith(sad::hfsm::AbstractHandler * handler);
	/*! Sets a handler for transition, destroying last handler and setting it to callable function
		\param[in] f callable data
	 */
	template<typename _Callable>
	void setHandler(_Callable f)
	{
		this->handleWith(new sad::hfsm::Function<_Callable>(f));
	}
	/*! Sets a handler for transition, destroying last handler and setting it to 
		call a method on an object
		\param[in] o object
		\param[in] p method pointer
	 */
	template<typename _Object, typename _MethodPointer>
	void setHandler(_Object o, _MethodPointer p)
	{
		this->handleWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
	}
	/*! Sets a handler for transition, destroying last handler and setting it to 
		call as sequential call of methods in object
		\param[in] o object
		\param[in] f first method pointer
		\param[in] g second method pointer
	 */
	template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
	void setHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
	{
		this->handleWith(new sad::hfsm::MethodComposition<_Object, _MethodPointer>(o, f, g));
	}
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
