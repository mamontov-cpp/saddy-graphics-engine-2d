/*! \file   hfsmhandler.h
    

    A handler for hierarchical finite state events, which could be invoked immediately
 */
#pragma once

namespace sad
{

namespace hfsm
{

class State;
class Transition;
class Machine;

/*! A simple handler for transitions and event entering
 */
class AbstractHandler
{
public:
    /*! Creates default unbound handler
     */
    AbstractHandler();
    /*! Invokes an abstract handler
     */
    virtual void invoke() = 0;
    /*! Destroys an abstract handler
     */
    virtual ~AbstractHandler();
    /*! Sets  state, which handler is linked to
        \param[in] state a state
     */
    virtual void setState(sad::hfsm::State * state);
    /*! Sets transition,  which handler is linked to
        \param[in] t transition
     */
    virtual void setTransition(sad::hfsm::Transition * t);
    /*! Returns state for handler
        \return state for handler
     */
    virtual sad::hfsm::State * state() const;
    /*! Returns a transition for handler
        \return transition
     */ 
    virtual sad::hfsm::Transition * transition() const;
    /*! Returns a machine for a handler
        \return machine
     */
    virtual sad::hfsm::Machine * machine() const;
protected:
    /*! A state for abstract handler
     */
    sad::hfsm::State * m_state;
    /*! A transition for abstract handler
     */
    sad::hfsm::Transition * m_transition;
};

/*! A simple handler for callable function
 */
template<
typename _Callable
>
class Function:public sad::hfsm::AbstractHandler
{
public:
    /*! Creates simple function-like handler
        \param[in] f a function-alike item
     */
    Function(_Callable f) : sad::hfsm::AbstractHandler(), m_f(f)
    {

    }
    /*! Invokes a handler
     */
    virtual void invoke() override
    {
        m_f();
    }
    /*! Destroys stored callable item
     */ 
    virtual ~Function()
    {

    }
protected:
    /*! A simple function
     */
    _Callable m_f;
};

/*! A simple handler for calling method on object
 */ 
template<
    typename _Object,
    typename _MethodPointer
>
class Method:public sad::hfsm::AbstractHandler
{
public:
    /*! Creates a simple handler for calling method
        \param[in] o a pointer object, which method should be called on
        \param[in] f a pointer to method
     */
    Method(_Object o, _MethodPointer f) : sad::hfsm::AbstractHandler(), m_o(o), m_f(f)
    {

    }
    /*! Invokes a handler
     */
    virtual void invoke() override
    {
        (m_o->*m_f)();
    }
    /*! Destroys both pointers
     */
    virtual ~Method()
    {

    }
protected:
    /*! A pointer to object
     */
    _Object m_o;
    /*! A pointer to method, which should be called
     */
    _MethodPointer m_f;
};
/*! A simple handler for calling sequential method composition on object
 */ 
template<
    typename _Object,
    typename _MethodFPointer,
    typename _MethodGPointer
>
class MethodComposition: public sad::hfsm::AbstractHandler
{
public:
    /*! Creates a simple handler for calling method
        \param[in] o a pointer object, which method should be called on
        \param[in] f a pointer to method
        \param[in] g a second pointer, which should belong to object returned by first
     */
    MethodComposition(_Object o, _MethodFPointer f, _MethodGPointer g)
    : sad::hfsm::AbstractHandler(), m_o(o), m_f(f), m_g(g)
    {

    }
    /*! Invokes a handler
     */
    virtual void invoke() override
    {
        (((m_o->*m_f)())->*m_g)();
    }
    /*! Destroys all pointers
     */ 
    ~MethodComposition()
    {

    }
protected:
    /*! A pointer to object
     */
    _Object m_o;
    /*! A pointer to first method, which should be called
     */
    _MethodFPointer m_f;
    /*! A pointer to second method, which should be called
     */
    _MethodGPointer m_g;
};

}

}
