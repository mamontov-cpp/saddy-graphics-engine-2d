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

}