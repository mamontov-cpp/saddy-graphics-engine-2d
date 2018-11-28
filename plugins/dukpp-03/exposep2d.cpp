#include "dukpp-03/context.h"
#include "dukpp-03/jsmovementlistener.h"
#include "dukpp-03/jscollisionhandler.h"
#include "dukpp-03/renderer.h"

#include <geometry2d.h>
#include <fuzzyequal.h>
#include <renderer.h>

#include <pipeline/pipeline.h>

#include <p2d/axle.h>
#include <p2d/circle.h>
#include <p2d/line.h>
#include <p2d/rectangle.h>
#include <p2d/collisiontest.h>
#include <p2d/vector.h>
#include <p2d/infiniteline.h>
#include <p2d/body.h>
#include <p2d/force.h>
#include <p2d/angularforce.h>
#include <p2d/elasticforce.h>
#include <p2d/weight.h>
#include <p2d/world.h>
#include <p2d/worldsteptask.h>
#include <p2d/bouncesolver.h>
#include <p2d/simplecollisiondetector.h>
#include <p2d/multisamplingcollisiondetector.h>
#include <p2d/broadcollisiondetector.h>
#include <p2d/collisionevent.h>
#include <p2d/walls.h>
#include <p2d/app/way.h>

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }

static sad::Rect2D ___moveBy(const sad::Point2D & dp, sad::Rect2D & r)
{
    sad::Rect2D result = r;
    sad::moveBy(dp, result);
    return result;
}

static sad::Rect2D __rotateRect(const sad::Rect2D & r, float angle)
{
    sad::Rect2D result = r;
    sad::rotate(result, angle);
    return result;
}

static sad::Vector2D __rotateVector(const sad::Vector2D& r, float angle)
{
    sad::Vector2D result = r;
    sad::rotate(result, angle);
    return result;
}

static bool __collidesR2R(const sad::Rect2D& r1, const sad::Rect2D& r2)
{
    sad::p2d::Rectangle r1x, r2x;
    r1x.setRect(r1);
    r2x.setRect(r2);
    sad::p2d::CollisionTest test1;
    return test1.invoke(&r1x, &r2x);
}

static bool __collidesC2C(const sad::Point2D& c1, double ce1, const sad::Point2D& c2, double ce2)
{
    sad::p2d::Circle c1x, c2x;
    c1x.setCenter(c1);
    c1x.setRadius(ce1);
    c2x.setCenter(c2);
    c2x.setRadius(ce2);
    sad::p2d::CollisionTest test1;
    return test1.invoke(&c1x, &c2x);
}


static bool __collidesL2L(const sad::Point2D& p11, const sad::Point2D& p12, const sad::Point2D& p21, const sad::Point2D& p22)
{
    sad::p2d::Line l1x, l2x;
    l1x.setCutter(sad::p2d::Cutter2D(p11, p12));
    l2x.setCutter(sad::p2d::Cutter2D(p21, p22));
    sad::p2d::CollisionTest test1;
    return test1.invoke(&l1x, &l2x);
}

static sad::p2d::Vector __ortho(const sad::p2d::Vector& v, int i)
{
    if ((i < 0) || (i > 1))
    {
        return v;
    }
    return sad::p2d::ortho(v, static_cast<sad::p2d::OrthoVectorIndex>(i));
}

static bool __isRect2D(const sad::Rect2D& p) {
    return true;
}

static sad::p2d::Body inner;

static sad::p2d::Vector __tangentialForceValue(sad::p2d::Force<sad::p2d::Vector>* f)
{
    return f->value(&inner);
}


static double __angularForceValue(sad::p2d::Force<double>* f)
{
    return f->value(&inner);
}

static void exposeForcePoint2D(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::Force<sad::p2d::Vector> >("SadP2DTangentialForce");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::Force<sad::p2d::Vector>::MajorId), sad::dukpp03::setter::from(&sad::p2d::Force<sad::p2d::Vector>::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::Force<sad::p2d::Vector>::MinorId), sad::dukpp03::setter::from(&sad::p2d::Force<sad::p2d::Vector>::MinorId));

    c->addMethod("_value", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::value));
    c->addMethod("setValue", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::setValue));
    c->addMethod("die", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::die));
    c->addMethod("isAlive", sad::dukpp03::bind_method::from(&sad::p2d::Force<sad::p2d::Vector>::isAlive));

    c->setPrototypeFunction("SadP2DTangentialForce");

    ctx->addClassBinding("sad::p2d::Force<sad::p2d::Vector>", c);

    ctx->registerCallable("SadP2DTangentialForceValue", sad::dukpp03::make_function::from(__tangentialForceValue));

    PERFORM_AND_ASSERT(
        "sad.p2d.TangentialForce = SadP2DTangentialForce;"
        "sad.p2d.TangentialForce.prototype.value = function(o)   { if (typeof o == \"undefined\") return SadP2DTangentialForceValue(this); else return this._value(); };"
        "sad.p2d.TangentialForce.prototype.toString = function() { return \"sad::p2d::Force<sad::p2d::Vector>(\" + this.value().toString()  + \")\";};"
        "sad.p2d.force = function(o)  {  var d = null; if (sad.p2d.isPoint2D(o)) d = new sad.p2d.TangentialForce(); else d = new sad.p2d.AngularForce(); d.setValue(o);  return d; };"
        "sad.p2d.impulseForce = function(o)  { var d = null; if (sad.p2d.isPoint2D(o)) d =  new sad.p2d.TangentialImpulseForce(); else return  d = sad.p2d.AngularImpulseForce(); d.setValue(o); return d;  };"
        "var pnt = function(x, y) {  if (typeof x == \"undefined\" && typeof y == \"undefined\") return sad.Point2D(); else return new sad.Point2D(x, y);  };"
    );
}


static void exposeImpulseForcePoint2D(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::ImpulseForce<sad::p2d::Vector> >("SadP2DTangentialImpulseForce");
    c->addParentBinding(ctx->getClassBinding("sad::p2d::Force<sad::p2d::Vector>"));
    c->setPrototypeFunction("SadP2DTangentialImpulseForce");

    ctx->addClassBinding("sad::p2d::ImpulseForce<sad::p2d::Vector>", c);

    PERFORM_AND_ASSERT(
        "sad.p2d.TangentialImpulseForce = SadP2DTangentialImpulseForce;"
        "sad.p2d.TangentialImpulseForce.prototype.value = function(o)   { if (typeof o == \"undefined\") return SadP2DTangentialForceValue(this); else return this._value(); };"
        "sad.p2d.TangentialImpulseForce.prototype.toString = function() { return \"sad::p2d::ImpulseForce<sad::p2d::Vector>(\" + this.value().toString()  + \")\";};"
    );
}


static void exposeForceDouble(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::Force<double> >("SadP2DAngularForce");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::Force<double>::MajorId), sad::dukpp03::setter::from(&sad::p2d::Force<double>::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::Force<double>::MinorId), sad::dukpp03::setter::from(&sad::p2d::Force<double>::MinorId));


    c->addMethod("_value", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::value));
    c->addMethod("setValue", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::setValue));
    c->addMethod("die", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::die));
    c->addMethod("isAlive", sad::dukpp03::bind_method::from(&sad::p2d::Force<double>::isAlive));

    c->setPrototypeFunction("SadP2DAngularForce");

    ctx->addClassBinding("sad::p2d::Force<double>", c);

    ctx->registerCallable("SadP2DAngularForceValue", sad::dukpp03::make_function::from(__angularForceValue));

    PERFORM_AND_ASSERT(
        "sad.p2d.AngularForce = SadP2DAngularForce;"
        "sad.p2d.AngularForce.prototype.value = function(o)   { if (typeof o == \"undefined\") return SadP2DAngularForceValue(this); else return this._value(); };"
        "sad.p2d.AngularForce.prototype.toString = function() { return \"sad::p2d::Force<double>(\" + this.value()  + \")\";};"
    );
}


static void exposeImpulseForceDouble(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::ImpulseForce<double> >("SadP2DAngularImpulseForce");
    c->addParentBinding(ctx->getClassBinding("sad::p2d::Force<double>"));
    c->setPrototypeFunction("SadP2DAngularImpulseForce");

    ctx->addClassBinding("sad::p2d::ImpulseForce<double>", c);

    PERFORM_AND_ASSERT(
        "sad.p2d.AngularImpulseForce = SadP2DAngularImpulseForce;"
        "sad.p2d.AngularImpulseForce.prototype.value = function(o)   { if (typeof o == \"undefined\") return SadP2DAngularForceValue(this); else return this._value(); };"
        "sad.p2d.AngularImpulseForce.prototype.toString = function() { return \"sad::p2d::ImpulseForce<double>(\" + this.value()  + \")\";};"
    );
}


static void exposeElasticForce(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::ElasticForce, sad::p2d::Body*, sad::p2d::Body* >("SadP2DElasticForce");
    c->addObjectConstructor<sad::p2d::ElasticForce, sad::p2d::Body*, sad::p2d::Body*, double >("SadP2DElasticForce");
    c->addObjectConstructor<sad::p2d::ElasticForce, sad::p2d::Body*, sad::p2d::Body*, double, double >("SadP2DElasticForce");

    c->addMethod("defaultDistance", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::defaultDistance));
    c->addMethod("setDefaultDistance", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::setDefaultDistance));

    c->addMethod("resistance", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::resistance));
    c->addMethod("setResistance", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::setResistance));

    c->addMethod("elasticity", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::elasticity));
    c->addMethod("setElasticity", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::setElasticity));

    c->addMethod("dependsFromBody", sad::dukpp03::bind_method::from(&sad::p2d::ElasticForce::dependsFromBody));

    c->addParentBinding(ctx->getClassBinding("sad::p2d::Force<sad::p2d::Vector>"));

    c->setPrototypeFunction("SadP2DElasticForce");

    ctx->addClassBinding("sad::p2d::ElasticForce", c);


    PERFORM_AND_ASSERT(
        "sad.p2d.ElasticForce = SadP2DElasticForce;"
        "sad.p2d.ElasticForce.prototype.toString = function() { return \"sad::p2d::ElasticForce(\" + console.dump(this.dependsFromBody()) + \",\" + this.resistance() + \",\" + this.elasticity()  + \")\";};"
    );
}

// Expose sad::p2d::InfiniteLine and related functions
static void exposeInfiniteLine(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::p2d::InfiniteLine>("SadP2DInfiniteLine");
    c->addConstructor<sad::p2d::InfiniteLine, double, double, double>("SadP2DInfiniteLine");
    c->addCloneValueObjectMethodFor<sad::p2d::InfiniteLine>();

    sad::p2d::MaybePoint (sad::p2d::InfiniteLine::*intersection1)(const sad::p2d::InfiniteLine&) const = &sad::p2d::InfiniteLine::intersection;
    sad::p2d::MaybePoint (sad::p2d::InfiniteLine::*intersection2)(const sad::p2d::Cutter2D& a) const = &sad::p2d::InfiniteLine::intersection;


    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * intersection_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    intersection_overload->add(sad::dukpp03::bind_method::from(intersection1));
    intersection_overload->add(sad::dukpp03::bind_method::from(intersection2));

    c->addMethod("intersection", intersection_overload);
    c->addMethod("hasPoint", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::hasPoint));
    c->addMethod("isSame", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::isSame));
    c->addMethod("isCollinear", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::isCollinear));
    c->addMethod("kx", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::kx));
    c->addMethod("ky", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::ky));
    c->addMethod("b", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::b));
    c->addMethod("direction", sad::dukpp03::bind_method::from(&sad::p2d::InfiniteLine::direction));

    c->setPrototypeFunction("SadP2DInfiniteLine");

    ctx->addClassBinding("sad::p2d::InfiniteLine", c);

    ctx->registerCallable("SadP2DInfiniteLineFromCutter", sad::dukpp03::make_function::from(sad::p2d::InfiniteLine::fromCutter));

    ctx->registerCallable("SadP2DInfiniteLineAppliedVector", sad::dukpp03::make_function::from(sad::p2d::InfiniteLine::appliedVector));
    
    
    sad::p2d::MaybePoint (*gintersection1)(const sad::p2d::Cutter2D& a, const sad::p2d::Cutter2D& b) = sad::p2d::intersection;
    sad::p2d::MaybePoint (*gintersection2)(const sad::p2d::Point& x, const sad::p2d::Vector& v, const sad::p2d::Cutter2D &) = sad::p2d::intersection;

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * global_intersection_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    global_intersection_overload->add(sad::dukpp03::make_function::from(gintersection1));
    global_intersection_overload->add(sad::dukpp03::make_function::from(gintersection2));
    ctx->registerCallable("SadP2DIntersection", global_intersection_overload);

    PERFORM_AND_ASSERT(
        "sad.p2d.InfiniteLine = SadP2DInfiniteLine;"
        "sad.p2d.InfiniteLine.fromCutter = SadP2DInfiniteLineFromCutter;"
        "sad.p2d.InfiniteLine.appliedVector = SadP2DInfiniteLineAppliedVector;"
        "sad.p2d.intersection = SadP2DIntersection;"
        "sad.p2d.InfiniteLine.prototype.toString = function() { return \"sad::p2d::InfiniteLine(\" + this.kx() + \",\" + this.ky() + \",\" + this.b() + \")\";};"
    );
}

// Expose sad::p2d::Weight and it's methods
static void exposeWeight(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::p2d::Weight>("SadP2DWeight");
    c->addConstructor<sad::p2d::Weight, double>("SadP2DWeight");
    c->addConstructor<sad::p2d::Weight, double, bool>("SadP2DWeight");
    c->addCloneValueObjectMethodFor<sad::p2d::Weight>();

    c->addMethod("isInfinite", sad::dukpp03::bind_method::from(&sad::p2d::Weight::isInfinite));
    c->addMethod("value", sad::dukpp03::bind_method::from(&sad::p2d::Weight::value));
    c->addMethod("setValue", sad::dukpp03::bind_method::from(&sad::p2d::Weight::setValue));

    c->setPrototypeFunction("SadP2DWeight");

    ctx->addClassBinding("sad::p2d::Weight", c);

    ctx->registerCallable("SadP2DWeightInfinite", sad::dukpp03::make_function::from(sad::p2d::Weight::infinite));
    ctx->registerCallable("SadP2DWeightConstant", sad::dukpp03::make_function::from(sad::p2d::Weight::constant));
    PERFORM_AND_ASSERT(
        "sad.p2d.Weight = SadP2DWeight;"
        "sad.p2d.Weight.infinite = SadP2DWeightInfinite;"
        "sad.p2d.Weight.constant = SadP2DWeightConstant;"
        "sad.p2d.Weight.prototype.toString = function() { return \"sad::p2d::Body(\" + this.value() + \", \" + this.isInfinite() + \")\"; };"
    );
}

static sad::p2d::AbstractMovementDeltaListener<sad::p2d::Vector>* __addMoveListener(
    sad::p2d::Body* b,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction f
)
{
    sad::p2d::AbstractMovementDeltaListener<sad::p2d::Vector>* result = new sad::dukpp03::JSMovementListener<sad::p2d::Vector>(ctx, f);
    b->addMoveListener(result);
    return result;
}


static void __removeMoveListener(
    sad::p2d::Body* b,
    sad::p2d::AbstractMovementDeltaListener<sad::p2d::Vector>* l
)
{
    b->removeMoveListener(l);
}

static sad::p2d::AbstractMovementDeltaListener<double>* __addRotateListener(
    sad::p2d::Body* b,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction f
)
{
    sad::p2d::AbstractMovementDeltaListener<double>* result = new sad::dukpp03::JSMovementListener<double>(ctx, f);
    b->addRotateListener(result);
    return result;
}

static void __removeRotateListener(
    sad::p2d::Body* b,
    sad::p2d::AbstractMovementDeltaListener<double>* l 
)
{
    b->removeRotateListener(l);
}


// Expose sad::p2d::Body and related functions
static void exposeBody(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::Body>("SadP2DBody");
    
    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::Body::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::Body::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::Body::MajorId), sad::dukpp03::setter::from(&sad::p2d::Body::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::Body::MinorId), sad::dukpp03::setter::from(&sad::p2d::Body::MinorId));


    c->addMethod("setUserObject", sad::dukpp03::bind_method::from(&sad::p2d::Body::setUserObject));
    c->addMethod("userObject", sad::dukpp03::bind_method::from(&sad::p2d::Body::userObject));
    c->addMethod("setShape", sad::dukpp03::bind_method::from(&sad::p2d::Body::setShape));
    c->addMethod("currentShape", sad::dukpp03::bind_method::from(&sad::p2d::Body::currentShape));
    c->addMethod("userType", sad::dukpp03::bind_method::from(&sad::p2d::Body::userType));

    {
         void (sad::p2d::Body::*addForce1)(sad::p2d::Force<sad::p2d::Vector>*) const = &sad::p2d::Body::addForce;
         void (sad::p2d::Body::*addForce2)(sad::p2d::Force<double>* ) const =  &sad::p2d::Body::addForce;


        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        overload->add(sad::dukpp03::bind_method::from(addForce1));
        overload->add(sad::dukpp03::bind_method::from(addForce2));

        c->addMethod("addForce", overload);
    }

    {
         void (sad::p2d::Body::*addForce1)(sad::p2d::Force<sad::p2d::Vector>*) const = &sad::p2d::Body::sheduleAddForce;
         void (sad::p2d::Body::*addForce2)(sad::p2d::Force<double>* ) const =  &sad::p2d::Body::sheduleAddForce;

         void (sad::p2d::Body::*addForce3)(sad::p2d::Force<sad::p2d::Vector>*, double) const = &sad::p2d::Body::sheduleAddForce;
         void (sad::p2d::Body::*addForce4)(sad::p2d::Force<double>*, double ) const =  &sad::p2d::Body::sheduleAddForce;


        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        overload->add(sad::dukpp03::bind_method::from(addForce1));
        overload->add(sad::dukpp03::bind_method::from(addForce2));
        overload->add(sad::dukpp03::bind_method::from(addForce3));
        overload->add(sad::dukpp03::bind_method::from(addForce4));

        c->addMethod("sheduleAddForce", overload);
    }

    {
         void (sad::p2d::Body::*removeForce1)(sad::p2d::Force<sad::p2d::Vector>*) const = &sad::p2d::Body::removeForce;
         void (sad::p2d::Body::*removeForce2)(sad::p2d::Force<double>* ) const =  &sad::p2d::Body::removeForce;


        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        overload->add(sad::dukpp03::bind_method::from(removeForce1));
        overload->add(sad::dukpp03::bind_method::from(removeForce2));

        c->addMethod("removeForce", overload);
    }

    c->addMethod("clearTangentialForces", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearTangentialForces));
    c->addMethod("clearAngularForces", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearAngularForces));
    c->addMethod("clearForces", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearForces));
    c->addMethod("tangentialForcesList", sad::dukpp03::bind_method::from(&sad::p2d::Body::tangentialForcesList));
    c->addMethod("angularForcesList", sad::dukpp03::bind_method::from(&sad::p2d::Body::angularForcesList));
    c->addMethod("stepDiscreteChangingValues", sad::dukpp03::bind_method::from(&sad::p2d::Body::stepDiscreteChangingValues));
    c->addMethod("stepPositionsAndVelocities", sad::dukpp03::bind_method::from(&sad::p2d::Body::stepPositionsAndVelocities));
    c->addMethod("clearMoveListeners", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearMoveListeners));
    c->addMethod("clearRotateListeners", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearRotateListeners));
    c->addMethod("clearListeners", sad::dukpp03::bind_method::from(&sad::p2d::Body::clearListeners));
    c->addMethod("move", sad::dukpp03::bind_method::from(&sad::p2d::Body::move));
    c->addMethod("rotate", sad::dukpp03::bind_method::from(&sad::p2d::Body::rotate));
    c->addMethod("setWeight", sad::dukpp03::bind_method::from(&sad::p2d::Body::setWeight));
    const sad::p2d::Weight & (sad::p2d::Body::*weight)() const = &sad::p2d::Body::weight;
    c->addMethod("weight", sad::dukpp03::bind_method::from(weight));
    c->addMethod("setIsGhost", sad::dukpp03::bind_method::from(&sad::p2d::Body::setIsGhost));
    c->addMethod("isGhost", sad::dukpp03::bind_method::from(&sad::p2d::Body::isGhost));
    c->addMethod("setWorld", sad::dukpp03::bind_method::from(&sad::p2d::Body::setWorld));
    c->addMethod("world", sad::dukpp03::bind_method::from(&sad::p2d::Body::world));

    c->addMethod("initPosition", sad::dukpp03::bind_method::from(&sad::p2d::Body::initPosition));
    c->addMethod("initAngle", sad::dukpp03::bind_method::from(&sad::p2d::Body::initAngle));

    c->addMethod("setCurrentPosition", sad::dukpp03::bind_method::from(&sad::p2d::Body::setCurrentPosition));
    c->addMethod("shedulePosition", sad::dukpp03::bind_method::from(&sad::p2d::Body::shedulePosition));
    c->addMethod("shedulePositionAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::shedulePositionAt));
    c->addMethod("position", sad::dukpp03::bind_method::from(&sad::p2d::Body::position));
    c->addMethod("willPositionChange", sad::dukpp03::bind_method::from(&sad::p2d::Body::willPositionChange));
    c->addMethod("nextPosition", sad::dukpp03::bind_method::from(&sad::p2d::Body::nextPosition));

    c->addMethod("setCurrentTangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::setCurrentTangentialVelocity));
    c->addMethod("sheduleTangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::sheduleTangentialVelocity));
    c->addMethod("sheduleTangentialVelocityAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::sheduleTangentialVelocityAt));
    c->addMethod("tangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::tangentialVelocity));
    c->addMethod("willTangentialVelocityChange", sad::dukpp03::bind_method::from(&sad::p2d::Body::willTangentialVelocityChange));
    c->addMethod("nextTangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::nextTangentialVelocity));

    c->addMethod("setCurrentAngle", sad::dukpp03::bind_method::from(&sad::p2d::Body::setCurrentAngle));
    c->addMethod("sheduleAngleAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::sheduleAngleAt));
    c->addMethod("angle", sad::dukpp03::bind_method::from(&sad::p2d::Body::angle));
    c->addMethod("willAngleChange", sad::dukpp03::bind_method::from(&sad::p2d::Body::willAngleChange));
    c->addMethod("nextAngle", sad::dukpp03::bind_method::from(&sad::p2d::Body::nextAngle));

    c->addMethod("setCurrentAngularVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::setCurrentAngularVelocity));
    c->addMethod("sheduleAngularVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::sheduleAngularVelocity));
    c->addMethod("sheduleAngularVelocityAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::sheduleAngularVelocityAt));
    c->addMethod("angularVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::angularVelocity));
    c->addMethod("angularVelocityAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::angularVelocityAt));
    c->addMethod("willAngularVelocityChange", sad::dukpp03::bind_method::from(&sad::p2d::Body::willAngularVelocityChange));
    c->addMethod("nextAngularVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::nextAngularVelocity));

    c->addMethod("averageChangeIndependentTangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::averageChangeIndependentTangentialVelocity));
    c->addMethod("tangentialVelocityAt", sad::dukpp03::bind_method::from(&sad::p2d::Body::tangentialVelocityAt));
    c->addMethod("timeStep", sad::dukpp03::bind_method::from(&sad::p2d::Body::timeStep));
    c->addMethod("correctPosition", sad::dukpp03::bind_method::from(&sad::p2d::Body::correctPosition));
    c->addMethod("correctTangentialVelocity", sad::dukpp03::bind_method::from(&sad::p2d::Body::correctTangentialVelocity));

    c->addMethod("setFixed", sad::dukpp03::bind_method::from(&sad::p2d::Body::setFixed));
    c->addMethod("fixed", sad::dukpp03::bind_method::from(&sad::p2d::Body::fixed));

    c->addMethod("setSamplingCount", sad::dukpp03::bind_method::from(&sad::p2d::Body::setSamplingCount));


    c->setPrototypeFunction("SadP2DBody");

    ctx->addClassBinding("sad::p2d::Body", c);

    ctx->registerCallable("__SadP2DAddMoveListener", sad::dukpp03::make_function::from(__addMoveListener));
    ctx->registerCallable("__SadP2DAddRotateListener", sad::dukpp03::make_function::from(__addRotateListener));

    ctx->registerCallable("__SadP2DRemoveMoveListener", sad::dukpp03::make_function::from(__removeMoveListener));
    ctx->registerCallable("__SadP2DRemoveRotateListener", sad::dukpp03::make_function::from(__removeRotateListener));

    PERFORM_AND_ASSERT(
        "sad.p2d.Body = SadP2DBody;"
        "sad.p2d.Body.prototype.setCollisionShape = function(o) { return this.setShape(o); };"
        "sad.p2d.Body.prototype.addMoveListener = function(ctx, f) { return __SadP2DAddMoveListener(this, ctx, f); };"
        "sad.p2d.Body.prototype.addRotateListener = function(ctx, f) { return __SadP2DAddRotateListener(this, ctx, f); };"
        "sad.p2d.Body.prototype.removeMoveListener = function(o) { return __SadP2DRemoveMoveListener(this, o); };"
        "sad.p2d.Body.prototype.removeRotateListener = function(o) { return __SadP2DRemoveRotateListener(this, o); };"
        "sad.p2d.Body.prototype.shape = function() { return this.currentShape(); };"
        "sad.p2d.Body.prototype.setVelocity = function(o) { if (sad.p2d.isPoint2D(o)) this.setCurrentTangentialVelocity(o); else this.setCurrentAngularVelocity(o); };"
        "sad.p2d.Body.prototype.sheduleVelocity = function(o) { if (sad.p2d.isPoint2D(o)) this.sheduleTangentialVelocity(o); else this.sheduleAngularVelocity(o); };"
        "sad.p2d.Body.prototype.setPosition = function(o) { if (sad.p2d.isPoint2D(o)) this.setCurrentPosition(o); else this.setCurrentAngle(o); };"
    );
}


static void __bounceSolverPushRotationFriction(sad::p2d::BounceSolver* b, double r, int index)
{
    if (index != 1 && index != 2)
    {
        throw new std::logic_error("Invalid coefficent index for rotation friction");
    }
    b->pushRotationFriction(r, index);
}


static void exposeBounceSolver(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::p2d::BounceSolver >("SadP2BounceSolver");

    c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::setObjectName));
    c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::objectName));

    c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::BounceSolver::MajorId), sad::dukpp03::setter::from(&sad::p2d::BounceSolver::MajorId));
    c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::BounceSolver::MinorId), sad::dukpp03::setter::from(&sad::p2d::BounceSolver::MinorId));

    c->addMethod("bounce", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::bounce));
    c->addMethod("enableDebug", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::enableDebug));
    c->addMethod("dump", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::dump));
    c->addMethod("toggleInelasticCollisions", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::toggleInelasticCollisions));
    c->addMethod("isEnabledInelasticCollisions", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::isEnabledInelasticCollisions));
    c->addMethod("toi", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::toi));
    c->addMethod("correctedTOI", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::correctedTOI));
    c->addMethod("toggleInelasticCollisions", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::toggleInelasticCollisions));
    c->addMethod("isEnabledInelasticCollisions", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::isEnabledInelasticCollisions));
    c->addMethod("setInelasticCollisionType", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::setInelasticCollisionTypeAsUnsignedInt));
    c->addMethod("inelasticCollisionType", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::inelasticCollisionTypeAsUnsignedInt));
    c->addMethod("setRecursionLimit", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::setRecursionLimit));
    c->addMethod("recursionLimit", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::recursionLimit));
    c->addMethod("setCollisionPrecisionStep", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::setCollisionPrecisionStep));
    c->addMethod("collisionPrecisionStep", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::collisionPrecisionStep));
    c->addMethod("setMaxSolverIterations", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::setMaxSolverIterations));
    c->addMethod("maxSolverIterations", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::maxSolverIterations));
    c->addMethod("toggleIgnoreContactPoints", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::toggleIgnoreContactPoints));
    c->addMethod("getIgnoreContactPointsFlag", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::getIgnoreContactPointsFlag));
    c->addMethod("toggleIgnoreNegativeTOI", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::toggleIgnoreNegativeTOI));
    c->addMethod("getIgnoreNegativeTOIFlag", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::getIgnoreNegativeTOIFlag));
    c->addMethod("pushRestitutionCoefficient", sad::dukpp03::bind_method::from(&sad::p2d::BounceSolver::pushRestitutionCoefficient));


    c->setPrototypeFunction("SadP2BounceSolver");

    ctx->addClassBinding("sad::p2d::BounceSolver", c);

    ctx->registerCallable("SadP2DBounceSolverPushRotationFriction", sad::dukpp03::make_function::from(__bounceSolverPushRotationFriction));

    PERFORM_AND_ASSERT(
        "sad.p2d.BounceSolver = SadP2BounceSolver;"
        "sad.p2d.BounceSolver.prototype.pushRotationFriction = function(r, index) { if (typeof index == \"undefined\")  { SadP2DBounceSolverPushRotationFriction(this, r, 1); SadP2DBounceSolverPushRotationFriction(this, r, 2); } else SadP2DBounceSolverPushRotationFriction(this, r, index); };"
        "sad.p2d.BounceSolver.InelasticCollisionType = {};"
        "sad.p2d.BounceSolver.InelasticCollisionType.ICT_NO_INELASTIC_COLLISION = 0;"
        "sad.p2d.BounceSolver.InelasticCollisionType.ICT_FIRST = 1;"
        "sad.p2d.BounceSolver.InelasticCollisionType.ICT_SECOND = 2;"
    );
}

static void exposeCollisionDetector(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::CollisionDetector::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::CollisionDetector::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::CollisionDetector::MajorId), sad::dukpp03::setter::from(&sad::p2d::CollisionDetector::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::CollisionDetector::MinorId), sad::dukpp03::setter::from(&sad::p2d::CollisionDetector::MinorId));

        c->addMethod("collides", sad::dukpp03::bind_method::from(&sad::p2d::CollisionDetector::collides));
        c->addMethod("sampleCount", sad::dukpp03::bind_method::from(&sad::p2d::CollisionDetector::sampleCount));

        ctx->addClassBinding("sad::p2d::CollisionDetector", c);
    }

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::p2d::SimpleCollisionDetector>("SadP2DSimpleCollisionDetector");
        c->addParentBinding(ctx->getClassBinding("sad::p2d::CollisionDetector"));

        c->setPrototypeFunction("SadP2DSimpleCollisionDetector");

        ctx->addClassBinding("sad::p2d::SimpleCollisionDetector", c);
    }

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::p2d::BroadCollisionDetector>("SadP2DBroadCollisionDetector");
        c->addParentBinding(ctx->getClassBinding("sad::p2d::CollisionDetector"));

        c->setPrototypeFunction("SadP2DBroadCollisionDetector");

        ctx->addClassBinding("sad::p2d::BroadCollisionDetector", c);
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::p2d::MultisamplingCollisionDetector>("SadP2DMultisamplingCollisionDetector");
        c->addObjectConstructor<sad::p2d::MultisamplingCollisionDetector, unsigned int>("SadP2DMultisamplingCollisionDetector");

        c->setPrototypeFunction("SadP2DMultisamplingCollisionDetector");

        c->addParentBinding(ctx->getClassBinding("sad::p2d::CollisionDetector"));

        ctx->addClassBinding("sad::p2d::MultisamplingCollisionDetector", c);
    }


    PERFORM_AND_ASSERT(
        "sad.p2d.SimpleCollisionDetector = SadP2DSimpleCollisionDetector;"
        "sad.p2d.BroadCollisionDetector = SadP2DBroadCollisionDetector;"
        "sad.p2d.MultisamplingCollisionDetector = SadP2DMultisamplingCollisionDetector;"
    );
}

static sad::p2d::BasicCollisionEvent __makeEvent(sad::p2d::Body* b1, sad::p2d::Body* b2, double time)
{
    return sad::p2d::BasicCollisionEvent(b1, b2, time);
}


static bool __invokeCollisionTest(sad::p2d::CollisionTest* c1, sad::p2d::CollisionShape* b1, sad::p2d::CollisionShape* b2)
{
    return c1->invoke(b1, b2);
}



static void exposeCollisionTest(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::p2d::CollisionTest >("SadP2DCollisionTest");

    c->setPrototypeFunction("SadP2DCollisionTest");

    ctx->addClassBinding("sad::p2d::CollisionTest", c);

    ctx->registerCallable("SadP2DInvokeCollisionTest", sad::dukpp03::make_function::from(__invokeCollisionTest));

    c->setPrototypeFunction("SadP2DCollisionTest");

    PERFORM_AND_ASSERT(
        "sad.p2d.CollisionTest = SadP2DCollisionTest;"
        "sad.p2d.CollisionTest.prototype.invoke = function(b1, b2)   { return SadP2DInvokeCollisionTest(this, b1, b2) };"
        "sad.p2d.CollisionTest.prototype.collides = sad.p2d.CollisionTest.prototype.invoke;"
    );
}



static void exposeWall(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addObjectConstructor<sad::p2d::Wall>("SadP2DWall");
        c->addObjectConstructor<sad::p2d::Wall, double>("SadP2DWall");
 
        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::Wall::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::Wall::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::Wall::MajorId), sad::dukpp03::setter::from(&sad::p2d::Wall::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::Wall::MinorId), sad::dukpp03::setter::from(&sad::p2d::Wall::MinorId));

        c->addMethod("tryTeleport", sad::dukpp03::bind_method::from(&sad::p2d::Wall::tryTeleport));
        c->addMethod("body", sad::dukpp03::bind_method::from(&sad::p2d::Wall::body));
        c->addMethod("setBody", sad::dukpp03::bind_method::from(&sad::p2d::Wall::setBody));
        c->addMethod("oppositeBody", sad::dukpp03::bind_method::from(&sad::p2d::Wall::oppositeBody));
        c->addMethod("setOppositeBody", sad::dukpp03::bind_method::from(&sad::p2d::Wall::setOppositeBody));
        c->addMethod("type", sad::dukpp03::bind_method::from(&sad::p2d::Wall::typeAsIntegralValue));
        c->addMethod("typeAsIntegralValue", sad::dukpp03::bind_method::from(&sad::p2d::Wall::typeAsIntegralValue));

        c->setPrototypeFunction("SadP2DWall");

        ctx->addClassBinding("sad::p2d::Wall", c);

        PERFORM_AND_ASSERT(
            "sad.p2d.Wall = SadP2DWall;"
        );
    }
}

static void exposeWalls(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addObjectConstructor<sad::p2d::Walls>("SadP2DWalls");
        c->addObjectConstructor<sad::p2d::Walls, double>("SadP2DWalls");
        c->addObjectConstructor<sad::p2d::Walls, double, double>("SadP2DWalls");
        c->addObjectConstructor<sad::p2d::Walls, double, double, double>("SadP2DWalls");
 
        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::Walls::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::Walls::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::Walls::MajorId), sad::dukpp03::setter::from(&sad::p2d::Walls::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::Walls::MinorId), sad::dukpp03::setter::from(&sad::p2d::Walls::MinorId));

        c->setPrototypeFunction("SadP2DWalls");

        c->addMethod("bodies", sad::dukpp03::bind_method::from(&sad::p2d::Walls::bodies));
        
        ctx->addClassBinding("sad::p2d::Walls", c);

        PERFORM_AND_ASSERT(
            "sad.p2d.Walls = SadP2DWalls;"
        );
    }
}

static void exposeWorld(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addObjectConstructor<sad::p2d::World>("SadP2DWorld");
 
        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::World::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::World::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::World::MajorId), sad::dukpp03::setter::from(&sad::p2d::World::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::World::MinorId), sad::dukpp03::setter::from(&sad::p2d::World::MinorId));

        c->addMethod("setDetector", sad::dukpp03::bind_method::from(&sad::p2d::World::setDetector));
        c->addMethod("timeStep", sad::dukpp03::bind_method::from(&sad::p2d::World::timeStep));

        c->addMethod("addBody", sad::dukpp03::bind_method::from(&sad::p2d::World::addBody));
        c->addMethod("removeBody", sad::dukpp03::bind_method::from(&sad::p2d::World::removeBody));
        c->addMethod("clearBodies", sad::dukpp03::bind_method::from(&sad::p2d::World::clearBodies));
        
        c->addMethod("addBodyToGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::addBodyToGroup));
        c->addMethod("removeBodyFromGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::removeBodyFromGroup));
        c->addMethod("clearGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::clearGroup));

        c->addMethod("addGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::addGroup));
        c->addMethod("removeGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::removeGroup));
        c->addMethod("clearGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::clearGroups));

        std::function<sad::p2d::BasicCollisionHandler*(sad::p2d::World*, const sad::String&, const sad::String&, sad::dukpp03::Context*, const sad::dukpp03::CompiledFunction&)> add_handler
        = [](sad::p2d::World*w, const sad::String& g1, const sad::String& g2, sad::dukpp03::Context* local_ctx, const sad::dukpp03::CompiledFunction& ff) {
            return w->addHandler(g1, g2, new sad::dukpp03::JSCollisionHandler(local_ctx, ff));
        };
        ctx->registerCallable("SadP2DWorldAddHandler", sad::dukpp03::make_lambda::from(add_handler));

        c->addMethod("removeHandlerFromGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::removeHandlerFromGroups));
        c->addMethod("removeHandler", sad::dukpp03::bind_method::from(&sad::p2d::World::removeHandler));
        c->addMethod("clearHandlers", sad::dukpp03::bind_method::from(&sad::p2d::World::clearHandlers));
        c->addMethod("clearHandlersForGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::clearHandlersForGroups));

        c->addMethod("clear", sad::dukpp03::bind_method::from(&sad::p2d::World::clear));
        c->addMethod("step", sad::dukpp03::bind_method::from(&sad::p2d::World::step));

        c->addMethod("isBodyInWorld", sad::dukpp03::bind_method::from(&sad::p2d::World::isBodyInWorld));
        c->addMethod("isInGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::isInGroup));
        c->addMethod("doesGroupExists", sad::dukpp03::bind_method::from(&sad::p2d::World::doesGroupExists));

        c->addMethod("totalBodyCount", sad::dukpp03::bind_method::from(&sad::p2d::World::totalBodyCount));
        c->addMethod("totalGroupCount", sad::dukpp03::bind_method::from(&sad::p2d::World::totalGroupCount));
        c->addMethod("amountOfBodiesInGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::amountOfBodiesInGroup));

        c->addMethod("allBodies", sad::dukpp03::bind_method::from(&sad::p2d::World::allBodies));
        c->addMethod("existingGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::existingGroups));
        c->addMethod("allBodiesInGroup", sad::dukpp03::bind_method::from(&sad::p2d::World::allBodiesInGroup));

        c->addMethod("amountOfHandlers", sad::dukpp03::bind_method::from(&sad::p2d::World::amountOfHandlers));
        c->addMethod("amountOfHandlersForGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::amountOfHandlersForGroups));
        c->addMethod("totalHandlerOccurences", sad::dukpp03::bind_method::from(&sad::p2d::World::totalHandlerOccurences));
        c->addMethod("totalHandlerOccurencesInGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::totalHandlerOccurencesInGroups));

        c->addMethod("isHandlerInWorld", sad::dukpp03::bind_method::from(&sad::p2d::World::isHandlerInWorld));
        c->addMethod("isHandlerInGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::isHandlerInGroups));
        c->addMethod("allHandlers", sad::dukpp03::bind_method::from(&sad::p2d::World::allHandlers));
        c->addMethod("allHandlersForGroups", sad::dukpp03::bind_method::from(&sad::p2d::World::allHandlersForGroups));

        c->setPrototypeFunction("SadP2DWorld");


        ctx->addClassBinding("sad::p2d::World", c);

        PERFORM_AND_ASSERT(
            "sad.p2d.World = SadP2DWorld;"
            "sad.p2d.World.prototype.addHandler = function(g1, g2, ctx, f) { return SadP2DWorldAddHandler(this, g1, g2, ctx, f); };"
        );
    }
}

static sad::String taskNameForWorld(sad::p2d::World* w)
{
    std::ostringstream s;
    s << "sad::p2d::WorldStepTask{m_world: ";
    s << w;
    s << "}";
    return s.str();
}

static sad::String addStepTask(sad::Renderer* r, sad::p2d::World* w)
{
    sad::String result = taskNameForWorld(w);
    sad::p2d::WorldStepTask* task = new sad::p2d::WorldStepTask(w, r);
    task->enable();
    task->mark(result);
    r->pipeline()->append(task);
    return result;
}

static void removeStepTask(sad::Renderer* r, sad::p2d::World* w)
{
    sad::String result = taskNameForWorld(w);
    r->pipeline()->removeByMarkWith(result, true);
}

static void enableStepTask(sad::Renderer* r, sad::p2d::World* w)
{
    sad::String result = taskNameForWorld(w);
    r->pipeline()->enableByMark(result);
}

static void disableStepTask(sad::Renderer* r, sad::p2d::World* w)
{
    sad::String result = taskNameForWorld(w);
    r->pipeline()->enableByMark(result);
}

static void exposeWorldStepTask(sad::dukpp03::Context* ctx)
{
    std::function<sad::String(sad::dukpp03::Renderer*, sad::p2d::World*)> add_step_task = [](sad::dukpp03::Renderer* r, sad::p2d::World* w)
    {
        return addStepTask(r, w);
    };
    ctx->registerCallable("SadP2DAddStepTask", sad::dukpp03::make_function::from(addStepTask));
    ctx->registerCallable("SadP2DAddStepTask", sad::dukpp03::make_lambda::from(add_step_task));

    std::function<void(sad::dukpp03::Renderer*, sad::p2d::World*)> remove_step_task = [](sad::dukpp03::Renderer* r, sad::p2d::World* w)
    {
        removeStepTask(r, w);
    };

    ctx->registerCallable("SadP2DRemoveStepTask", sad::dukpp03::make_function::from(removeStepTask));
    ctx->registerCallable("SadP2DRemoveStepTask", sad::dukpp03::make_lambda::from(remove_step_task));

    std::function<void(sad::dukpp03::Renderer*, sad::p2d::World*)> enable_step_task = [](sad::dukpp03::Renderer* r, sad::p2d::World* w)
    {
        enableStepTask(r, w);
    };

    ctx->registerCallable("SadP2DEnableStepTask", sad::dukpp03::make_function::from(enableStepTask));
    ctx->registerCallable("SadP2DEnableStepTask", sad::dukpp03::make_lambda::from(enable_step_task));

    std::function<void(sad::dukpp03::Renderer*, sad::p2d::World*)> disable_step_task = [](sad::dukpp03::Renderer* r, sad::p2d::World* w)
    {
        disableStepTask(r, w);
    };

    ctx->registerCallable("SadP2DDisableStepTask", sad::dukpp03::make_function::from(disableStepTask));
    ctx->registerCallable("SadP2DDisableStepTask", sad::dukpp03::make_lambda::from(disable_step_task));

    PERFORM_AND_ASSERT(
        "sad.p2d.addStepTask = SadP2DAddStepTask;"
        "sad.p2d.removeStepTask = SadP2DRemoveStepTask;"
        "sad.p2d.enableStepTask = SadP2DEnableStepTask;"
        "sad.p2d.disableStepTask = SadP2DDisableStepTask;"
    );
}


void exposeWay(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addObjectConstructor<sad::p2d::app::Way>("SadP2DAppWay");

        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::p2d::app::Way::MajorId), sad::dukpp03::setter::from(&sad::p2d::app::Way::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::p2d::app::Way::MinorId), sad::dukpp03::setter::from(&sad::p2d::app::Way::MinorId));

        c->addMethod("getPointInTime", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::getPointInTime));
        c->addMethod("setPoint", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::setPoint));
        c->addMethod("addPoint", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::addPoint));
        c->addMethod("insertPoint", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::insertPoint));
        c->addMethod("removePoint", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::removePoint));
        c->addMethod("closed", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::closed));
        c->addMethod("setClosed", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::setClosed));
        c->addMethod("makeClosed", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::makeClosed));
        c->addMethod("makeOpen", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::makeOpen));
        c->addMethod("setTotalTime", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::setTotalTime));
        c->addMethod("totalTime", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::totalTime));
        c->addMethod("wayPoints", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::wayPoints));
        c->addMethod("construct", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::construct));
        c->addMethod("startConstruction", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::startConstruction));
        c->addMethod("serializableName", sad::dukpp03::bind_method::from(&sad::p2d::app::Way::serializableName));

        c->setPrototypeFunction("SadP2DAppWay");
        
        ctx->addClassBinding("sad::p2d::app::Way", c);

        PERFORM_AND_ASSERT(
            "sad.p2d.app = {};"
            "sad.p2d.app.Way = SadP2DAppWay;"
        );
    }
}




void sad::dukpp03::exposeP2D(sad::dukpp03::Context* ctx)
{

    ctx->registerCallable("SadP2DAxle", sad::dukpp03::make_function::from(sad::p2d::axle));

    p2d::Cutter1D (*project1)(const p2d::Cutter2D & c, const p2d::Axle & a) = sad::p2d::project;
    p2d::Cutter1D (*project2)(const sad::Point2D & p1,
        const sad::Point2D & p2,
        const p2d::Axle & a) = sad::p2d::project;

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * project_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    project_overload->add(sad::dukpp03::make_function::from(project1));
    project_overload->add(sad::dukpp03::make_function::from(project2));
    ctx->registerCallable("SadP2DProject", project_overload);

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * collides_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    collides_overload->add(sad::dukpp03::make_function::from(sad::p2d::collides));
    collides_overload->add(sad::dukpp03::make_function::from(__collidesR2R));
    collides_overload->add(sad::dukpp03::make_function::from(__collidesC2C));
    collides_overload->add(sad::dukpp03::make_function::from(__collidesL2L));

    ctx->registerCallable("SadP2DCollides", collides_overload);
    ctx->registerCallable("SadP2DCutter", sad::dukpp03::make_function::from(sad::p2d::cutter));

    PERFORM_AND_ASSERT(
        "sad.p2d.axle = SadP2DAxle;"
        "sad.p2d.project = SadP2DProject;"
        "sad.p2d.collides = SadP2DCollides;"
        "sad.p2d.cutter = SadP2DCutter;"
    );

    // Require geometry bindings

    ctx->registerCallable("SadProjectionIsWithin", sad::dukpp03::make_function::from(sad::projectionIsWithin));

    bool (*isWithin1)(const sad::Point2D & p, const sad::Rect2D & r) = sad::isWithin;
    bool (*isWithin2)(const sad::Point2D & p, const sad::Vector<sad::Rect2D> & r) = sad::isWithin;

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * is_within_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    is_within_overload->add(sad::dukpp03::make_function::from(isWithin1));
    is_within_overload->add(sad::dukpp03::make_function::from(isWithin2));
    ctx->registerCallable("SadIsWithin", is_within_overload);

    ctx->registerCallable("__SadMoveBy", sad::dukpp03::make_function::from(___moveBy));

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * rotate_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    rotate_overload->add(sad::dukpp03::make_function::from(__rotateRect));
    rotate_overload->add(sad::dukpp03::make_function::from(__rotateVector));
    ctx->registerCallable("__SadRotate", rotate_overload);

    ctx->registerCallable("SadAngleOf", sad::dukpp03::make_function::from(sad::angleOf));
    ctx->registerCallable("SadAcos", sad::dukpp03::make_function::from(sad::acos));
    ctx->registerCallable("SadAsin", sad::dukpp03::make_function::from(sad::asin));
    ctx->registerCallable("SadNormalizeAngle", sad::dukpp03::make_function::from(sad::normalizeAngle));
    ctx->registerCallable("SadFindAngle", sad::dukpp03::make_function::from(sad::findAngle));
    ctx->registerCallable("SadIsValid", sad::dukpp03::make_function::from(sad::isValid));
    ctx->registerCallable("SadIsAABB", sad::dukpp03::make_function::from(sad::isAABB));


    ctx->registerCallable("SadIsFuzzyEqual", sad::dukpp03::make_function::from(sad::is_fuzzy_equal));
    ctx->registerCallable("SadIsFuzzyZero", sad::dukpp03::make_function::from(sad::is_fuzzy_zero));
    ctx->registerCallable("SadNonFuzzyZero", sad::dukpp03::make_function::from(sad::non_fuzzy_zero));

    bool (*eq1)(const sad::Point2D & p1, const sad::Point2D & p2, float precision) = sad::equal;
    bool (*eq2)(const sad::Point3D & p1, const sad::Point3D & p2, float precision) = sad::equal;
    bool (*eq3)(const sad::Rect2D & p1, const sad::Rect2D & p2, float precision) = sad::equal;
    bool (*eq4)(const sad::Rect<sad::Point3D> & p1, const sad::Rect<sad::Point3D> & p2, float precision) = sad::equal;

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext> * equal_overload = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    equal_overload->add(sad::dukpp03::make_function::from(eq1));
    equal_overload->add(sad::dukpp03::make_function::from(eq2));
    equal_overload->add(sad::dukpp03::make_function::from(eq3));
    equal_overload->add(sad::dukpp03::make_function::from(eq4));
    ctx->registerCallable("__SadEqual", equal_overload);

    PERFORM_AND_ASSERT(
        "sad.projectionIsWithin = SadProjectionIsWithin;"
        "sad.isWithin = SadIsWithin;"
        "sad.moveBy = __SadMoveBy;"
        "sad.rotate = __SadRotate;"
        "sad.angleOf = SadAngleOf;"
        "sad.acos = SadAcos;"
        "sad.normalizeAngle = SadNormalizeAngle;"
        "sad.findAngle = SadFindAngle;"
        "sad.isValid = SadIsValid;"
        "sad.isAABB = SadIsAABB;"
        "sad.is_fuzzy_equal = function(x1, x2, prec) { if (typeof prec == \"undefined\") prec = 0.001; return SadIsFuzzyEqual(x1, x2, prec); };"
        "sad.is_fuzzy_zero = function(x, prec) { if (typeof prec == \"undefined\") prec = 0.001; return SadIsFuzzyZero(x, prec); };"
        "sad.non_fuzzy_zero = function(x, prec) { if (typeof prec == \"undefined\") prec = 0.001; return SadNonFuzzyZero(x, prec); };"
        "sad.equal = function(x1, x2, prec) { if (typeof prec == \"undefined\") prec = 0.001; return __SadEqual(x1, x2, prec); };"
    );

    // p2d/vector.h

    ctx->registerCallable("SadP2DModulo", sad::dukpp03::make_function::from(sad::p2d::modulo));
    ctx->registerCallable("SadP2DBasis", sad::dukpp03::make_function::from(sad::p2d::basis));
    ctx->registerCallable("SadP2DUnit", sad::dukpp03::make_function::from(sad::p2d::unit));
    ctx->registerCallable("SadP2DOrtho", sad::dukpp03::make_function::from(__ortho));
    ctx->registerCallable("SadP2DScalar", sad::dukpp03::make_function::from(sad::p2d::scalar));
    ctx->registerCallable("SadP2DRect2D", sad::dukpp03::make_function::from(__isRect2D));
    ctx->registerCallable("SadP2DMakeEvent", sad::dukpp03::make_function::from(__makeEvent));


    PERFORM_AND_ASSERT(
        "sad.p2d.OrthoVectorIndex = {};"
        "sad.p2d.OrthoVectorIndex.OVI_DEG_90 = 0;"
        "sad.p2d.OrthoVectorIndex.OVI_DEG_270 = 1;"
        "sad.p2d.modulo = SadP2DModulo;"
        "sad.p2d.basis = SadP2DBasis;"
        "sad.p2d.unit = SadP2DUnit;"
        "sad.p2d.ortho = SadP2DOrtho;"
        "sad.p2d.scalar = SadP2DScalar;"
        "sad.p2d.makeEvent = SadP2DMakeEvent;"
    );

    exposeInfiniteLine(ctx);
    exposeBody(ctx);
    exposeForcePoint2D(ctx);
    exposeImpulseForcePoint2D(ctx);
    exposeForceDouble(ctx);
    exposeImpulseForceDouble(ctx);
    exposeElasticForce(ctx);
    exposeWeight(ctx);
    exposeBounceSolver(ctx);
    exposeCollisionDetector(ctx);
    exposeCollisionTest(ctx);
    exposeWall(ctx);
    exposeWalls(ctx);
    exposeWorld(ctx);
    exposeWorldStepTask(ctx);
    exposeWay(ctx);
}
