#include "randomstrategy.h"

#include "../../game/actor.h"

#include <dukpp-03/context.h>


// ReSharper disable once CppUnusedIncludeDirective
#include <cmath>

DECLARE_SOBJ_INHERITANCE(bots::shootingstrategies::RandomStrategy, bots::shootingstrategies::ShootingStrategy)

// ============================================== PUBLIC METHODS ==============================================

bots::shootingstrategies::RandomStrategy::RandomStrategy()
{

}

bots::shootingstrategies::RandomStrategy::~RandomStrategy()
{

}

// ============================================== PROTECTED METHODS ==============================================


void bots::shootingstrategies::RandomStrategy::shoot(game::Actor* actor)
{
    if (actor->game())
    {
        double rnd =  rand();
        double angle = rnd * M_PI * 2.0 / RAND_MAX;
        actor->setLookupAngle(angle);
    }
    actor->tryShoot();
}

void bots::shootingstrategies::exposeRandomStrategy(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
    b->addObjectConstructor<bots::shootingstrategies::RandomStrategy>("RandomStrategy");

    b->addParentBinding(ctx->getClassBinding("bots::shootingstrategies::ShootingStrategy"));

    ctx->addClassBinding("bots::shootingstrategies::RandomStrategy", b);
}