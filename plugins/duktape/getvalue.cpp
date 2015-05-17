#include "duktape/getvalue.h"
#include "duktape/context.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbtypename.h"


sad::Maybe<bool> sad::duktape::GetValue<bool>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<bool> result;
    if (duk_is_boolean(ctx->context(), pos))
    {
        result.setValue(duk_to_boolean(ctx->context(), pos) > 0);
    } 
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(duk_to_number(ctx->context(), pos) > 0);
    } 
    else
    {
        if (duk_is_string(ctx->context(), pos))
        {
            const char* str = duk_to_string(ctx->context(), pos);
            sad::db::Variant* v = ctx->getValueFromPool(str);
            if (v)
            {
                result = v->get<bool>(true);
            }
        }
    }
    return result;
}

sad::Maybe<char> sad::duktape::GetValue<char>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<char> result;
    sad::Maybe<sad::String> tmp = sad::duktape::GetValue<sad::String>::perform(ctx, pos);
    if (tmp.exists())
    {
        if (tmp.value().size() == 1) {
            result.setValue(tmp.value()[0]);
        }
    }
    if (!result.exists())
    {
        sad::Maybe<int> tmp2 = sad::duktape::GetValue<int>::perform(ctx, pos);
        if (tmp2.exists())
        {
            result.setValue(static_cast<char>(tmp2.value()));
        } 
        else
        {
            result = ctx->getValueFromPoolByStringFromStack<char>(pos);
        }
    }
    return result;
}

sad::Maybe<unsigned  char> sad::duktape::GetValue<unsigned char>::perform(
    sad::duktape::Context* ctx, 
    duk_idx_t pos
)
{
    sad::Maybe<unsigned char> result;
    sad::Maybe<sad::String> tmp = sad::duktape::GetValue<sad::String>::perform(ctx, pos);
    if (tmp.exists())
    {
        if (tmp.value().size() == 1) {
            result.setValue(static_cast<unsigned char>(tmp.value()[0]));
        }
    }
    if (!result.exists())
    {
        sad::Maybe<int> tmp2 = sad::duktape::GetValue<int>::perform(ctx, pos);
        if (tmp2.exists())
        {
            result.setValue(static_cast<unsigned char>(tmp2.value()));
        } 
        else
        {
            result = ctx->getValueFromPoolByStringFromStack<unsigned char>(pos);
        }
    }
    return result;
}

#define DEFINE_PERFORM_AS_CONVERSION_FROM_INT( TYPE )												  \
sad::Maybe< TYPE > sad::duktape::GetValue< TYPE >::perform(sad::duktape::Context* ctx, duk_idx_t pos) \
{																									  \
    sad::Maybe< TYPE > result;                                                                        \
    if (duk_is_number(ctx->context(), pos))															  \
    {																								  \
        result.setValue(static_cast< TYPE >(duk_to_int(ctx->context(), pos)));						  \
    }                                                                                                 \
    if (!result.exists())                                                                             \
    {                                                                                                 \
        result = ctx->getValueFromPoolByStringFromStack< TYPE >(pos);                                 \
    }																								  \
    return result;                                                                                    \
}

DEFINE_PERFORM_AS_CONVERSION_FROM_INT(short)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(unsigned short)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(int)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(unsigned int)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(long)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(unsigned long)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(long long)
DEFINE_PERFORM_AS_CONVERSION_FROM_INT(unsigned long long)


sad::Maybe<float> sad::duktape::GetValue<float>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<float> result;
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(static_cast<float>(duk_to_number(ctx->context(), pos)));
    }
    if (!result.exists())
    {
        result = ctx->getValueFromPoolByStringFromStack<float>(pos);
    }
    return result;
}

sad::Maybe<double> sad::duktape::GetValue<double>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<double> result;
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(duk_to_number(ctx->context(), pos));
    }
    if (!result.exists())
    {
        result = ctx->getValueFromPoolByStringFromStack<double>(pos);
    }
    return result;
}


sad::Maybe<long double> sad::duktape::GetValue<long double>::perform(sad::duktape::Context* ctx, duk_idx_t pos)
{
    sad::Maybe<long double> result;
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(duk_to_number(ctx->context(), pos));
    }
    if (!result.exists())
    {
        result = ctx->getValueFromPoolByStringFromStack<long double>(pos);
    }
    return result;
}


sad::Maybe<const char*> sad::duktape::GetValue<const char*>::perform(
    sad::duktape::Context* ctx, 
    duk_idx_t pos
)
{
    sad::Maybe<const char*> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(duk_to_string(ctx->context(), pos));
        sad::db::Variant* v = ctx->getValueFromPool(result.value());
        if (v)
        {
            sad::Maybe<sad::String> result1 = v->get<sad::String>();
            if (result1.exists())
            {
                result.setValue(result1.value().c_str());
            }
        }
    }
    return result;
}

sad::Maybe<sad::String> sad::duktape::GetValue<sad::String>::perform(
    sad::duktape::Context* ctx, 
    duk_idx_t pos
)
{
    sad::Maybe<sad::String> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(duk_to_string(ctx->context(), pos));
        sad::db::Variant* v = ctx->getValueFromPool(result.value());
        if (v)
        {
            sad::Maybe<sad::String> result1 = v->get<sad::String>();
            
            if (result1.exists())
            {
                result.setValue(result1.value().c_str());
            }
        }
    }
    return result;
}