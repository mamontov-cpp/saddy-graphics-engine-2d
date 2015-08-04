#include "duktape/pushvalue.h"
#include "duktape/context.h"

sad::String sad::duktape::PushValue<bool>::perform(sad::duktape::Context* ctx, bool v, bool)
{	
    duk_push_boolean(ctx->context(), v);
    return sad::String();
}

sad::String sad::duktape::PushValue<char>::perform(sad::duktape::Context* ctx, char v, bool)
{	
    char buffer[2] = {v, 0};
    duk_push_string(ctx->context(), buffer);
    return sad::String();
}

sad::String sad::duktape::PushValue<unsigned char>::perform(
    sad::duktape::Context* ctx, 
    unsigned char v, 
    bool
)
{	
    char buffer[2] = {static_cast<char>(v), 0};
    duk_push_string(ctx->context(), buffer);
    return sad::String();
}

sad::String sad::duktape::PushValue<int>::perform(sad::duktape::Context* ctx, int v, bool)
{
    if (v != v) //-V501
    {
        duk_push_nan(ctx->context());
        return "";
    }
    duk_push_int(ctx->context(), v);
    return sad::String();
}

sad::String sad::duktape::PushValue<float>::perform(sad::duktape::Context* ctx, float v, bool)
{
    if (v != v)
    {
        duk_push_nan(ctx->context());
        return sad::String();
    }
    duk_push_number(ctx->context(),static_cast<double>(v));
    return sad::String();
}

sad::String sad::duktape::PushValue<double>::perform(sad::duktape::Context* ctx, double v, bool)
{
    if (v != v)
    {
        duk_push_nan(ctx->context());
        return sad::String();
    }
    duk_push_number(ctx->context(),v);
    return sad::String();
}

sad::String sad::duktape::PushValue<const char*>::perform(sad::duktape::Context* ctx, const char* v, bool)
{	
    duk_push_string(ctx->context(), v);
    return sad::String();
}

sad::String sad::duktape::PushValue<char*>::perform(sad::duktape::Context* ctx, char* v, bool)
{	
    duk_push_string(ctx->context(), v);
    return sad::String();
}

sad::String sad::duktape::PushValue<sad::String>::perform(
    sad::duktape::Context* ctx,  
    sad::String v,
    bool
)
{
    duk_push_string(ctx->context(), v.c_str());
    return sad::String();
}
