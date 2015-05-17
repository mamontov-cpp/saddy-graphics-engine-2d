/*! \file duktapeconstructor.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for creating new objects
 */
#pragma once
#include "duktapecallable.h"
#include "decay.h"
#include "getvalue.h"
#include "pushvalue.h"
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
class Constructor{{argscount}} : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor{{argscount}}()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor{{argscount}}<_ClassName{{#has_args}}, {{/has_args}}{{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}>();
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
        if (c->getTop() != {{argscount}})
        {
            c->throwError(sad::String("Function receives {{argscount}} arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }	
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
            _ClassName  t{{#has_args}}({{#args}}_a{{number}}.mutableValue(){{#not_last}}, {{/not_last}}{{/args}}){{/has_args}};
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor{{argscount}}()
    {
    }   
};
{{/variants}}

{{#variants}}
/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor{{argscount}}<_ClassName{{#has_args}}, {{/has_args}}{{#args}}{{#not_last}}_Arg{{number}}, {{/not_last}}{{#last}}_Arg{{number}}{{/last}}{{/args}}>();
    ctx->registerCallable(prop, c);
}

{{/variants}}

}

}
