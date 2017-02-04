#include "dukpp-03/context.h"
#include "dukpp-03/pushvariant.h"

static duk_ret_t local_finalize(duk_context *ctx)
{
    sad::db::Variant* v =  dukpp03::Finalizer<sad::dukpp03::BasicContext>::getVariantToFinalize(ctx);
    sad::dukpp03::BasicContext* parent = static_cast<sad::dukpp03::BasicContext*>(::dukpp03::AbstractContext::getContext(ctx));
    if (v)
    {
        if (parent->isVariantRegistered(v))
        {
            if (v->get<sad::db::Object*>().exists())
            {
                v->get<sad::db::Object*>().value()->delRef();
            }
            delete v;
            parent->unregisterVariant(v);
        }
    }
    return 0;
}

::dukpp03::FinalizerFunction dukpp03::internal::finalizer_maker(sad::db::Object* o)
{
    o->addRef();
    return local_finalize;
}


void dukpp03::PushValue<sad::String, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const std::string& v)
{
    duk_push_string(ctx->context(), v.c_str());
}

void dukpp03::PushValue<const char*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const char*& v)
{
    duk_push_string(ctx->context(), v);
}

void dukpp03::PushValue<sad::db::Variant, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::db::Variant& v)
{
    sad::dukpp03::pushVariant(ctx, v);
}