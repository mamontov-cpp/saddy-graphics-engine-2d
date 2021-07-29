/*! \file animations/setstate/methodcall.h
    

    Defines a command, which casts object to class invokes a method on it
 */
#pragma once
#include "typedcommand.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! Defines a fast call as cast to object and calling a method on it
 */
template<
    typename _Object,
    typename _Method,
    typename _Argument
>
class MethodCall : public sad::animations::setstate::TypedCommand<_Argument>
{
public:
    /*! Constructs new empty method call object
        \param[in]  o object object, which method should be called upon
        \param[in]  f a method, which should be called
     */
    inline MethodCall(sad::db::Object* o, _Method f) : m_o(o), m_f(f) { }
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const override
    {
        return new sad::animations::setstate::MethodCall<_Object, _Method, _Argument>(m_o, m_f);
    }
    /*! Calls a method with specified argument
        \param[in] a argument
     */
    virtual void call(const _Argument& a) override
    {
        (static_cast<_Object *>(m_o)->*m_f)(a);
    }
    virtual ~MethodCall() override { }
protected:
    /*! An object link for fast call
     */
    sad::db::Object* m_o;
    /*! A method to be called on object
     */
    _Method m_f;
};

/*! Makes a method call
    \param[in] o object
    \param[in] f method
    \return fast call
 */
template<
    typename _Object,
    typename _Arg
>
sad::animations::setstate::AbstractSetStateCommand* make(sad::db::Object* o, void (_Object::*f)(const _Arg&))
{
    return new sad::animations::setstate::MethodCall<_Object, void (_Object::*)(const _Arg&), _Arg>(o, f);
}

/*! Makes a method call
    \param[in] o object
    \param[in] f method
    \return fast call
 */
template<
    typename _Object,
    typename _Arg
>
sad::animations::setstate::AbstractSetStateCommand* make(sad::db::Object* o, void (_Object::*f)(_Arg))
{
    return new sad::animations::setstate::MethodCall<_Object, void (_Object::*)(_Arg), _Arg>(o, f);
}

/*! Makes fast call, based on method
    \param[in] o object
    \param[in] f method
    \return fast call
 */
template<
    typename _Object,
    typename _Arg
>
sad::animations::setstate::AbstractSetStateCommand* make(sad::db::Object* o, void (_Object::*f)(const _Arg&) const)
{
    return new sad::animations::setstate::MethodCall<_Object, void (_Object::*)(const _Arg&) const, _Arg>(o, f);
}

/*! Makes a method call
    \param[in] o object
    \param[in] f method
    \return fast call
 */
template<
    typename _Object,
    typename _Arg
>
sad::animations::setstate::AbstractSetStateCommand* make(sad::db::Object* o, void (_Object::*f)(_Arg) const)
{
    return new sad::animations::setstate::MethodCall<_Object, void (_Object::*)(_Arg) const, _Arg>(o, f);
}

}

}

}
