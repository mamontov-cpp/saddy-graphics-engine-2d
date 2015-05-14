/*! \file duktapefunction.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for any kind of function
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

/*! Defines a wrapper for function, which returns nothing and receives {{argscount}} arguments
 */
{{#has_args}}
template<
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
{{/has_args}}
class VoidFunction{{argscount}} : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)({{#args}}{{#not_last}}_Arg{{number}},{{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}});
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction{{argscount}}(Function f){{#not_has_args}};{{/not_has_args}}{{#has_args}}: m_callee(f)   
    {
    }
{{/has_args}}
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone(){{#not_has_args}};{{/not_has_args}}{{#has_args}}  
    {
        return new sad::duktape::VoidFunction{{argscount}}<{{#args}}_Arg{{number}}{{#not_last}},{{/not_last}}{{/args}}>(m_callee);
    }{{/has_args}}  
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments(){{#not_has_args}};{{/not_has_args}}{{#has_args}}  
    {
        return {{argscount}};
    }{{/has_args}}  
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c){{#not_has_args}};{{/not_has_args}}{{#has_args}}  
    {
{{#args}}
        sad::Maybe< typename sad::duktape::Decay<_Arg{{number}}>::Type > _a{{number}} = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg{{number}}>::Type >::perform(c, {{number}});
{{/args}}{{#args}}      
        if (_a{{number}}.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::init();
            c->throwError("Invalid type passed for argument {{numberp1}}. Argument {{numberp1}} must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        {{/args}}
        
        try
        {
            m_callee({{#args}}_a{{number}}.mutableValue(){{#not_last}}, {{/not_last}}{{/args}});
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }{{/has_args}}
    /*! Can be inherited
     */
    virtual ~VoidFunction{{argscount}}(){{#not_has_args}};
{{/not_has_args}}{{#has_args}}
    {
    }
{{/has_args}}
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};
{{/variants}}
{{#variants}}

/*! Defines a wrapper for function, which returns some value and receives {{argscount}} arguments
 */
template<
    typename _ReturnType{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
class RetFunction{{argscount}} : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)({{#args}}{{#not_last}}_Arg{{number}},{{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}});
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction{{argscount}}(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction{{argscount}}<_ReturnType{{#has_args}},{{/has_args}}{{#args}}_Arg{{number}}{{#not_last}},{{/not_last}}{{/args}}>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return {{argscount}};
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
{{#has_args}}{{#args}}      sad::Maybe< typename sad::duktape::Decay<_Arg{{number}}>::Type > _a{{number}} = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg{{number}}>::Type >::perform(c, {{number}});
        {{/args}}
        
{{#args}}       if (_a{{number}}.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::init();
            c->throwError("Invalid type passed for argument {{numberp1}}. Argument {{numberp1}} must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg{{number}}>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
{{/args}}

{{/has_args}}       try
        {
            _ReturnType t = m_callee({{#args}}_a{{number}}.mutableValue(){{#not_last}}, {{/not_last}}{{/args}});
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction{{argscount}}()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};
{{/variants}}

{{#variants}}
/*! Makes callable from function which returns nothing and receives {{argscount}} arguments
    \param[in] f function
    \return callable version
 */
{{#has_args}}
template<
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
{{/has_args}}
inline sad::duktape::DuktapeCallable* make_callable(void (*f)({{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}))
{
    return new VoidFunction{{argscount}}{{#has_args}}<{{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}}>{{/has_args}}(f);
}

{{/variants}}

{{#variants}}
/*! Makes callable from function which returns value and receives {{argscount}} arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)({{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}))
{
    return new RetFunction{{argscount}}<_ReturnType{{#has_args}}, {{/has_args}}{{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}}>(f);
}

{{/variants}}
}

}