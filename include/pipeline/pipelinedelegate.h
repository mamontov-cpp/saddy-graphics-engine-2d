/*! \file pipelinedelegate.h
    

    Describes a simple delegate, used to determine common call of pipeline process and tasks
 */
#pragma once

namespace sad
{

namespace pipeline
{

/*! Pipeline delegate used to determine call method of pipeline process and tasks
 */
class Delegate
{
public:
    /*! Invokes a delegate
     */
    virtual void call() = 0;
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o);
    /*! A pipeline delegate is a base class for all kinds of delegates
     */
    virtual ~Delegate();
};

/* Determines callable function as null-ary function
 */
template<
    typename _Callable
>
class Function: public Delegate
{
public:
    /*! Creates new function delegate
        \param[in] f a callback
     */
    inline Function(_Callable f) : m_f(f)
    {
    }
    /*! Invokes a delegate
     */
    virtual void call() override
    {
        m_f();
    }
    /*! Destroys a function
     */
    virtual ~Function()  override
    {
    }
private:
    /*! A callable function
     */
    _Callable m_f;
};

/*! Defines a method call delegate
 */
template<
    typename _Object,
    typename _Method
>
class MethodCall: public Delegate
{
public:
    /*! Creates new method call
        \param[in] o an object
        \param[in] f a method
     */
    inline MethodCall(_Object * o, _Method f) : m_o(o), m_f(f)
    {
    }
    /*! Invokes a delegate
     */
    virtual void call() override
    {
        (m_o ->* m_f)();
    }
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o) override
    {
        m_o = static_cast<_Object *>(o);
    }
    /*! Destroys a method call
     */
    virtual ~MethodCall()  override
    {
    }
private:
    /*! An object, which method will be called on
     */
    _Object * m_o;
     /*! A method, which will be invoked
      */
    _Method m_f;
};


/*! Defines a composed method call delegate, which chains two method calls
 */
template<
    typename _Object,
    typename _FirstMethod,
    typename _SecondMethod
>
class ComposedMethodCall: public Delegate
{
public:
    /*! Creates new composed method call
        \param[in] o an object
        \param[in] f a method
        \param[in] g a second method to be called
     */
    inline ComposedMethodCall(_Object * o, _FirstMethod f, _SecondMethod g) : m_o(o), m_f(f), m_g(g)
    {
    }
    /*! Invokes a delegate
     */
    virtual void call() override
    {
        (((m_o ->* m_f)()) ->* m_g)();
    }
    /*! Changes object for all method call. Object is casted down to method
        \param[in] o object for method call
     */
    virtual void changeObject(void * o) override
    {
        m_o = static_cast<_Object *>(o);
    }
    /*! Destroys a method call
     */
    virtual ~ComposedMethodCall()  override
    {
    }
private:
    /*! An object, which method will be called on
     */
    _Object * m_o;
     /*! A method, which will be invoked
      */
    _FirstMethod m_f;
     /*! A second applied method
      */
    _SecondMethod m_g;
};


}

}

