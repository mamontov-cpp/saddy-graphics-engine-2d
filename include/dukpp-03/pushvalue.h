/*! \file pushvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "pushvariant.h"
#include "../sadstring.h"

namespace dukpp03
{

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
    inline static void perform(sad::dukpp03::BasicContext* ctx, const std::string& v)
    {
        duk_push_string(ctx->context(), v.c_str());
    }
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
    inline static void perform(sad::dukpp03::BasicContext* ctx, const char*& v)
    {
        duk_push_string(ctx->context(), v);
    }
};

/*! An instantiation for pushig variant on stack
 */
template<>
class PushValue<sad::db::Variant, sad::dukpp03::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
     */
    static void perform(sad::dukpp03::BasicContext* ctx, const sad::db::Variant& v)
    {
        sad::dukpp03::pushVariant(ctx, v);
    }
};

}
 