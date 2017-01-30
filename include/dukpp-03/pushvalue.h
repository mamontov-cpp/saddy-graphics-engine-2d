/*! \file pushvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
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

}
 