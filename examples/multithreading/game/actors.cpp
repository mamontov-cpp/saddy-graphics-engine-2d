#include "actors.h"
#include "../game.h"

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

void game::Actors::process(Game* game, sad::dukpp03::Context* ctx)
{
    for(size_t i = 0 ; i < m_actors.size(); i++)
    {
        game::Actor* actor = m_actors[i]->Actor;
        if (!actor->isDying())
        {
            m_actors[i]->Bot->perform(game, actor);
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

void game::Actors::clearFixedFlags()
{
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        m_actors[i]->Actor->clearFixedFlags();
    }
}

void game::Actors::clear()
{
    this->destroy();
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
