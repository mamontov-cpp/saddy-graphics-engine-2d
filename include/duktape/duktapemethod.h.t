/*! \file duktapemethod.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for any kind of method
 */
#pragma once
#include "duktapecallable.h"
#include "decay.h"
#include "getvalue.h"
#include "pushvalue.h"
#include "errorcodes.h"
#include "context.h"
#include "../db/dbtypename.h"

namespace sad
{

namespace duktape
{
{{#variants}}

/*! Defines a wrapper for method, which returns nothing and receives {{argscount}} arguments
 */
template<
    typename _ClassName{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
class VoidMethod{{argscount}} : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)({{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}});
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod{{argscount}}(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod{{argscount}}<_ClassName{{#has_args}}, {{/has_args}}{{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return {{argscountp1}};
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
{{#args}}
        sad::Maybe< typename sad::duktape::Decay<_Arg{{number}}>::Type > _a{{number}} = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg{{number}}>::Type >::perform(c, {{numberp1}});
{{/args}}       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        {{#args}}       
        if (_a{{number}}.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::init();
            c->throwError("Invalid type passed for argument {{numberp2}}. Argument {{numberp2}} must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        {{/args}}
        
        try
        {
            ((_cls.mutableValue()).*m_callee)({{#args}}_a{{number}}.mutableValue(){{#not_last}}, {{/not_last}}{{/args}});
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod{{argscount}}()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};
{{/variants}}
{{#variants}}

/*! Defines a wrapper for method, which returns some value and receives {{argscount}} arguments
 */
template<
    typename _ClassName,
    typename _ReturnType{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
class RetMethod{{argscount}} : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)({{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}});
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod{{argscount}}(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod{{argscount}}<_ClassName, _ReturnType{{#has_args}}, {{/has_args}}{{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return {{argscountp1}};
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
{{#has_args}}{{#args}}      sad::Maybe< typename sad::duktape::Decay<_Arg{{number}}>::Type > _a{{number}} = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg{{number}}>::Type >::perform(c, {{number}});
        {{/args}}
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
{{#args}}       if (_a{{number}}.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::init();
            c->throwError("Invalid type passed for argument {{numberp1}}. Argument {{numberp1}} must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
{{/args}}

{{/has_args}}       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)({{#args}}_a{{number}}.mutableValue(){{#not_last}}, {{/not_last}}{{/args}});
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod{{argscount}}()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};
{{/variants}}

{{#variants}}
/*! Makes callable from method which returns nothing and receives {{argscount}} arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)({{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}))
{
    return new VoidMethod{{argscount}}<_ClassName{{#has_args}}, {{/has_args}}{{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}}>(f);
}

{{/variants}}

{{#variants}}
/*! Makes callable from method which returns value and receives {{argscount}} arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)({{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}))
{
    return new RetMethod{{argscount}}<_ClassName, _ReturnType{{#has_args}}, {{/has_args}}{{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}}>(f);
}

{{/variants}}
}

}