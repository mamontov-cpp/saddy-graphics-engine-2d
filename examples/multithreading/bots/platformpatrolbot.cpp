#include "platformpatrolbot.h"

#include <p2d/rectangle.h>

#include "../game/actor.h"


bots::PlatformPatrolBot::PlatformPatrolBot() : m_is_left(false)
{

}

bots::PlatformPatrolBot::~PlatformPatrolBot()
{

}


bots::AbstractBot* bots::PlatformPatrolBot::clone() const
{
    return new bots::PlatformPatrolBot();
}


void bots::PlatformPatrolBot::perform(Game*, game::Actor* actor)
{
    if (actor->isResting())
    {
        sad::Rect2D rect = static_cast<sad::p2d::Rectangle*>(actor->restingPlatform()->currentShape())->rect();
        double right = rect[2].x();
        double left=  rect[0].x();
        sad::Rect2D actor_rect = actor->sprite()->area();
        double actor_right = actor_rect[2].x();
        double actor_left = actor_rect[0].x();
        if (m_is_left)
        {
            if (actor_left <= left)
            {
                m_is_left = false;
                actor->tryStopGoingLeft();
                actor->tryStartGoingRight();
            }
            else
            {
                actor->tryStopGoingRight();
                actor->tryStartGoingLeft();
            }
        }
        else
        {
            if (actor_right >= right)
            {
                m_is_left = true;
                actor->tryStopGoingRight();
                actor->tryStartGoingLeft();
            }
            else
            {
                actor->tryStopGoingLeft();
                actor->tryStartGoingRight();
            }
        }
    }
}
