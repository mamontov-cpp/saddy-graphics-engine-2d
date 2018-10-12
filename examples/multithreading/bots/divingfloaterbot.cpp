#include "divingfloaterbot.h"
#include "../game/actor.h"

bots::DivingFloaterBot::DivingFloaterBot(double from, double to, double top, double bottom)
: m_left_bound(from),
m_right_bound(to),
m_top_bound(top),
m_bottom_bound(bottom),
m_is_left(false),
m_is_diving(false),
m_is_returning(false)
{

}


bots::DivingFloaterBot::~DivingFloaterBot()
{

}


bots::AbstractBot* bots::DivingFloaterBot::clone() const
{
    return new bots::DivingFloaterBot(m_left_bound, m_right_bound, m_top_bound, m_bottom_bound);
}

void bots::DivingFloaterBot::perform(Game*, game::Actor* actor)
{
    sad::Point2D middle = actor->middle();
    if (m_is_left)
    {
        actor->tryStartGoingLeft();
        if (middle.x() <= m_left_bound)
        {
            actor->tryStopGoingLeft();
            actor->tryStartGoingRight();
            m_is_left = false;
        }
    }
    else
    {
        actor->tryStartGoingRight();
        if (middle.x() >= m_right_bound)
        {
            actor->tryStopGoingRight();
            actor->tryStartGoingLeft();
            m_is_left = true;
        }
    }
    if (!m_is_diving)
    {
        if (!m_is_returning)
        {
            if (static_cast<double>(rand()) / RAND_MAX * 4.0 > 3.8)
            {
                m_is_diving = true;
                actor->tryStartGoingDown();
            }
        }
        else
        {
             actor->tryStartGoingUp();
             if (middle.y() >= m_top_bound)
             {
                 m_is_returning = false;
                 actor->tryStopGoingUp();
             }
        }
    }
    else
    {
        actor->tryStartGoingDown();
        if (middle.y() <= m_bottom_bound)
        {
            m_is_diving = false;
            m_is_returning = true;
            actor->tryStopGoingDown();
            actor->tryStartGoingUp();
        }
    }
}
