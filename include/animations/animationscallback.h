/*! \file animationscallback.h
    

    Defines a callback, which should be called after animation end
 */
#pragma once

namespace sad
{
    
namespace animations
{

/*! A callback, which should be called after animation begins or ends
 */
class Callback
{
public:
    /*! Constructs new callback
     */
    Callback();
    /*! Must be inherited
     */
    virtual ~Callback();
    /*! Invokes a callback
     */
    virtual void invoke() = 0;
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const = 0;
};

/*! A callback, which represents a function call
 */
template<
    typename _Fun
>
class FunctionCall: public sad::animations::Callback
{
public:
    /*! Constructs new callback
     */
    FunctionCall(_Fun f) : m_f(f)
    {
        
    }
    /*! Must be inherited
     */
    virtual ~FunctionCall() override
    {
        
    }
    /*! Invokes a callback
     */
    virtual void invoke() override
    {
        m_f();
    }
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const override
    {
        return new sad::animations::FunctionCall<_Fun>(m_f);
    }
protected:
    /*! An inner function
     */
    _Fun m_f;
};

/*! A callback, which represents a method call
 */
template<
    typename _Object,
    typename _Method
>
class MethodCall0: public sad::animations::Callback
{
public:
    /*! Constructs new callback
     */
    MethodCall0(_Object* o, _Method f) : m_o(o), m_f(f)
    {
        
    }
    /*! Must be inherited
     */
    virtual ~MethodCall0()
    {
        
    }
    /*! Invokes a callback
     */
    virtual void invoke() override
    {
        (m_o->*m_f)();
    }
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const override
    {
        return new sad::animations::MethodCall0<_Object, _Method>(m_o, m_f);
    }
protected:
    /*! An object, which method is called upon
     */
    _Object* m_o;
    /*! An inner method
     */
    _Method m_f;
};

/*! A callback, which represents a method call with one argument
 */
template<
    typename _Object,
    typename _Method,
    typename _Arg
>
class MethodCall1: public sad::animations::Callback
{
public:
    /*! Constructs new callback
     */
    MethodCall1(_Object* o, _Method f, const _Arg& a) : m_o(o), m_f(f), m_a(a)
    {
        
    }
    /*! Must be inherited
     */
    virtual ~MethodCall1()
    {
        
    }
    /*! Invokes a callback
     */
    virtual void invoke() override
    {
        (m_o->*m_f)(m_a);
    }
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const override
    {
        return new sad::animations::MethodCall1<_Object, _Method, _Arg>(m_o, m_f, m_a);
    }
protected:
    /*! An object, which method is called upon
     */
    _Object* m_o;
    /*! An inner method
     */
    _Method m_f;
    /*! An argument for callback
     */
    _Arg m_a;
};


/*! A callback, which represents a method call with two arguments
 */
template<
    typename _Object,
    typename _Method,
    typename _Arg1,
    typename _Arg2
>
class MethodCall2: public sad::animations::Callback
{
public:
    /*! Constructs new callback
     */
    MethodCall2(_Object* o, _Method f, const _Arg1& a1, const _Arg2& a2) : m_o(o), m_f(f), m_a1(a1), m_a2(a2)
    {
        
    }
    /*! Must be inherited
     */
    virtual ~MethodCall2()
    {
        
    }
    /*! Invokes a callback
     */
    virtual void invoke() override
    {
        (m_o->*m_f)(m_a1, m_a2);
    }
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const override
    {
        return new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(m_o, m_f, m_a1, m_a2);
    }
protected:
    /*! An object, which method is called upon
     */
    _Object* m_o;
    /*! An inner method
     */
    _Method m_f;
    /*! A first argument for callback
     */
    _Arg1 m_a1;
    /*! A second argument for callback
     */
    _Arg2 m_a2;
};

/*! A callback, which represents a method call with three arguments
 */
template<
    typename _Object,
    typename _Method,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class MethodCall3: public sad::animations::Callback
{
public:
    /*! Constructs new callback
     */
    MethodCall3(_Object* o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3) : m_o(o), m_f(f), m_a1(a1), m_a2(a2), m_a3(a3)
    {
        
    }
    /*! Must be inherited
     */
    virtual ~MethodCall3()
    {
        
    }
    /*! Invokes a callback
     */
    virtual void invoke() override
    {
        (m_o->*m_f)(m_a1, m_a2, m_a3);
    }
    /*! Clones a callback
        \return copy of callback
     */
    virtual sad::animations::Callback* clone() const override
    {
        return new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(m_o, m_f, m_a1, m_a2, m_a3);
    }
protected:
    /*! An object, which method is called upon
     */
    _Object* m_o;
    /*! An inner method
     */
    _Method m_f;
    /*! A first argument for callback
     */
    _Arg1 m_a1;
    /*! A second argument for callback
     */
    _Arg2 m_a2;
    /*! A third argument for callback
     */
    _Arg3 m_a3;
};

}

}
