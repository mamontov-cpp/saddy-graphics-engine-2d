#include "playerlocatingstrategy.h"

#include <geometry2d.h>
#include <dukpp-03/context.h>

#include "../../game/actor.h"
#include "../../game/player.h"

#include "../../game.h"

DECLARE_SOBJ_INHERITANCE(bots::shootingstrategies::PlayerLocatingStrategy, bots::shootingstrategies::ShootingStrategy)

// ============================================== PUBLIC METHODS ==============================================

bots::shootingstrategies::PlayerLocatingStrategy::PlayerLocatingStrategy()
{
    
}

bots::shootingstrategies::PlayerLocatingStrategy::~PlayerLocatingStrategy()
{

}

// ============================================== PROTECTED METHODS ==============================================


void bots::shootingstrategies::PlayerLocatingStrategy::shoot(game::Actor* actor)
{
    if (actor->game())
    {
        game::Player* player = actor->game()->player();
        sad::Point2D v = player->middle() - actor->middle();
        double angle = sad::angleOf(v.x() , v.y());
        actor->setLookupAngle(angle);
    }
    actor->tryShoot();
}


void bots::shootingstrategies::exposePlayerLocationStrategy(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
    b->addObjectConstructor<bots::shootingstrategies::PlayerLocatingStrategy>("PlayerLocatingStrategy");

    b->addParentBinding(ctx->getClassBinding("bots::shootingstrategies::ShootingStrategy"));

    ctx->addClassBinding("bots::shootingstrategies::PlayerLocatingStrategy", b);
}