#include "randombot.h"

#include "../game/actor.h"

#include <camera.h>

bots::RandomBot::RandomBot(double min_left_offset, double max_right_offset) : 
m_min_left_offset(min_left_offset),
m_real_left_offset(-1),
m_max_right_offset(max_right_offset), 
m_real_right_offset(-1), 
m_left(false)
{
    
}

bots::RandomBot::~RandomBot()
{
    
}


bots::AbstractBot* bots::RandomBot::clone() const
{
    return new bots::RandomBot(m_min_left_offset, m_max_right_offset);
}

void bots::RandomBot::perform(Game* game, game::Actor* actor)
{
    sad::Scene* scene = actor->sprite()->scene();
    if (!scene)
    {
        return;
    }
    if (m_real_right_offset < 0)
    { 
        m_real_right_offset = m_max_right_offset + scene->camera().TranslationOffset.x();
        m_real_left_offset = m_min_left_offset + scene->camera().TranslationOffset.x();
    }

    if (actor->isResting()) 
    {
        if (static_cast<double>(rand()) / RAND_MAX * 4.0 > 3.8)  
        {
            actor->tryStartGoingUp();
        }
        if (m_left) 
        {
            if (actor->middle().x() < m_real_left_offset) {
                m_left = false;
            }
            else 
            {
                actor->tryStopGoingRight();
                actor->tryStartGoingLeft();
            }
        }
        else {
            if (actor->middle().x() > m_real_right_offset)
            {
                m_left= true;
            }
            else 
            {
                actor->tryStopGoingLeft();
                actor->tryStartGoingRight();
            }
        }
    }
}