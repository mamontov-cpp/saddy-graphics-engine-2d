/*! \file pushvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "../sadstring.h"
#include "../db/dbvariant.h"
#include "../sadvector.h"
#include "../sadhash.h"
#include "../isrefcountable.h"

namespace dukpp03
{


namespace internal
{

/*! Tries to get ref-countable from variant via getting type
    \param[in] v variant
    \return data
 */
template<
    typename _Type
>
sad::RefCountable* tryGetRefCountable(sad::db::Variant* v)
{
    sad::Maybe<_Type*> t = v->get<_Type*>();
    if (t.exists())
    {
        return t.value();
    }
    return NULL;
}

/*! A generic finalizer for non-refcountable objects
 */
template<
    typename _Type, 
    bool _IsRefCountable
>
struct LocalFinalizer
{
    /*! Returns finalization function
     
        \param[in] ctx context
        \return context result
     */
    static FinalizerFunction getFinalizerFunction(_Type* o, sad::dukpp03::BasicContext* ctx)
    {
        return ::dukpp03::Finalizer<sad::dukpp03::BasicContext>::finalize;
    }
};

/*! A generic function for dereferencing reference-countable object when finalizing
     \param[in] ctx context
     \param[in] caster a function for fetching ref-countable value
 */
duk_ret_t unrefAndFinalize(duk_context* ctx, sad::RefCountable* (*caster)(sad::db::Variant*));


/*! A generic finalizer for refcountable objects
 */
template<
    typename _Type
>
struct LocalFinalizer<_Type, true>
{
    /*! A finalization function
        \param[in] ctx context
        \return 0
     */
    static duk_ret_t finalize(duk_context* ctx)
    {
        return ::dukpp03::internal::unrefAndFinalize(ctx, dukpp03::internal::tryGetRefCountable<_Type>);
    }

    /*! Returns finalization function
     
        \param[in] ctx context
        \return context result
     */
    static FinalizerFunction getFinalizerFunction(_Type* o, sad::dukpp03::BasicContext* ctx)
    {
        // If current context is pushed here
        if (static_cast<void*>(o)  ==  static_cast<void*>(ctx))
        {
            return ::dukpp03::Finalizer<sad::dukpp03::BasicContext>::finalize;
        }
        o->addRef();
        return ::dukpp03::internal::LocalFinalizer<_Type, true>::finalize;
    }

};

/*! A finalizer for objects, that adds reference to it and also, removes it when object is finalized
    \param[in] o object
    \param[in] 
    \return finalizer function, which delete reference if needed
 */
::dukpp03::FinalizerFunction finalizerMaker(sad::db::Object* o, sad::dukpp03::BasicContext* ctx);

/*! Returns default finalizer function
 */
template<
    typename T
>
::dukpp03::FinalizerFunction finalizerMaker(T* o, sad::dukpp03::BasicContext* ctx)
{
    return ::dukpp03::internal::LocalFinalizer<T, sad::IsRefCountable<T>::value>::getFinalizerFunction(o, ctx);
}

}



/*! Pushes a linear structure on stack
 */
template<
    template<
        typename Value
    > 
    class _LinearStructure,
    typename _ValueType,
    typename _Context
>
class PushLinearStructure
{
public:
     /*! Performs pushing value from stack
         \param[in] c context
         \param[in] result a result value
     */
    inline static void perform(
        _Context* c,
        const _LinearStructure<_ValueType> & result
    )
    {
        duk_context* ctx = c->context();
        int arr_idx = duk_push_array(ctx);
        int index = 0;
        for(typename _LinearStructure<_ValueType>::const_iterator it = result.begin(); it != result.end(); ++it)
        {
            dukpp03::PushValue<_ValueType, _Context>::perform(c, *it);
            duk_put_prop_index(ctx, arr_idx, index);
            ++index;
        }        
    }
};


/*! Pushes a dictionary structure on stack
 */
template<
    template<
        typename Key,
        typename Value
    > 
    class _DictionaryStructure,
    typename _ValueType,
    typename _Context
>
class PushDictionaryStructure
{
public:
     /*! Performs pushing value from stack
         \param[in] c context
         \param[in] result a result value
     */
    inline static void perform(
        _Context* c,
        const _DictionaryStructure<sad::String, _ValueType> & result
    )
    {
        duk_context* ctx = c->context();
        int obj_idx = duk_push_object(ctx);
        for(typename _DictionaryStructure<sad::String, _ValueType>::const_iterator it = result.const_begin(); it != result.const_end(); ++it)
        {
            dukpp03::PushValue<_ValueType, _Context>::perform(c, it.value());
            duk_put_prop_string(ctx, obj_idx, it.key().c_str());
        }        
    }
};

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
    ::dukpp03::FinalizerFunction f = ::dukpp03::internal::finalizerMaker(v, ctx);
    ctx->template pushVariant<T*>(sad::dukpp03::BasicContext::VariantUtils::template makeFrom(v), f);
}
    
};

/*! An instantiation for sad::Vector 
 */
template<
    typename T
>
class PushValue<sad::Vector<T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
     */
    static void perform(sad::dukpp03::BasicContext* ctx, const sad::Vector<T>& v)
    {
        ::dukpp03::PushLinearStructure<sad::Vector, T, sad::dukpp03::BasicContext>::perform(ctx, v);
    }
};

/*! An instantiation for sad::Hash 
 */
template<
    typename T
>
class PushValue<sad::Hash<sad::String, T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
     */
    static void perform(sad::dukpp03::BasicContext* ctx, const sad::Hash<sad::String, T>& v)
    {
        ::dukpp03::PushDictionaryStructure<sad::Hash, T, sad::dukpp03::BasicContext>::perform(ctx, v);
    }
};

/*! An instantiation for sad::Maybe 
 */
template<
    typename T
>
class PushValue<sad::Maybe<T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
     */
    static void perform(sad::dukpp03::BasicContext* ctx, const sad::Maybe<T>& v)
    {
        if (v.exists())
        {
            dukpp03::PushValue<T, sad::dukpp03::BasicContext>::perform(ctx, v.value());
        }
        else
        {
            duk_push_null(ctx->context());
        }
    }
};

}
 