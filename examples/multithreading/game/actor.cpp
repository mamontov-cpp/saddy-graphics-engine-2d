#include "actor.h"

void game::Actor::Actor() : m_game(NULL), m_options(NULL)
{

}

void game::Actor::~Actor()
{
    if (m_options)
    {
        m_options->delRef();
    }
}

bool game::Actor::setOptions(game::ActorOptions* opts)
{
    if (m_options)
    {
        return false;
    }
    
    if (!opts)
    {
        reurn false;
    }
    opts->validate();
    opts->addRef();
    m_options = opts;
    return true;
}