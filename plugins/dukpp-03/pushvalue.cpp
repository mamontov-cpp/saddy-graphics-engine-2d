#include "dukpp-03/context.h"


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
