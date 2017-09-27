#include <animations\animationssimplemovement.h>
#include "dukpp-03/context.h"


void exposeSimpleMovement(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::animations::SimpleMovement>("SadAnimationsSimpleMovement");
    c->addMethod("setStartingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::setStartingPoint));
    c->addMethod("setEndingPoint", sad::dukpp03::bind_method::from(&sad::animations::SimpleMovement::setEndingPoint));
    c->setPrototypeFunction("SadAnimationsSimpleMovement");

    ctx->addClassBinding("sad::animations::SimpleMovement", c);
}