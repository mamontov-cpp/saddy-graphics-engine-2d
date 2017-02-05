/*! \file pushvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "../sadstring.h"
#include "../db/dbvariant.h"

namespace dukpp03
{


namespace internal
{

/*! A finalizer for objects, that adds reference to it and also, removes it when object is finalized
    \return finalizer function, which delete reference if needed
 */
::dukpp03::FinalizerFunction finalizer_maker(sad::db::Object* o);


/*! Returns default finalizer function
 */
template<
    typename T
>
::dukpp03::FinalizerFunction finalizer_maker(T* o)
{
    return ::dukpp03::Finalizer<sad::dukpp03::BasicContext>::finalize;
}

}

/*! An instantiation for pushing sad::String on stack
 */ 
template<>
class PushValue<sad::String, sad::dukpp03::BasicContext>
{
public:
/*! Performs pushing value 
    \param[in] ctx context
    \param[in] v value
 */
static void perform(sad::dukpp03::BasicContext* ctx, const std::string& v);
};


/*! An instantiation for pushing sad::String on stack
 */ 
template<>
class PushValue<const char*, sad::dukpp03::BasicContext>
{
public:
/*! Performs pushing value 
    \param[in] ctx context
    \param[in] v value
 */
static void perform(sad::dukpp03::BasicContext* ctx, const char*& v);
};

/*! An instantiation for pushing variant on stack
 */
template<>
class PushValue<sad::db::Variant, sad::dukpp03::BasicContext>
{
public:
/*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
 */
static void perform(sad::dukpp03::BasicContext* ctx, const sad::db::Variant& v);
};

/*! An instantiation, which ensures, that created sad::Object and sad::db::Object 
    will be garbage-collected
 */
template<
    typename T    
>
class PushValue<T*, sad::dukpp03::BasicContext>
{
public:
/*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
 */
static void perform(sad::dukpp03::BasicContext* ctx, T* v)
{
    ::dukpp03::FinalizerFunction f = ::dukpp03::internal::finalizer_maker(v);
    ctx->template pushVariant<T*>(sad::dukpp03::BasicContext::VariantUtils::template makeFrom(v), f);
}
    
};

}
 