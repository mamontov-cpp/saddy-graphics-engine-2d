/*! \file ptrconstructor.h
     
    Defines a simple defines for creating bindings for creating new objects on heap (not on stack)
 */
#pragma once
#include "basiccontext.h"

namespace dukpp03
{

{{#variants}}

/*! Defines a wrapper for method, which returns nothing and receives {{argscount}} arguments
 */
template<
    typename _Context,
    typename _ClassName{{#has_args}},{{/has_args}}
{{#args}}{{#not_last}}    typename _Arg{{number}},{{/not_last}}{{#last}}    typename _Arg{{number}}{{/last}}
{{/args}}
>
class PtrConstructor{{argscount}} : public dukpp03::Callable<_Context>
{
public:
    /*! Constructs new object
     */
    PtrConstructor{{argscount}}()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual dukpp03::Callable<_Context>* clone() override
    {
        return new ::dukpp03::PtrConstructor{{argscount}}<_Context, _ClassName{{#has_args}}, {{/has_args}}{{#args}}_Arg{{number}}{{#not_last}}, {{/not_last}}{{/args}}>();
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments() override
    {
        return {{argscount}};
    }
    DUKPP03_CBC_{{argscount}}{{#has_args}}(0,{{#args}}_Arg{{number}}{{#not_last}},{{/not_last}}{{/args}}){{/has_args}}
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int _call(_Context* c) override
    {
{{#args}}
        DUKPP03_MAYBE_FROM_STACK(_Arg{{number}}, {{number}}, {{number}}, {{numberp1}});
{{/args}}        
        _ClassName  *t = new _ClassName({{#has_args}}{{#args}}_a{{number}}._(){{#not_last}}, {{/not_last}}{{/args}}{{/has_args}});
        dukpp03::PushValue<_ClassName*, _Context>::perform(c, t);
        duk_push_current_function(c->context());
        duk_set_prototype(c->context(), -2);
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~PtrConstructor{{argscount}}() override = default;
};
{{/variants}}

}
