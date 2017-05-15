#include "dukpp-03/context.h"
#include "dukpp-03/pushvariant.h"

#include <sstream>


static duk_ret_t localFinalize(duk_context *ctx)
{
    return dukpp03::internal::unrefAndFinalize(ctx, dukpp03::internal::tryGetRefCountable<sad::db::Object>);
}

::dukpp03::FinalizerFunction dukpp03::internal::finalizerMaker(sad::db::Object* o, sad::dukpp03::BasicContext* ctx)
{
    o->addRef();
    return localFinalize;
}

duk_ret_t dukpp03::internal::unrefAndFinalize(duk_context* ctx, sad::RefCountable* (*caster)(sad::db::Variant*))
{
    sad::db::Variant* v =  dukpp03::Finalizer<sad::dukpp03::BasicContext>::getVariantToFinalize(ctx);
    sad::dukpp03::BasicContext* parent = static_cast<sad::dukpp03::BasicContext*>(::dukpp03::AbstractContext::getContext(ctx));
    if (v)
    {
        if (parent->isVariantRegistered(v))
        {
            sad::RefCountable* result = caster(v);
            if (result)
            {
                result->delRef();
            }
            delete v;
            parent->unregisterVariant(v);
        }
    }
    return 0; 
}

void dukpp03::PushValue<sad::String, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::String& v)
{
    duk_push_string(ctx->context(), v.c_str());
}

void dukpp03::PushValue<const sad::String&, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::String& v)
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

void dukpp03::PushValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Bound* v)
{
    duk_bool_t result = duk_peval_string(ctx->context(), "sad.p2d.Bound");
    assert(result == 0);
    duk_push_int(ctx->context(), static_cast<int>(v->type()));
    duk_push_number(ctx->context(), v->position());
    duk_new(ctx->context(), 2);
}


void dukpp03::PushValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Line* v)
{
    duk_bool_t result = duk_peval_string(ctx->context(), "sad.p2d.Line");
    assert(result == 0);
    dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, v->p1());
    dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, v->p2());
    duk_new(ctx->context(), 2);
}

void dukpp03::PushValue<sad::p2d::Bound, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Bound v)
{
    dukpp03::PushValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}

void dukpp03::PushValue<sad::p2d::Line, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Line& v)
{
    dukpp03::PushValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}


void dukpp03::PushValue<sad::p2d::CollisionShape*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::CollisionShape* v)
{
    if (v == NULL)
    {
        duk_push_null(ctx->context());
        return;
    }
    if (const_cast<sad::p2d::CollisionShape*>(v)->metaIndex() == sad::p2d::Bound::globalMetaIndex())
    {
        dukpp03::PushValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(ctx, static_cast<const sad::p2d::Bound*>(v));
    }

    if (const_cast<sad::p2d::CollisionShape*>(v)->metaIndex() == sad::p2d::Line::globalMetaIndex())
    {
        dukpp03::PushValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(ctx, static_cast<const sad::p2d::Line*>(v));
    }
}