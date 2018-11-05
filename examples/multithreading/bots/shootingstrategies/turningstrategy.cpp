#include "turningstrategy.h"
#include "../../game/actor.h"
#include "../../game.h"

#include <renderer.h>
#include <fuzzyequal.h>
#include <dukpp-03/context.h>


DECLARE_SOBJ_INHERITANCE(bots::shootingstrategies::TurningStrategy, bots::shootingstrategies::ShootingStrategy)

// ============================================== PUBLIC METHODS ==============================================

bots::shootingstrategies::TurningStrategy::TurningStrategy() : m_full_turn_time(60.0)
{
    
}


bots::shootingstrategies::TurningStrategy::TurningStrategy(double full_turn_time) : m_full_turn_time(full_turn_time)
{
    
}

bots::shootingstrategies::TurningStrategy::~TurningStrategy()
{
    
}

void bots::shootingstrategies::TurningStrategy::setFullTurnTime(double time)
{
    m_full_turn_time = time;
}

double bots::shootingstrategies::TurningStrategy::fullTurnTime() const
{
    return m_full_turn_time;
}

void bots::shootingstrategies::TurningStrategy::tryShoot(game::Actor* actor)
{
    if (actor->game())
    {
        double ftt = 0.001; // Default value to avoid divizion by zero
        if (!sad::is_fuzzy_zero(m_full_turn_time))
        {
            ftt = m_full_turn_time;
        }

        sad::Renderer* r = actor->game()->rendererForMainThread();
        double fps = r->fps();
        if (sad::is_fuzzy_zero(fps))
        {
            fps = 1.0;
        }
        double dangle = (1.0 / fps) * ((M_PI * 2.0) / ftt);
        double angle = actor->lookupAngle() + dangle;
        while (angle > 2 * M_PI)
        {
            angle -= 2* M_PI;
        }
        actor->setLookupAngle(angle);
    }
    this->bots::shootingstrategies::ShootingStrategy::tryShoot(actor);
}

// ============================================== PROTECTED METHODS ==============================================

void bots::shootingstrategies::TurningStrategy::shoot(game::Actor* actor)
{
    actor->tryShoot();
}

void bots::shootingstrategies::exposeTurningStrategy(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
    b->addObjectConstructor<bots::shootingstrategies::TurningStrategy>("TurningStrategy0");
    b->addObjectConstructor<bots::shootingstrategies::TurningStrategy, double>("TurningStrategy");

    b->addMethod("setFullTurnTime", sad::dukpp03::bind_method::from(&bots::shootingstrategies::TurningStrategy::setFullTurnTime));
    b->addMethod("fullTurnTime", sad::dukpp03::bind_method::from(&bots::shootingstrategies::TurningStrategy::fullTurnTime));

    b->addParentBinding(ctx->getClassBinding("bots::shootingstrategies::ShootingStrategy"));

    ctx->addClassBinding("bots::shootingstrategies::TurningStrategy", b);
}