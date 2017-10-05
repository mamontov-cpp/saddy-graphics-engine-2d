#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <animations/easing/easingfunction.h>

#include <animations/animationsanimation.h>
#include <animations/animationssimplemovement.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }


static void exposeEasingFunction(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::easing::Function>("SadAnimationsEasingFunction");
    c->addObjectConstructor<sad::animations::easing::Function, unsigned int, double, double>("SadAnimationsEasingFunction");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::animations::easing::Function::MajorId), sad::dukpp03::setter::from(&sad::animations::easing::Function::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::animations::easing::Function::MinorId), sad::dukpp03::setter::from(&sad::animations::easing::Function::MinorId));

    c->addMethod("clone", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::clone));
    c->addMethod("eval", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::eval));
    c->addMethod("evalBounded", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::evalBounded));

    c->addMethod("setFunctionTypeAsUnsignedInt", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::setFunctionTypeAsUnsignedInt));
    c->addMethod("setFunctionType", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::setFunctionTypeAsUnsignedInt));
    c->addMethod("functionTypeAsUnsignedInt", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::functionTypeAsUnsignedInt));
    c->addMethod("functionType", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::functionTypeAsUnsignedInt));

    c->addMethod("setOvershootAmplitude", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::setOvershootAmplitude));
    c->addMethod("overshootAmplitude", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::overshootAmplitude));

    c->addMethod("setPeriod", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::setPeriod));
    c->addMethod("period", sad::dukpp03::bind_method::from(&sad::animations::easing::Function::period));
    c->setPrototypeFunction("SadAnimationsEasingFunction");

    ctx->addClassBinding("sad::animations::easing::Function", c);

    PERFORM_AND_ASSERT("sad.animations.easing.Function = SadAnimationsEasingFunction");
}

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
    c->addMethod("easing", sad::dukpp03::bind_method::from(&sad::animations::Animation::easing));
    c->addMethod("setEasing", sad::dukpp03::bind_method::from(&sad::animations::Animation::setEasing));

    ctx->addClassBinding("sad::animations::Animation", c);
}


static void exposeSimpleMovement(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::SimpleMovement>("SadAnimationsSimpleMovement");
    c->addCloneObjectMethodFor<sad::animations::SimpleMovement>();
    c->addMethod("startingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::startingPoint));
    c->addMethod("endingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::endingPoint));
    c->addMethod("setStartingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::setStartingPoint));
    c->addMethod("setEndingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::setEndingPoint));
    c->setPrototypeFunction("SadAnimationsSimpleMovement");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::SimpleMovement", c);
  
    PERFORM_AND_ASSERT("sad.animations.SimpleMovement = SadAnimationsSimpleMovement");
}

void sad::dukpp03::exposeAnimations(sad::dukpp03::Context* ctx)
{
    exposeEasingFunction(ctx);
    exposeAnimation(ctx);
    exposeSimpleMovement(ctx);
}
