#include "dukpp-03/context.h"
#include "dukpp-03/pushvariant.h"
#include "p2d/body.h"

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
            parent->unregisterVariant(v);
            delete v;
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

void dukpp03::PushValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Rectangle* v)
{
    duk_bool_t result = duk_peval_string(ctx->context(), "sad.p2d.Rectangle");
    assert(result == 0);
    dukpp03::PushValue<sad::Rect2D, sad::dukpp03::BasicContext>::perform(ctx, v->rect());
    duk_new(ctx->context(), 1);
}

void dukpp03::PushValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Circle* v)
{
    duk_bool_t result = duk_peval_string(ctx->context(), "sad.p2d.Circle");
    assert(result == 0);
    dukpp03::PushValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, v->centerRef());
    dukpp03::PushValue<double, sad::dukpp03::BasicContext>::perform(ctx, v->radius());
    duk_new(ctx->context(), 2);
}


void dukpp03::PushValue<sad::p2d::Bound, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Bound& v)
{
    dukpp03::PushValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}

void dukpp03::PushValue<sad::p2d::Line, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Line& v)
{
    dukpp03::PushValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}


void dukpp03::PushValue<sad::p2d::Rectangle, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Rectangle& v)
{
    dukpp03::PushValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}

void dukpp03::PushValue<sad::p2d::Circle, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::Circle& v)
{
    dukpp03::PushValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>::perform(ctx, &v);
}



void dukpp03::PushValue<sad::p2d::CollisionShape*, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::CollisionShape* v)
{
    if (v == nullptr)
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

    if (const_cast<sad::p2d::CollisionShape*>(v)->metaIndex() == sad::p2d::Rectangle::globalMetaIndex())
    {
        dukpp03::PushValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>::perform(ctx, static_cast<const sad::p2d::Rectangle*>(v));
    }

    if (const_cast<sad::p2d::CollisionShape*>(v)->metaIndex() == sad::p2d::Circle::globalMetaIndex())
    {
        dukpp03::PushValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>::perform(ctx, static_cast<const sad::p2d::Circle*>(v));
    }
}


void dukpp03::PushValue<sad::p2d::BasicCollisionEvent, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::p2d::BasicCollisionEvent& v)
{
    sad::Triplet<sad::p2d::Body*, sad::p2d::Body*, double> tr(v.m_object_1, v.m_object_2, v.m_time);
    dukpp03::PushValue<sad::Triplet<sad::p2d::Body*, sad::p2d::Body*, double>, sad::dukpp03::BasicContext>::perform(ctx, tr);
}

void dukpp03::PushValue<sad::layouts::HorizontalAlignment, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::layouts::HorizontalAlignment& v)
{
    dukpp03::PushValue<unsigned  int, sad::dukpp03::BasicContext>::perform(ctx, static_cast<unsigned int>(v));
}

void dukpp03::PushValue<sad::layouts::VerticalAlignment, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::layouts::VerticalAlignment& v)
{
    dukpp03::PushValue<unsigned  int, sad::dukpp03::BasicContext>::perform(ctx, static_cast<unsigned int>(v));
}

void dukpp03::PushValue<sad::layouts::StackingType, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::layouts::StackingType& v)
{
    dukpp03::PushValue<unsigned  int, sad::dukpp03::BasicContext>::perform(ctx, static_cast<unsigned int>(v));
}

void dukpp03::PushValue<sad::layouts::Unit, sad::dukpp03::BasicContext>::perform(sad::dukpp03::BasicContext* ctx, const sad::layouts::Unit& v)
{
    dukpp03::PushValue<unsigned  int, sad::dukpp03::BasicContext>::perform(ctx, static_cast<unsigned int>(v));
}
