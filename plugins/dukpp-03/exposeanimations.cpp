#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <animations/animationsanimation.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }

static void exposeAnimation(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::animations::Animation::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::animations::Animation::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::animations::Animation::MajorId), sad::dukpp03::setter::from(&sad::animations::Animation::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::animations::Animation::MinorId), sad::dukpp03::setter::from(&sad::animations::Animation::MinorId));

    c->addMethod("setLooped", sad::dukpp03::bind_method::from(&sad::animations::Animation::setLooped));
    c->addMethod("looped", sad::dukpp03::bind_method::from(&sad::animations::Animation::looped));
    c->addMethod("setTime", sad::dukpp03::bind_method::from(&sad::animations::Animation::setTime));
    c->addMethod("time", sad::dukpp03::bind_method::from(&sad::animations::Animation::time));
    c->addMethod("applicableTo", sad::dukpp03::bind_method::from(&sad::animations::Animation::applicableTo));
    c->addMethod("valid", sad::dukpp03::bind_method::from(&sad::animations::Animation::valid));
    // TODO: Somehow, easing should be set too...
    ctx->addClassBinding("sad::animations::Animation", c);
}


void sad::dukpp03::exposeAnimations(sad::dukpp03::Context* ctx)
{
    PERFORM_AND_ASSERT(
        "sad.animations = {};"
    );

    exposeAnimation(ctx);
}
