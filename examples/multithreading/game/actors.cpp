#include "actors.h"
#include "../game.h"

#include "../bots/shootingstrategies/shootingstrategy.h"

// ============================================ PUBLIC METHODS ============================================

game::Actors::Actors(const game::Actors& o)
{
    this->copyState(o);
}

game::Actors& game::Actors::operator=(const game::Actors& o)
{
    this->destroy();
    this->copyState(o);
    return *this;
}

game::Actors::~Actors()
{
    this->destroy();
}


bool game::Actors::add(game::Actor* actor, bots::AbstractBot* bot)
{
    if (!actor)
    {
        return false;
    }
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        if (m_actors[i]->Actor == actor)
        {
            return false;
        }
    }

    actor->addRef();
    m_actors << new game::Actors::Data{actor, bot};
    return true;
}

void game::Actors::remove(game::Actor* actor)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        if (m_actors[i]->Actor == actor)
        {
            actor->delRef();
            delete m_actors[i]->Bot;
            delete m_actors[i];
            m_actors.removeAt(i);
            --i;
        }
    }
}

void game::Actors::remove(sad::p2d::Body* body)
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        game::Actor* actor = m_actors[i]->Actor;
        if (actor->body() == body)
        {
            actor->delRef();
            delete m_actors[i]->Bot;
            delete m_actors[i];
            m_actors.removeAt(i);
            --i;
        }
    }
}

bool game::Actors::isDead(game::Actor* actor) const
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        game::Actor* local_actor = m_actors[i]->Actor;
        if (local_actor == actor)
        {
            return false;
        }
    }
    return true;
}

void game::Actors::setStrategyForBot(game::Actor* actor, bots::shootingstrategies::ShootingStrategy* s)
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        game::Actor* local_actor = m_actors[i]->Actor;
        if (local_actor == actor)
        {
            m_actors[i]->Bot->setStrategy(s);
        }
    }
}

void game::Actors::process(Game* game)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        game::Actor* actor = m_actors[i]->Actor;
        if (!actor->isDying())
        {
            m_actors[i]->Bot->perform(game, actor);
            bots::shootingstrategies::ShootingStrategy* strategy = m_actors[i]->Bot->strategy();
            if (!actor->isDying())
            {
                if (strategy)
                {
                    strategy->tryShoot(actor);
                }
            }
        }
    }
}

void game::Actors::testResting()
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        m_actors[i]->Actor->testResting();
    }
}

void game::Actors::clear()
{
    this->destroy();
}

void game::Actors::pause()
{
    for  (size_t i = 0; i < m_actors.size(); i++)
    {
        bots::AbstractBot* bot = m_actors[i]->Bot;
        if (bot)
        {
            bots::shootingstrategies::ShootingStrategy* strategy =  bot->strategy();
            if (strategy)
            { 
                strategy->pause();
            }
        }
        game::Actor* actor = m_actors[i]->Actor;
        if (actor)
        {
            actor->pauseWeaponsReloading();
        }
    }
}

void game::Actors::resume()
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        bots::AbstractBot* bot = m_actors[i]->Bot;
        if (bot)
        {
            bots::shootingstrategies::ShootingStrategy* strategy = bot->strategy();
            if (strategy)
            {
                strategy->resume();
            }
        }
        game::Actor* actor = m_actors[i]->Actor;
        if (actor)
        {
            actor->resumeWeaponsReloading();
        }
    }
}

// ============================================ PRIVATE METHODS ============================================

void game::Actors::copyState(const game::Actors& o)
{
    for(size_t i = 0; i < o.m_actors.size(); i++)
    {
        game::Actors::Data data = *(o.m_actors[i]);
        data.Actor->addRef();
        data.Bot = data.Bot->clone();
        this->m_actors << new game::Actors::Data(data);
    }
}

void game::Actors::destroy()
{
    for(size_t i = 0; i < m_actors.size(); i++)
    {
        game::Actors::Data* data = m_actors[i];
        data->Actor->delRef();
        delete data->Bot;
        delete data;
    }
    m_actors.clear();
}
