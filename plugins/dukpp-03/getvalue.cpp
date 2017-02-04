#include "dukpp-03/getvalue.h"

::dukpp03::Maybe<double> dukpp03::internal::tryGetDoubleProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
)
{
    ::dukpp03::Maybe<double> result;
    if (duk_has_prop_string(ctx->context(), pos, propname))
    {
        duk_get_prop_string(ctx->context(), pos, propname);
        result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}

::dukpp03::Maybe<int>  dukpp03::internal::tryGetIntProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
)
{
    ::dukpp03::Maybe<int> result;
    if (duk_has_prop_string(ctx->context(), pos, propname))
    {
        duk_get_prop_string(ctx->context(), pos, propname);
        result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}


dukpp03::Maybe<const char*> dukpp03::GetValue<const char*, sad::dukpp03::BasicContext>::perform(
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

dukpp03::Maybe<sad::String> dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(
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