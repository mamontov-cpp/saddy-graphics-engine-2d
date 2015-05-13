#pragma once
#include "getvalue.h"
#include "../db/save.h"
#include "../db/load.h"

namespace sad
{

namespace duktape
{

template<
    typename T
>
sad::Maybe<T> sad::duktape::GetValue<T>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<T> result;
    if (duk_is_string(ctx->context(), pos))
    {
        const char* string = duk_to_string(ctx->context(), pos);
        sad::db::Variant* v = ctx->getValueFromPool(string);
        if (v)
        {
            result = v->get<T>();
        }
    }
    return result;
}


}

}