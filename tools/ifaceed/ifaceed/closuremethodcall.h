/*!  \file closuremethodcall.h
 *   
 *
 *   Defines a simple wrapper for creating a closure for simple method calls
 */
#pragma once
#include <closure.h>

/*! \class ClosureMethodCall0
 *
 * A class for closure method call with zero arguments
 */
template<
    typename _ClassName,
    typename _Method
>
class ClosureMethodCall0: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param o object
     * \param f method
     */
    inline ClosureMethodCall0(_ClassName * o, _Method f) : m_o(o), m_f(f)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall0() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)();
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
};

/*! \class ClosureMethodCall1
 *
 * A class for closure method call with one argument
 */
template<
    typename _ClassName,
    typename _Method,
    typename _Arg
>
class ClosureMethodCall1: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param o object
     * \param f method
     * \param a argument for calling
     */
    inline ClosureMethodCall1(_ClassName * o, _Method f, const _Arg & a) : m_o(o), m_f(f), m_a(a)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall1() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)(m_a);
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
    /*!
     * An argument to be passed into method call
     */
    _Arg m_a;
};

/*! \class ClosureMethodCall2
 *
 * A class for closure method call with two arguments
 */
template<
    typename _ClassName,
    typename _Method,
    typename _Arg1,
    typename _Arg2
>
class ClosureMethodCall2: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param o object
     * \param f method
     * \param a1 first argument
     * \param a2 second argument
     */
    inline ClosureMethodCall2(_ClassName * o, _Method f, const _Arg1& a1, const _Arg2& a2) : m_o(o), m_f(f), m_a1(a1), m_a2(a2)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall2() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)(m_a1, m_a2);
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
    /*!
     * A first argument to be passed into method call
     */
    _Arg1 m_a1;
    /*!
     * A second argument to be passed into method call
     */
    _Arg2 m_a2;
};


/*! \class ClosureMethodCall3
 *
 * A class for closure method call with three arguments
 */
template<
    typename _ClassName,
    typename _Method,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class ClosureMethodCall3: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param o object
     * \param f method
     * \param a1 first argument
     * \param a2 second argument
     * \param a3 third argument
     */
    inline ClosureMethodCall3(_ClassName * o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3) 
    : m_o(o), 
    m_f(f), 
    m_a1(a1), 
    m_a2(a2), 
    m_a3(a3)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall3() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)(m_a1, m_a2, m_a3);
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
    /*!
     * A first argument to be passed into method call
     */
    _Arg1 m_a1;
    /*!
     * A second argument to be passed into method call
     */
    _Arg2 m_a2;
    /*!
     * A third argument to be passed into method call
     */
    _Arg3 m_a3;
};


/*! \class ClosureMethodCall4
 *
 * A class for closure method call with four arguments
 */
template<
    typename _ClassName,
    typename _Method,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
class ClosureMethodCall4: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param[in] o object
     * \param[in] f method
     * \param[in] a1 first argument
     * \param[in] a2 second argument
     * \param[in] a3 third argument
     * \param[in] a4 fourth argument
     */
    inline ClosureMethodCall4(_ClassName * o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3, const _Arg4& a4) 
    : m_o(o), 
    m_f(f), 
    m_a1(a1), 
    m_a2(a2), 
    m_a3(a3),
    m_a4(a4)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall4() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)(m_a1, m_a2, m_a3, m_a4);
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
    /*!
     * A first argument to be passed into method call
     */
    _Arg1 m_a1;
    /*!
     * A second argument to be passed into method call
     */
    _Arg2 m_a2;
    /*!
     * A third argument to be passed into method call
     */
    _Arg3 m_a3;
    /*!
     * A fourth argument to be passed into method call
     */
    _Arg4 m_a4;
};


/*! \class ClosureMethodCall5
 *
 * A class for closure method call with five arguments
 */
template<
    typename _ClassName,
    typename _Method,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
class ClosureMethodCall5: public sad::ClosureBasic
{
public:
    /*!
     * Constructs new instance of method call
     * \param[in] o object
     * \param[in] f method
     * \param[in] a1 first argument
     * \param[in] a2 second argument
     * \param[in] a3 third argument
     * \param[in] a4 fourth argument
     * \param[in] a5 fifth argument
     */
    inline ClosureMethodCall5(_ClassName * o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3, const _Arg4& a4, const _Arg5& a5) 
    : m_o(o), 
    m_f(f), 
    m_a1(a1), 
    m_a2(a2), 
    m_a3(a3),
    m_a4(a4),
    m_a5(a5)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall5() override
    {

    }
    /*!
     * Invokes method
     */
    virtual void run() override
    {
        (m_o->*m_f)(m_a1, m_a2, m_a3, m_a4, m_a5);
    }
protected:
    /*!
     * An object for method invocation
     */
    _ClassName* m_o;
    /*!
     * A method to be called on object
     */
    _Method m_f;
    /*!
     * A first argument to be passed into method call
     */
    _Arg1 m_a1;
    /*!
     * A second argument to be passed into method call
     */
    _Arg2 m_a2;
    /*!
     * A third argument to be passed into method call
     */
    _Arg3 m_a3;
    /*!
     * A fourth argument to be passed into method call
     */
    _Arg4 m_a4;
    /*!
     * A fifth argument to be passed into method call
     */
    _Arg5 m_a5;
};

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 */
template<
  typename _ClassName,
  typename _Method
>
sad::ClosureBasic* bind(_ClassName* o, _Method f)
{
    return new ClosureMethodCall0<_ClassName, _Method>(o, f);
}

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 * \param[in] a argument
 */
template<
  typename _ClassName,
  typename _Method,
  typename _Arg
>
sad::ClosureBasic* bind(_ClassName* o, _Method f, const _Arg& a)
{
    return new ClosureMethodCall1<_ClassName, _Method, _Arg>(o, f, a);
}

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 * \param[in] a1 argument
 * \param[in] a2 argument
 */
template<
  typename _ClassName,
  typename _Method,
  typename _Arg1,
  typename _Arg2
>
sad::ClosureBasic* bind(_ClassName* o, _Method f, const _Arg1& a1, const _Arg2& a2)
{
    return new ClosureMethodCall2<_ClassName, _Method, _Arg1, _Arg2>(o, f, a1, a2);
}

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 * \param[in] a1 argument
 * \param[in] a2 argument
 * \param[in] a3 argument
 */
template<
  typename _ClassName,
  typename _Method,
  typename _Arg1,
  typename _Arg2,
  typename _Arg3
>
sad::ClosureBasic* bind(_ClassName* o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3)
{
    return new ClosureMethodCall3<_ClassName, _Method, _Arg1, _Arg2, _Arg3>(o, f, a1, a2, a3);
}

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 * \param[in] a1 argument
 * \param[in] a2 argument
 * \param[in] a3 argument
 * \param[in] a4 argument
 */
template<
  typename _ClassName,
  typename _Method,
  typename _Arg1,
  typename _Arg2,
  typename _Arg3,
  typename _Arg4
>
sad::ClosureBasic* bind(_ClassName* o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3, const _Arg4& a4)
{
    return new ClosureMethodCall4<_ClassName, _Method, _Arg1, _Arg2, _Arg3, _Arg4>(o, f, a1, a2, a3, a4);
}

/*!
 * Creates new call
 * \param[in] o object
 * \param[in] f method
 * \param[in] a1 argument
 * \param[in] a2 argument
 * \param[in] a3 argument
 * \param[in] a4 argument
 * \param[in] a5 argument
 */
template<
  typename _ClassName,
  typename _Method,
  typename _Arg1,
  typename _Arg2,
  typename _Arg3,
  typename _Arg4,
  typename _Arg5
>
sad::ClosureBasic* bind(_ClassName* o, _Method f, const _Arg1& a1, const _Arg2& a2, const _Arg3& a3, const _Arg4& a4, const _Arg5& a5)
{
    return new ClosureMethodCall5<_ClassName, _Method, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(o, f, a1, a2, a3, a4, a5);
}
