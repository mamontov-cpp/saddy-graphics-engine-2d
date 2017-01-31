/*! \file getvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "../sadstring.h"
 
namespace dukpp03
{


/*! An instantiation for getting constant char pointer
 */
template<>
class GetValue<const char*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack 
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<const char*> perform(
        sad::dukpp03::BasicContext* ctx, 
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<const char*> result;
        if (duk_is_string(ctx->context(), pos))
        {
            result.setValue(duk_to_string(ctx->context(), pos));
        }
        return result;
    }

};


/*! An instantiation for getting sad::String
 */
template<>
class GetValue<sad::String, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack 
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::String> perform(
        sad::dukpp03::BasicContext* ctx, 
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::String> result;
        if (duk_is_string(ctx->context(), pos))
        {
            result.setValue(duk_to_string(ctx->context(), pos));
        }
        return result;
    }

};


/*! An instantiation for variant
 */
template<>
class GetValue<sad::db::Variant,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
inline static ::dukpp03::Maybe<sad::db::Variant> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::db::Variant> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(sad::String(duk_to_string(ctx->context(), pos)));
    }
    if (duk_is_boolean(ctx->context(), pos))
    {
        result.setValue(static_cast<bool>(duk_to_string(ctx->context(), pos)));
    }
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(duk_to_number(ctx->context(), pos));
    }
    if (duk_is_object(ctx->context(), pos))
    {
        duk_get_prop_string(ctx->context(), pos, DUKPP03_VARIANT_PROPERTY_SIGNATURE);
        if (duk_is_pointer(ctx->context(), -1))
        {
            void* ptr = duk_to_pointer(ctx->context(), -1);
            sad::db::Variant * v = reinterpret_cast<sad::db::Variant *>(ptr);
            if (v)
            {
                result.setValue(*v);
            }
        }
        duk_pop(ctx->context());
    }
    return result;
}

};

}