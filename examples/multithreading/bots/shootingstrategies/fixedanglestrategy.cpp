#include "fixedanglestrategy.h"
#include "../../game/actor.h"

#include <dukpp-03/context.h>

DECLARE_SOBJ_INHERITANCE(bots::shootingstrategies::FixedAngleStrategy, bots::shootingstrategies::ShootingStrategy)

// ============================================== PUBLIC METHODS ==============================================

bots::shootingstrategies::FixedAngleStrategy::FixedAngleStrategy() : m_angle(0)
{
    
}

bots::shootingstrategies::FixedAngleStrategy::FixedAngleStrategy(double angle) : m_angle(angle)
{
    
}

bots::shootingstrategies::FixedAngleStrategy::~FixedAngleStrategy()
{
    
}

void bots::shootingstrategies::FixedAngleStrategy::setAngle(double angle)
{
    m_angle = angle;
}

double bots::shootingstrategies::FixedAngleStrategy::angle() const
{
    return m_angle;
}


// ============================================== PROTECTED METHODS ==============================================

void bots::shootingstrategies::FixedAngleStrategy::shoot(game::Actor* actor)
{
    actor->setLookupAngle(m_angle);
    actor->tryShoot();
}


void bots::shootingstrategies::exposeFixedAngleStrategy(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
    b->addObjectConstructor<bots::shootingstrategies::FixedAngleStrategy>("FixedAngleStrategy0");
    b->addObjectConstructor<bots::shootingstrategies::FixedAngleStrategy, double>("FixedAngleStrategy");

    b->addMethod("setAngle", sad::dukpp03::bind_method::from(&bots::shootingstrategies::FixedAngleStrategy::setAngle));
    b->addMethod("angle", sad::dukpp03::bind_method::from(&bots::shootingstrategies::FixedAngleStrategy::angle));

    b->addParentBinding(ctx->getClassBinding("bots::shootingstrategies::ShootingStrategy"));

    ctx->addClassBinding("bots::shootingstrategies::FixedAngleStrategy", b);
}