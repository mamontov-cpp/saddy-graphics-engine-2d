/*! \file   hfsmtransition.h
	

	Defines a transition for hierarchical finite state transformation
 */
#pragma once

#include "hfsmhandler.h"

#include "../sadptrvector.h"

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
	virtual void setRepository(sad::hfsm::TransitionRepository* repo);
	/*! Sets a handler for transition, destroying last handler
		\param[in] handler a used handler
		\return handler
	 */
	virtual sad::hfsm::AbstractHandler* handleWith(sad::hfsm::AbstractHandler* handler);
	/*! Sets a handler for transition, destroying last handler and setting it to callable function
		\param[in] f callable data
		\return handler
	 */
	template<typename _Callable>
	sad::hfsm::AbstractHandler* addHandler(_Callable f)
	{
		return this->handleWith(new sad::hfsm::Function<_Callable>(f));
	}
	/*! Sets a handler for transition, destroying last handler and setting it to 
		call a method on an object
		\param[in] o object
		\param[in] p method pointer
		\return handler
	 */
	template<typename _Object, typename _MethodPointer>
	sad::hfsm::AbstractHandler* addHandler(_Object o, _MethodPointer p)
	{
		return this->handleWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
	}
	/*! Sets a handler for transition, destroying last handler and setting it to 
		call as sequential call of methods in object
		\param[in] o object
		\param[in] f first method pointer
		\param[in] g second method pointer
		\return handler
	 */
	template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
	sad::hfsm::AbstractHandler* addHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
	{
		return this->handleWith(new sad::hfsm::MethodComposition<_Object, _MethodFPointer, _MethodGPointer>(o, f, g));
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
	sad::PtrVector<sad::hfsm::AbstractHandler> m_handlers;
	/*! An abstract machine
	 */
	sad::hfsm::TransitionRepository * m_repository;
};

}

}
