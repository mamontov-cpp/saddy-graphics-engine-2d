#include "shootingstrategy.h"

#include <dukpp-03/context.h>

DECLARE_SOBJ(bots::shootingstrategies::ShootingStrategy)

bots::shootingstrategies::ShootingStrategy::ShootingStrategy() : m_interval(500.0)
{
    m_timer.start();
}

bots::shootingstrategies::ShootingStrategy::~ShootingStrategy()
{
    
}

void bots::shootingstrategies::ShootingStrategy::pause()
{
    m_timer.pause();
}

void bots::shootingstrategies::ShootingStrategy::resume()
{
    m_timer.resume();
}

void bots::shootingstrategies::ShootingStrategy::tryShoot(game::Actor* actor)
{
    if (m_timer.paused())
    {
        return;
    }
    double elapsed = m_timer.elapsed();
    if (elapsed <= m_interval)
    {
        return;
    }
    m_timer.start();
    shoot(actor);
}

void bots::shootingstrategies::ShootingStrategy::setInterval(double interval)
{
    m_interval = interval;
}

double bots::shootingstrategies::ShootingStrategy::interval() const
{
    return m_interval;
}


void bots::shootingstrategies::exposeShootingStrategy(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* b = new sad::dukpp03::ClassBinding();
    b->addMethod("pause", sad::dukpp03::bind_method::from(&bots::shootingstrategies::ShootingStrategy::pause));
    b->addMethod("resume", sad::dukpp03::bind_method::from(&bots::shootingstrategies::ShootingStrategy::resume));
    b->addMethod("setInterval", sad::dukpp03::bind_method::from(&bots::shootingstrategies::ShootingStrategy::setInterval));
    b->addMethod("interval", sad::dukpp03::bind_method::from(&bots::shootingstrategies::ShootingStrategy::interval));

    ctx->addClassBinding("bots::shootingstrategies::ShootingStrategy", b);
}
