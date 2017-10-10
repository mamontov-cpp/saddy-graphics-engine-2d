#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"
#include "dukpp-03/jsanimationcallback.h"

#include <db/dbtable.h>

#include <geometry2d.h>
#include <renderer.h>

#include <animations/animationsanimations.h>

#include <animations/easing/easingfunction.h>

#include <animations/animationsanimation.h>
#include <animations/animationssimplemovement.h>
#include <animations/animationsblinking.h>
#include <animations/animationscolor.h>
#include <animations/animationscamerarotation.h>
#include <animations/animationscamerashaking.h>
#include <animations/animationstexturecoordinatescontinuous.h>
#include <animations/animationstexturecoordinateslist.h>
#include <animations/animationstyping.h>
#include <animations/animationsfontlist.h>
#include <animations/animationsfontsize.h>
#include <animations/animationsoptionlist.h>
#include <animations/animationsresize.h>
#include <animations/animationsrotate.h>
#include <animations/animationscomposite.h>
#include <animations/animationsparallel.h>
#include <animations/animationssequential.h>
#include <animations/animationswaymoving.h>
#include <animations/animationsprocess.h>
#include <animations/animationsinstance.h>
#include <animations/animationswayinstance.h>
#include <animations/animationsgroup.h>

#include <animations/animationsfactory.h>

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
  
    PERFORM_AND_ASSERT("sad.animations.SimpleMovement = SadAnimationsSimpleMovement;");
}


static void exposeBlinking(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Blinking>("SadAnimationsBlinking");
    c->addCloneObjectMethodFor<sad::animations::Blinking>();
    c->addMethod("setFrequency", sad::dukpp03::bind_method::from(&sad::animations::Blinking::setFrequency));
    c->addMethod("frequency", sad::dukpp03::bind_method::from(&sad::animations::Blinking::frequency));
    c->setPrototypeFunction("SadAnimationsBlinking");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Blinking", c);

    PERFORM_AND_ASSERT("sad.animations.Blinking = SadAnimationsBlinking;");
}

static void exposeColor(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Color>("SadAnimationsColor");
    c->addCloneObjectMethodFor<sad::animations::Color>();
    c->addMethod("minColor", sad::dukpp03::bind_method::from(&sad::animations::Color::minColor));
    c->addMethod("setMinColor", sad::dukpp03::bind_method::from(&sad::animations::Color::setMinColor));
    c->addMethod("maxColor", sad::dukpp03::bind_method::from(&sad::animations::Color::maxColor));
    c->addMethod("setMaxColor", sad::dukpp03::bind_method::from(&sad::animations::Color::setMaxColor));
    c->setPrototypeFunction("SadAnimationsColor");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Color", c);

    PERFORM_AND_ASSERT("sad.animations.Color = SadAnimationsColor;");
}

static void exposeCameraRotation(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::CameraRotation>("SadAnimationsCameraRotation");
    c->addCloneObjectMethodFor<sad::animations::CameraRotation>();
    c->addMethod("minAngle", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::minAngle));
    c->addMethod("setMinAngle", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::setMinAngle));
    c->addMethod("maxAngle", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::maxAngle));
    c->addMethod("setMaxAngle", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::setMaxAngle));
    c->addMethod("pivot", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::pivot));
    c->addMethod("setPivot", sad::dukpp03::bind_method::from(&sad::animations::CameraRotation::setPivot));
    c->setPrototypeFunction("SadAnimationsCameraRotation");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::CameraRotation", c);

    PERFORM_AND_ASSERT("sad.animations.CameraRotation = SadAnimationsCameraRotation;");
}


static void exposeCameraShaking(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::CameraShaking>("SadAnimationsCameraShaking");
    c->addCloneObjectMethodFor<sad::animations::CameraShaking>();
    c->addMethod("setOffset", sad::dukpp03::bind_method::from(&sad::animations::CameraShaking::setOffset));
    c->addMethod("offset", sad::dukpp03::bind_method::from(&sad::animations::CameraShaking::offset));
    c->addMethod("setFrequency", sad::dukpp03::bind_method::from(&sad::animations::CameraShaking::setFrequency));
    c->addMethod("frequency", sad::dukpp03::bind_method::from(&sad::animations::CameraShaking::frequency));
    c->setPrototypeFunction("SadAnimationsCameraShaking");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::CameraShaking", c);

    PERFORM_AND_ASSERT("sad.animations.CameraShaking = SadAnimationsCameraShaking;");
}


static void exposeFactory(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Factory>("SadAnimationsFactory");
    c->addMethod("create", sad::dukpp03::bind_method::from(&sad::animations::Factory::create));
    c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::animations::Factory::clear));
    c->addMethod("registeredClasses", sad::dukpp03::bind_method::from(&sad::animations::Factory::registeredClasses));
    c->setPrototypeFunction("SadAnimationsFactory");

    ctx->addClassBinding("sad::animations::Factory", c);

    PERFORM_AND_ASSERT("sad.animations.Factory = SadAnimationsFactory;");
}


static void exposeAnimationsObject(sad::dukpp03::Context* ctx)
{
    PERFORM_AND_ASSERT("SadAnimationsAnimations = function() { }; sad.animations.Animations = SadAnimationsAnimations;");

    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addMethod("add", sad::dukpp03::bind_method::from(&sad::animations::Animations::add));
    c->addMethod("remove", sad::dukpp03::bind_method::from(&sad::animations::Animations::remove));
    c->addMethod("removeByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::removeByName));
    c->addMethod("removeByNameAndType", sad::dukpp03::bind_method::from(&sad::animations::Animations::removeByNameAndType));
    c->addMethod("removeByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::removeByMajorId));
    c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::animations::Animations::clear));
    c->addMethod("insertAt", sad::dukpp03::bind_method::from(&sad::animations::Animations::insertAt));
    c->addMethod("removeAt", sad::dukpp03::bind_method::from(&sad::animations::Animations::removeAt));
    c->addMethod("count", sad::dukpp03::bind_method::from(&sad::animations::Animations::count));
    c->addMethod("list", sad::dukpp03::bind_method::from(&sad::animations::Animations::list));
    c->addMethod("find", sad::dukpp03::bind_method::from(&sad::animations::Animations::find));
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::animations::Process* before, sad::animations::Process* o) = &sad::animations::Animations::insertBefore;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBefore;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBefore;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertBefore;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertBefore", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::db::Object* before, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertBeforeInstanceWithObject", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::animations::Animation* before, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertBeforeInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertBeforeInstanceWithAnimation", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::animations::Process* after, sad::animations::Process* o) = &sad::animations::Animations::insertAfter;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfter;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfter;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertAfter;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertAfter", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::db::Object* after, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithObject;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertAfterInstanceWithObject", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::animations::Animation* after, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type, const sad::String& name, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id, sad::animations::Process* o) = &sad::animations::Animations::insertAfterInstanceWithAnimation;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("insertAfterInstanceWithAnimation", overload);
    }
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        {
            void (sad::animations::Animations::*f)(sad::animations::Process* first, sad::animations::Process* second) = &sad::animations::Animations::swap;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& name1, const sad::String name2) = &sad::animations::Animations::swap;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(const sad::String& type1, const sad::String& name1, const sad::String& type2, const sad::String name2) = &sad::animations::Animations::swap;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        {
            void (sad::animations::Animations::*f)(unsigned long long major_id1, unsigned long long major_id2) = &sad::animations::Animations::swap;
            overload->add(sad::dukpp03::bind_method::from(f));
        }
        c->addMethod("swap", overload);
    }
    c->addMethod("queryProcessesByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesByName));
    c->addMethod("queryProcessesByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesByTypeAndName));
    c->addMethod("queryProcessesByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesByMajorId));
    c->addMethod("queryProcessesRelatedToObject", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToObject));
    c->addMethod("queryProcessesRelatedToObjectByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToObjectByName));
    c->addMethod("queryProcessesRelatedToObjectByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToObjectByTypeAndName));
    c->addMethod("queryProcessesRelatedToObjectByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToObjectByMajorId));
    c->addMethod("queryProcessesRelatedToAnimation", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToAnimation));
    c->addMethod("queryProcessesRelatedToAnimationByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToAnimationByName));
    c->addMethod("queryProcessesRelatedToAnimationByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToAnimationByTypeAndName));
    c->addMethod("queryProcessesRelatedToAnimationByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::queryProcessesRelatedToAnimationByMajorId));
    c->addMethod("stopProcess", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcess));
    c->addMethod("stopProcessByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessByName));
    c->addMethod("stopProcessByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessByTypeAndName));
    c->addMethod("stopProcessByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessByMajorId));
    c->addMethod("stopProcessesRelatedToObject", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToObject));
    c->addMethod("stopProcessesRelatedToObjectWithName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToObjectWithName));
    c->addMethod("stopProcessesRelatedToObjectWithTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToObjectWithTypeAndName));
    c->addMethod("stopProcessesRelatedToObjectWithMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToObjectWithMajorId));
    c->addMethod("stopProcessesRelatedToAnimation", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToAnimation));
    c->addMethod("stopProcessesRelatedToAnimationWithName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToAnimationWithName));
    c->addMethod("stopProcessesRelatedToAnimationWithTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToAnimationWithTypeAndName));
    c->addMethod("stopProcessesRelatedToAnimationWithMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::stopProcessesRelatedToAnimationWithMajorId));
    c->addMethod("countProcesses", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcesses));
    c->addMethod("countProcessesByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesByName));
    c->addMethod("countProcessesByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesByTypeAndName));
    c->addMethod("countProcessesByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesByMajorId));
    c->addMethod("countProcessesRelatedToObject", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToObject));
    c->addMethod("countProcessesRelatedToObjectByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToObjectByName));
    c->addMethod("countProcessesRelatedToObjectByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToObjectByTypeAndName));
    c->addMethod("countProcessesRelatedToObjectByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToObjectByMajorId));
    c->addMethod("countProcessesRelatedToAnimation", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToAnimation));
    c->addMethod("countProcessesRelatedToAnimationByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToAnimationByName));
    c->addMethod("countProcessesRelatedToAnimationByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToAnimationByTypeAndName));
    c->addMethod("countProcessesRelatedToAnimationByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::countProcessesRelatedToAnimationByMajorId));
    c->addMethod("hasProcesses", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcesses));
    c->addMethod("hasProcessesByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesByName));
    c->addMethod("hasProcessesByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesByTypeAndName));
    c->addMethod("hasProcessesByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesByMajorId));
    c->addMethod("hasProcessesRelatedToObject", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToObject));
    c->addMethod("hasProcessesRelatedToObjectByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToObjectByName));
    c->addMethod("hasProcessesRelatedToObjectByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToObjectByTypeAndName));
    c->addMethod("hasProcessesRelatedToObjectByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToObjectByMajorId));
    c->addMethod("hasProcessesRelatedToAnimation", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToAnimation));
    c->addMethod("hasProcessesRelatedToAnimationByName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToAnimationByName));
    c->addMethod("hasProcessesRelatedToAnimationByTypeAndName", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToAnimationByTypeAndName));
    c->addMethod("hasProcessesRelatedToAnimationByMajorId", sad::dukpp03::bind_method::from(&sad::animations::Animations::hasProcessesRelatedToAnimationByMajorId));
    c->setPrototypeFunction("SadAnimationsAnimations");

    ctx->addClassBinding("sad::animations::Animations", c);
}


static void exposeTextureCoordinatesContinuous(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::TextureCoordinatesContinuous>("SadAnimationsTextureCoordinatesContinuous");
    c->addCloneObjectMethodFor<sad::animations::TextureCoordinatesContinuous>();
    c->addMethod("setStartRect", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesContinuous::setStartRect));
    c->addMethod("startRect", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesContinuous::startRect));
    c->addMethod("setEndRect", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesContinuous::setEndRect));
    c->addMethod("endRect", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesContinuous::endRect));
    c->setPrototypeFunction("SadAnimationsTextureCoordinatesContinuous");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::TextureCoordinatesContinuous", c);

    PERFORM_AND_ASSERT("sad.animations.TextureCoordinatesContinuous = SadAnimationsTextureCoordinatesContinuous;");
}


static void exposeTextureCoordinatesList(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::TextureCoordinatesList>("SadAnimationsTextureCoordinatesList");
    c->addCloneObjectMethodFor<sad::animations::TextureCoordinatesList>();
    c->addMethod("setList", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesList::setList));
    c->addMethod("list", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesList::list));
    c->addMethod("setTreeName", sad::dukpp03::bind_method::from(&sad::animations::TextureCoordinatesList::setTreeName));    
    std::function<void(sad::animations::TextureCoordinatesList*, sad::dukpp03::Renderer*, const sad::String&)> lambda = [](sad::animations::TextureCoordinatesList* a, sad::dukpp03::Renderer* r, const sad::String& s) {
            a->setTreeName(r, s);
    };
    c->addMethod("setTreeName", sad::dukpp03::bind_lambda::from(lambda));
    c->setPrototypeFunction("SadAnimationsTextureCoordinatesList");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::TextureCoordinatesList", c);

    PERFORM_AND_ASSERT("sad.animations.TextureCoordinatesList = SadAnimationsTextureCoordinatesList;");
}

static void exposeFontList(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::FontList>("SadAnimationsFontList");
    c->addCloneObjectMethodFor<sad::animations::FontList>();
    c->addMethod("setFonts", sad::dukpp03::bind_method::from(&sad::animations::FontList::setFonts));
    c->addMethod("fonts", sad::dukpp03::bind_method::from(&sad::animations::FontList::fonts));
    c->setPrototypeFunction("SadAnimationsFontList");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::FontList", c);

    PERFORM_AND_ASSERT("sad.animations.FontList = SadAnimationsFontList;");
}


static void exposeOptionList(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::OptionList>("SadAnimationsOptionList");
    c->addCloneObjectMethodFor<sad::animations::OptionList>();
    c->addMethod("setList", sad::dukpp03::bind_method::from(&sad::animations::OptionList::setList));
    c->addMethod("list", sad::dukpp03::bind_method::from(&sad::animations::OptionList::list));
    c->setPrototypeFunction("SadAnimationsOptionList");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::OptionList", c);

    PERFORM_AND_ASSERT("sad.animations.OptionList = SadAnimationsOptionList;");
}

static void exposeTyping(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Typing>("SadAnimationsTyping");
    c->addCloneObjectMethodFor<sad::animations::Typing>();
    c->setPrototypeFunction("SadAnimationsTyping");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Typing", c);

    PERFORM_AND_ASSERT("sad.animations.Typing = SadAnimationsTyping;");
}

static void exposeResize(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Resize>("SadAnimationsResize");
    c->addCloneObjectMethodFor<sad::animations::Resize>();
    void (sad::animations::Resize::*f1)(const sad::Point2D& v) = &sad::animations::Resize::setStartSize;
    void (sad::animations::Resize::*f2)(const sad::Size2D& v) = &sad::animations::Resize::setStartSize;
    c->addMethod("setStartSize", sad::dukpp03::bind_method::from(f1));
    c->addMethod("setStartSize", sad::dukpp03::bind_method::from(f2));
    f1 = &sad::animations::Resize::setEndSize;
    f2 = &sad::animations::Resize::setEndSize;
    c->addMethod("setEndSize", sad::dukpp03::bind_method::from(f1));
    c->addMethod("setEndSize", sad::dukpp03::bind_method::from(f2));

    c->addMethod("startSize", sad::dukpp03::bind_method::from(&sad::animations::Resize::startSize));
    c->addMethod("endSize", sad::dukpp03::bind_method::from(&sad::animations::Resize::endSize));

    c->setPrototypeFunction("SadAnimationsResize");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Resize", c);

    PERFORM_AND_ASSERT("sad.animations.Resize = SadAnimationsResize;");
}

static void exposeRotate(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Rotate>("SadAnimationsRotate");
    c->addCloneObjectMethodFor<sad::animations::Rotate>();
    c->addMethod("setMinAngle", sad::dukpp03::bind_method::from(&sad::animations::Rotate::setMinAngle));
    c->addMethod("minAngle", sad::dukpp03::bind_method::from(&sad::animations::Rotate::minAngle));
    c->addMethod("setMaxAngle", sad::dukpp03::bind_method::from(&sad::animations::Rotate::setMaxAngle));
    c->addMethod("maxAngle", sad::dukpp03::bind_method::from(&sad::animations::Rotate::maxAngle));
    c->setPrototypeFunction("SadAnimationsRotate");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Rotate", c);

    PERFORM_AND_ASSERT("sad.animations.Rotate = SadAnimationsRotate;");
}

static void exposeFontSize(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::FontSize>("SadAnimationsFontSize");
    c->addCloneObjectMethodFor<sad::animations::FontSize>();
    c->addMethod("setMinSize", sad::dukpp03::bind_method::from(&sad::animations::FontSize::setMinSize));
    c->addMethod("minSize", sad::dukpp03::bind_method::from(&sad::animations::FontSize::minSize));
    c->addMethod("setMaxSize", sad::dukpp03::bind_method::from(&sad::animations::FontSize::setMaxSize));
    c->addMethod("maxSize", sad::dukpp03::bind_method::from(&sad::animations::FontSize::maxSize));
    c->setPrototypeFunction("SadAnimationsFontSize");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::FontSize", c);

    PERFORM_AND_ASSERT("sad.animations.FontSize = SadAnimationsFontSize;");
}


static void exposeComposite(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addMethod("setTable", sad::dukpp03::bind_method::from(&sad::animations::Composite::setTable));
    void (sad::animations::Composite::*_add1)(unsigned long long majorid) = &sad::animations::Composite::add;
    void (sad::animations::Composite::*_add2)(sad::animations::Animation* o) = &sad::animations::Composite::add;
    c->addMethod("add", sad::dukpp03::bind_method::from(_add1));
    c->addMethod("add", sad::dukpp03::bind_method::from(_add2));
    void (sad::animations::Composite::*_insert1)(unsigned long long majorid, int pos) = &sad::animations::Composite::insert;
    void (sad::animations::Composite::*_insert2)(sad::animations::Animation* o, int pos) = &sad::animations::Composite::insert;
    c->addMethod("insert", sad::dukpp03::bind_method::from(_insert1));
    c->addMethod("insert", sad::dukpp03::bind_method::from(_insert2));
    
    c->addMethod("swap", sad::dukpp03::bind_method::from(&sad::animations::Composite::swap));
    c->addMethod("remove", sad::dukpp03::bind_method::from(&sad::animations::Composite::remove));
    c->addMethod("animation", sad::dukpp03::bind_method::from(&sad::animations::Composite::animation));
    c->addMethod("size", sad::dukpp03::bind_method::from(&sad::animations::Composite::size));
    c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::animations::Composite::clear));
    c->addMethod("setAnimationsNames", sad::dukpp03::bind_method::from(&sad::animations::Composite::setAnimationsNames));
    c->addMethod("animationNames", sad::dukpp03::bind_method::from(&sad::animations::Composite::animationNames));
    c->addMethod("setAnimationsMajorId", sad::dukpp03::bind_method::from(&sad::animations::Composite::setAnimationsMajorId));
    c->addMethod("animationMajorIds", sad::dukpp03::bind_method::from(&sad::animations::Composite::animationMajorIds));
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::Composite", c);
}

static void exposeParallel(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Parallel>("SadAnimationsParallel");
    c->addCloneObjectMethodFor<sad::animations::Parallel>();
    c->setPrototypeFunction("SadAnimationsParallel");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Composite"));

    ctx->addClassBinding("sad::animations::Parallel", c);

    PERFORM_AND_ASSERT("sad.animations.Parallel = SadAnimationsParallel;");
}

static void exposeSequential(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Sequential>("SadAnimationsSequential");
    c->addCloneObjectMethodFor<sad::animations::Sequential>();
    c->setPrototypeFunction("SadAnimationsSequential");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Composite"));

    ctx->addClassBinding("sad::animations::Sequential", c);

    PERFORM_AND_ASSERT("sad.animations.Sequential = SadAnimationsSequential;");
}

static void exposeWayMoving(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::WayMoving>("SadAnimationsWayMoving");
    c->addCloneObjectMethodFor<sad::animations::WayMoving>();
    c->addMethod("setTable", sad::dukpp03::bind_method::from(&sad::animations::WayMoving::setTable));
    c->addMethod("setWay", sad::dukpp03::bind_method::from(&sad::animations::WayMoving::setWay));
    c->addMethod("way", sad::dukpp03::bind_method::from(&sad::animations::WayMoving::way));
    c->addMethod("setWayObjectId", sad::dukpp03::bind_method::from(&sad::animations::WayMoving::setWayObjectId));
    c->addMethod("wayObjectId", sad::dukpp03::bind_method::from(&sad::animations::WayMoving::wayObjectId));
    c->setPrototypeFunction("SadAnimationsWayMoving");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Animation"));

    ctx->addClassBinding("sad::animations::WayMoving", c);

    PERFORM_AND_ASSERT("sad.animations.WayMoving = SadAnimationsWayMoving;");
}

template< 
    typename _Object
>
static void __addMethodsToProcess(
    sad::dukpp03::ClassBinding* c, 
    const sad::String& nameForQuery,
    const sad::String& nameForStop,
    bool (sad::animations::Process::*query)(const std::function<bool(_Object)>& f),
    void (sad::animations::Process::*stop)(const std::function<bool(_Object)>& f, sad::animations::Animations* a)
)
{
    {
        std::function<bool(sad::animations::Process*, _Object)> lambda1 = [query](sad::animations::Process* p, _Object o) {
            return (p->*query)([o](_Object obj)  -> bool {
                return obj == o;
            });
        };
        c->addMethod(nameForQuery, sad::dukpp03::bind_lambda::from(lambda1));
        std::function<bool(sad::animations::Process*, const sad::String&)> lambda2 = [query](sad::animations::Process* p, const sad::String& name) {
            return (p->*query)([name](_Object obj)  -> bool {
                if (obj)
                {
                    return obj->objectName() == name;
                }
                return false;
            });
        };
        c->addMethod(nameForQuery, sad::dukpp03::bind_lambda::from(lambda2));
        std::function<bool(sad::animations::Process*, const sad::String&, const sad::String&)> lambda3 = [query](sad::animations::Process* p, const sad::String& type, const sad::String& name) {
            return (p->*query)([type, name](_Object obj)  -> bool {
                if (obj)
                {
                    if (obj->isInstanceOf(type))
                    {
                        return obj->objectName() == name;
                    }
                }
                return false;
            });
        };
        c->addMethod(nameForQuery, sad::dukpp03::bind_lambda::from(lambda3));
        std::function<bool(sad::animations::Process*, unsigned long long)> lambda4 = [query](sad::animations::Process* p, unsigned long long major_id) {
            return (p->*query)([major_id](_Object obj)  -> bool {
                if (obj)
                {
                    return obj->MajorId = major_id;
                }
                return false;
            });
        };
        c->addMethod(nameForQuery, sad::dukpp03::bind_lambda::from(lambda4));
    }
    {
        std::function<void(sad::animations::Process*, sad::animations::Animations*, _Object)> lambda1 = [stop](sad::animations::Process* p, sad::animations::Animations* a, _Object o) {
            (p->*stop)([o](_Object obj)  -> bool {
                return obj == o;
            }, a);
        };
        c->addMethod(nameForStop, sad::dukpp03::bind_lambda::from(lambda1));
        std::function<void(sad::animations::Process*, sad::animations::Animations*, const sad::String&)> lambda2 = [stop](sad::animations::Process* p, sad::animations::Animations* a, const sad::String& name) {
            (p->*stop)([name](_Object obj)  -> bool {
                if (obj)
                {
                    return obj->objectName() == name;
                }
                return false;
            }, a);
        };
        c->addMethod(nameForStop, sad::dukpp03::bind_lambda::from(lambda2));
        std::function<void(sad::animations::Process*, sad::animations::Animations*, const sad::String&, const sad::String&)> lambda3 = [stop](sad::animations::Process* p, sad::animations::Animations* a, const sad::String& type, const sad::String& name) {
            (p->*stop)([type, name](_Object obj)  -> bool {
                if (obj)
                {
                    if (obj->isInstanceOf(type))
                    {
                        return obj->objectName() == name;
                    }
                }
                return false;
            }, a);
        };
        c->addMethod(nameForStop, sad::dukpp03::bind_lambda::from(lambda3));
        std::function<void(sad::animations::Process*, sad::animations::Animations*, unsigned long long)> lambda4 = [stop](sad::animations::Process* p, sad::animations::Animations* a, unsigned long long major_id) {
            (p->*stop)([major_id](_Object obj)  -> bool {
                if (obj)
                {
                    return obj->MajorId = major_id;
                }
                return false;
            }, a);
        };
        c->addMethod(nameForStop, sad::dukpp03::bind_lambda::from(lambda4));
    }
}

static void exposeProcess(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addMethod("restart", sad::dukpp03::bind_method::from(&sad::animations::Process::restart));
    c->addMethod("clearFinished", sad::dukpp03::bind_method::from(&sad::animations::Process::clearFinished));
    c->addMethod("finished", sad::dukpp03::bind_method::from(&sad::animations::Process::finished));
    c->addMethod("process", sad::dukpp03::bind_method::from(&sad::animations::Process::process));
    c->addMethod("pause", sad::dukpp03::bind_method::from(&sad::animations::Process::pause));
    c->addMethod("resume", sad::dukpp03::bind_method::from(&sad::animations::Process::resume));
    c->addMethod("cancel", sad::dukpp03::bind_method::from(&sad::animations::Process::cancel));

    __addMethodsToProcess<sad::db::Object*>(c, 
        "isRelatedToMatchedObject", 
        "stopInstancesRelatedToMatchedObject", 
        &sad::animations::Process::isRelatedToMatchedObject,
        &sad::animations::Process::stopInstancesRelatedToMatchedObject
     );
    __addMethodsToProcess<sad::animations::Animation*>(c,
        "isRelatedToMatchedAnimation",
        "stopInstancesRelatedToMatchedAnimation",
        &sad::animations::Process::isRelatedToMatchedAnimation,
        &sad::animations::Process::stopInstancesRelatedToMatchedAnimation
     );

    __addMethodsToProcess<sad::animations::Process*>(c,
        "isRelatedToMatchedProcess",
        "stopInstancesRelatedToMatchedProcess",
        &sad::animations::Process::isRelatedToMatchedProcess,
        &sad::animations::Process::stopInstancesRelatedToMatchedProcess
    );
    

    ctx->addClassBinding("sad::animations::Process", c);
}

static void exposeInstance(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Instance>("SadAnimationsInstance");
    c->addCloneObjectMethodFor<sad::animations::Instance>();
    c->addMethod("setTreeName", sad::dukpp03::bind_method::from(&sad::animations::Instance::setTreeName));
    c->addMethod("setTable", sad::dukpp03::bind_method::from(&sad::animations::Instance::setTable));
    c->addMethod("serializableName", sad::dukpp03::bind_method::from(&sad::animations::Instance::serializableName));
    c->addMethod("setAnimation", sad::dukpp03::bind_method::from(&sad::animations::Instance::setAnimation));
    c->addMethod("animation", sad::dukpp03::bind_method::from(&sad::animations::Instance::animation));
    c->addMethod("setObject", sad::dukpp03::bind_method::from(&sad::animations::Instance::setObject));
    c->addMethod("object", sad::dukpp03::bind_method::from(&sad::animations::Instance::object));
    c->addMethod("setAnimationName", sad::dukpp03::bind_method::from(&sad::animations::Instance::setAnimationName));
    c->addMethod("animationName", sad::dukpp03::bind_method::from(&sad::animations::Instance::animationName));
    c->addMethod("setAnimationMajorId", sad::dukpp03::bind_method::from(&sad::animations::Instance::setAnimationMajorId));
    c->addMethod("animationMajorId", sad::dukpp03::bind_method::from(&sad::animations::Instance::animationMajorId));
    c->addMethod("setStartTime", sad::dukpp03::bind_method::from(&sad::animations::Instance::setStartTime));
    c->addMethod("startTime", sad::dukpp03::bind_method::from(&sad::animations::Instance::startTime));
    c->addMethod("setObjectId", sad::dukpp03::bind_method::from(&sad::animations::Instance::setObjectId));
    c->addMethod("objectId", sad::dukpp03::bind_method::from(&sad::animations::Instance::objectId));
    c->addMethod("restart", sad::dukpp03::bind_method::from(&sad::animations::Instance::restart));
    c->addMethod("clearFinished", sad::dukpp03::bind_method::from(&sad::animations::Instance::clearFinished));
    c->addMethod("finished", sad::dukpp03::bind_method::from(&sad::animations::Instance::finished));
    c->addMethod("pause", sad::dukpp03::bind_method::from(&sad::animations::Instance::pause));
    c->addMethod("resume", sad::dukpp03::bind_method::from(&sad::animations::Instance::resume));
    c->addMethod("cancel", sad::dukpp03::bind_method::from(&sad::animations::Instance::cancel));
    c->addMethod("markAsValid", sad::dukpp03::bind_method::from(&sad::animations::Instance::markAsValid));

    std::function<sad::dukpp03::JSAnimationCallback*(sad::animations::Instance*, sad::dukpp03::Context*, sad::dukpp03::CompiledFunction)> add_on_start = [](sad::animations::Instance* i,  sad::dukpp03::Context* context, sad::dukpp03::CompiledFunction f) {
        sad::dukpp03::JSAnimationCallback* cb = new sad::dukpp03::JSAnimationCallback(context, f);
        i->addCallbackOnStart(cb);
        return cb;
    };
    c->addMethod("addCallbackOnStart", sad::dukpp03::bind_lambda::from(add_on_start));
    c->addMethod("start", sad::dukpp03::bind_lambda::from(add_on_start));

    std::function<sad::dukpp03::JSAnimationCallback*(sad::animations::Instance*, sad::dukpp03::Context*, sad::dukpp03::CompiledFunction)> add_on_end = [](sad::animations::Instance* i, sad::dukpp03::Context* context, sad::dukpp03::CompiledFunction f) {
        sad::dukpp03::JSAnimationCallback* cb = new sad::dukpp03::JSAnimationCallback(context, f);
        i->addCallbackOnEnd(new sad::dukpp03::JSAnimationCallback(context, f));
        return cb;
    };
    c->addMethod("addCallbackOnEnd", sad::dukpp03::bind_lambda::from(add_on_end));
    c->addMethod("end", sad::dukpp03::bind_lambda::from(add_on_end));

    std::function<void(sad::animations::Instance*, sad::dukpp03::JSAnimationCallback*)> remove_on_start = [](sad::animations::Instance* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallbackOnStart(cb);
    };
    c->addMethod("removeCallbackOnStart", sad::dukpp03::bind_lambda::from(remove_on_start));
    std::function<void(sad::animations::Instance*, sad::dukpp03::JSAnimationCallback*)> remove_on_end = [](sad::animations::Instance* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallbackOnEnd(cb);
    };
    c->addMethod("removeCallbackOnEnd", sad::dukpp03::bind_lambda::from(remove_on_end));
    std::function<void(sad::animations::Instance*, sad::dukpp03::JSAnimationCallback*)> remove_cb = [](sad::animations::Instance* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallback(cb);
    };
    c->addMethod("removeCallback", sad::dukpp03::bind_lambda::from(remove_cb));
    c->addMethod("clearCallbacksOnStart", sad::dukpp03::bind_method::from(&sad::animations::Instance::clearCallbacksOnStart));
    c->addMethod("clearCallbacksOnEnd", sad::dukpp03::bind_method::from(&sad::animations::Instance::clearCallbacksOnEnd));
    c->addMethod("clearCallbacks", sad::dukpp03::bind_method::from(&sad::animations::Instance::clearCallbacks));

    c->setPrototypeFunction("SadAnimationsInstance");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Process"));

    ctx->addClassBinding("sad::animations::Instance", c);

    PERFORM_AND_ASSERT("sad.animations.Instance = SadAnimationsInstance;");
}

static void exposeWayInstance(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::WayInstance>("SadAnimationsWayInstance");
    c->addCloneObjectMethodFor<sad::animations::WayInstance>();
    c->addMethod("setWayMajorId", sad::dukpp03::bind_method::from(&sad::animations::WayInstance::setWayMajorId));
    c->addMethod("wayMajorId", sad::dukpp03::bind_method::from(&sad::animations::WayInstance::wayMajorId));
    c->addMethod("setWay", sad::dukpp03::bind_method::from(&sad::animations::WayInstance::setWay));
    c->addMethod("way", sad::dukpp03::bind_method::from(&sad::animations::WayInstance::way));

    c->setPrototypeFunction("SadAnimationsWayInstance");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Instance"));

    ctx->addClassBinding("sad::animations::WayInstance", c);

    PERFORM_AND_ASSERT("sad.animations.WayInstance = SadAnimationsWayInstance;");
}



static void exposeGroup(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::Group>("SadAnimationsGroup");
    c->addCloneObjectMethodFor<sad::animations::Group>();
    c->addMethod("isSequential", sad::dukpp03::bind_method::from(&sad::animations::Group::isSequential));
    c->addMethod("toggleIsSequential", sad::dukpp03::bind_method::from(&sad::animations::Group::toggleIsSequential));
    c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::animations::Group::clear));
    c->addMethod("add", sad::dukpp03::bind_method::from(&sad::animations::Group::add));
    c->addMethod("insert", sad::dukpp03::bind_method::from(&sad::animations::Group::insert));
    c->addMethod("remove", sad::dukpp03::bind_method::from(&sad::animations::Group::remove));
    c->addMethod("count", sad::dukpp03::bind_method::from(&sad::animations::Group::count));
    c->addMethod("get", sad::dukpp03::bind_method::from(&sad::animations::Group::get));
    c->addMethod("getMajorId", sad::dukpp03::bind_method::from(&sad::animations::Group::getMajorId));
    c->addMethod("setTable", sad::dukpp03::bind_method::from(&sad::animations::Group::setTable));
    c->addMethod("serializableName", sad::dukpp03::bind_method::from(&sad::animations::Group::serializableName));
    c->addMethod("setInstances", sad::dukpp03::bind_method::from(&sad::animations::Group::setInstances));
    c->addMethod("instances", sad::dukpp03::bind_method::from(&sad::animations::Group::instances));
    c->addMethod("findInstance", sad::dukpp03::bind_method::from(&sad::animations::Group::findInstance));
    c->addMethod("setLooped", sad::dukpp03::bind_method::from(&sad::animations::Group::setLooped));
    c->addMethod("looped", sad::dukpp03::bind_method::from(&sad::animations::Group::looped));
    c->addMethod("restart", sad::dukpp03::bind_method::from(&sad::animations::Group::restart));
    c->addMethod("clearFinished", sad::dukpp03::bind_method::from(&sad::animations::Group::clearFinished));
    c->addMethod("finished", sad::dukpp03::bind_method::from(&sad::animations::Group::finished));
    c->addMethod("process", sad::dukpp03::bind_method::from(&sad::animations::Group::process));
    c->addMethod("pause", sad::dukpp03::bind_method::from(&sad::animations::Group::pause));
    c->addMethod("resume", sad::dukpp03::bind_method::from(&sad::animations::Group::resume));
    c->addMethod("cancel", sad::dukpp03::bind_method::from(&sad::animations::Group::cancel));

    std::function<sad::dukpp03::JSAnimationCallback*(sad::animations::Group*, sad::dukpp03::Context*, sad::dukpp03::CompiledFunction)> add_on_start = [](sad::animations::Group* i, sad::dukpp03::Context* context, sad::dukpp03::CompiledFunction f) {
        sad::dukpp03::JSAnimationCallback* cb = new sad::dukpp03::JSAnimationCallback(context, f);
        i->addCallbackOnStart(cb);
        return cb;
    };
    c->addMethod("addCallbackOnStart", sad::dukpp03::bind_lambda::from(add_on_start));
    c->addMethod("start", sad::dukpp03::bind_lambda::from(add_on_start));

    std::function<sad::dukpp03::JSAnimationCallback*(sad::animations::Group*, sad::dukpp03::Context*, sad::dukpp03::CompiledFunction)> add_on_end = [](sad::animations::Group* i, sad::dukpp03::Context* context, sad::dukpp03::CompiledFunction f) {
        sad::dukpp03::JSAnimationCallback* cb = new sad::dukpp03::JSAnimationCallback(context, f);
        i->addCallbackOnEnd(new sad::dukpp03::JSAnimationCallback(context, f));
        return cb;
    };
    c->addMethod("addCallbackOnEnd", sad::dukpp03::bind_lambda::from(add_on_end));
    c->addMethod("end", sad::dukpp03::bind_lambda::from(add_on_end));

    std::function<void(sad::animations::Group*, sad::dukpp03::JSAnimationCallback*)> remove_on_start = [](sad::animations::Group* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallbackOnStart(cb);
    };
    c->addMethod("removeCallbackOnStart", sad::dukpp03::bind_lambda::from(remove_on_start));
    std::function<void(sad::animations::Group*, sad::dukpp03::JSAnimationCallback*)> remove_on_end = [](sad::animations::Group* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallbackOnEnd(cb);
    };
    c->addMethod("removeCallbackOnEnd", sad::dukpp03::bind_lambda::from(remove_on_end));
    std::function<void(sad::animations::Group*, sad::dukpp03::JSAnimationCallback*)> remove_cb = [](sad::animations::Group* i, sad::dukpp03::JSAnimationCallback* cb) {
        i->removeCallback(cb);
    };
    c->addMethod("removeCallback", sad::dukpp03::bind_lambda::from(remove_cb));
    c->addMethod("clearCallbacksOnStart", sad::dukpp03::bind_method::from(&sad::animations::Group::clearCallbacksOnStart));
    c->addMethod("clearCallbacksOnEnd", sad::dukpp03::bind_method::from(&sad::animations::Group::clearCallbacksOnEnd));
    c->addMethod("clearCallbacks", sad::dukpp03::bind_method::from(&sad::animations::Group::clearCallbacks));

    c->setPrototypeFunction("SadAnimationsGroup");
    c->addParentBinding(ctx->getClassBinding("sad::animations::Process"));

    ctx->addClassBinding("sad::animations::Group", c);

    PERFORM_AND_ASSERT("sad.animations.Group = SadAnimationsGroup;");
}


void sad::dukpp03::exposeAnimations(sad::dukpp03::Context* ctx)
{
    exposeEasingFunction(ctx);
    exposeAnimation(ctx);
    exposeSimpleMovement(ctx);
    exposeColor(ctx);
    exposeCameraRotation(ctx);
    exposeCameraShaking(ctx);
    exposeBlinking(ctx);
    exposeFontList(ctx);
    exposeFontSize(ctx);
    exposeOptionList(ctx);
    exposeTyping(ctx);
    exposeTextureCoordinatesList(ctx);
    exposeTextureCoordinatesContinuous(ctx);
    exposeFactory(ctx);
    exposeResize(ctx);
    exposeRotate(ctx);
    exposeComposite(ctx);
    exposeParallel(ctx);
    exposeSequential(ctx);
    exposeWayMoving(ctx);
    exposeProcess(ctx);
    exposeInstance(ctx);
    exposeWayInstance(ctx);
    exposeGroup(ctx);

    exposeAnimationsObject(ctx);
}
