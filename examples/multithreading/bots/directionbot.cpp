#include "directionbot.h"
#include "../game/actor.h"


bots::DirectionBot::DirectionBot(int horizontal_direction, int vertical_direction)
{
    if (horizontal_direction < bots::DirectionBot::DBHD_RIGHT)
    {
        m_horizontal_direction =  bots::DirectionBot::DBHD_RIGHT;
    }
    else
    {
        if (horizontal_direction > bots::DirectionBot::DBHD_NONE)
        {
            m_horizontal_direction =  bots::DirectionBot::DBHD_NONE;
        }
        else
        {
            m_horizontal_direction = static_cast<bots::DirectionBot::HDir>(horizontal_direction);
        }
    }

    if (vertical_direction < bots::DirectionBot::DBVD_UP)
    {
        m_vertical_direction =  bots::DirectionBot::DBVD_UP;
    }
    else
    {
        if (vertical_direction > bots::DirectionBot::DBVD_NONE)
        {
            m_vertical_direction =  bots::DirectionBot::DBVD_NONE;
        }
        else
        {
            m_vertical_direction = static_cast<bots::DirectionBot::VDir>(vertical_direction);
        }
    }
}

bots::DirectionBot::~DirectionBot()
{

}

bots::AbstractBot* bots::DirectionBot::clone() const
{
    return new bots::DirectionBot(m_horizontal_direction, m_vertical_direction);
}

void bots::DirectionBot::perform(Game*, game::Actor* actor)
{
    if (m_horizontal_direction != bots::DirectionBot::DBHD_NONE)
    {
        if (m_horizontal_direction == bots::DirectionBot::DBHD_RIGHT)
            actor->tryStartGoingRight();
        else
            actor->tryStartGoingLeft();
    }

    if (m_vertical_direction != bots::DirectionBot::DBVD_NONE)
    {
        if (m_vertical_direction == bots::DirectionBot::DBVD_UP)
            actor->tryStartGoingUp();
        else
            actor->tryStartGoingDown();
    }
}

