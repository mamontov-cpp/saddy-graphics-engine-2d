/*!  \file closuremethodcall.h
 *   \author HiddenSeeker
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
    virtual ~ClosureMethodCall0()
    {

    }
    /*!
     * Invokes method
     */
    virtual void run()
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
     */
    inline ClosureMethodCall1(_ClassName * o, _Method f, const _Arg & a) : m_o(o), m_f(f), m_a(a)
    {

    }
    /*!
     * Can be inherited
     */
    virtual ~ClosureMethodCall1()
    {

    }
    /*!
     * Invokes method
     */
    virtual void run()
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
    virtual ~ClosureMethodCall2()
    {

    }
    /*!
     * Invokes method
     */
    virtual void run()
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

