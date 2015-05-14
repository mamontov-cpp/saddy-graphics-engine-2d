/*! \file registercallable.h
    \author HiddenSeeker
    
    Defines a function, needed for registering simple callable function or method
 */
#pragma once
#include "context.h"
#include "duktapefunction.h"
#include "duktapemethod.h"
#include "duktapeconstructor.h"
#include "duktapeptrmethod.h"
 
namespace sad
{

namespace duktape
{
/*! Register callable object (method or function) as global function of context
    \param[in] ctx context
    \param[in] prop property name
    \param[in] o  object
 */
template<
    typename T
>
void register_callable(sad::duktape::Context* ctx, const sad::String& prop, T o)
{
    ctx->registerCallable(prop, sad::duktape::make_callable(o));
}

/*! Register callable method, which will be called via casting first argument to pointer
    \param[in] ctx context
    \param[in] prop property name
    \param[in] o  object
 */
template<
    typename T
>
void register_ptr_method(sad::duktape::Context* ctx, const sad::String& prop, T o)
{
    ctx->registerCallable(prop, sad::duktape::make_method_ptr_callable(o));
}

}
 
}
 