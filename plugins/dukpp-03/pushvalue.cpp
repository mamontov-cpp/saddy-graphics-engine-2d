#include "dukpp-03/context.h"


static duk_ret_t local_finalize(duk_context *ctx)
{
    sad::db::Variant* v =  dukpp03::Finalizer<sad::dukpp03::BasicContext>::getVariantToFinalize(ctx);
    if (v)
    {
        if (v->get<sad::db::Object*>().exists())
        {
            v->get<sad::db::Object*>().value()->delRef();
        }
        delete v;
    }
    return 0;
}

::dukpp03::FinalizerFunction finalizer_maker(sad::db::Object* o)
{
    return local_finalize;
}
