#include "wayfloater.h"
#include "../game.h"

bots::WayFloater::WayFloater(double start_time, const sad::String& name) : m_time(start_time), m_way(nullptr), m_way_name(name)
{
    
}

bots::WayFloater::~WayFloater()
{
    
}

bots::AbstractBot* bots::WayFloater::clone() const
{
    return new bots::WayFloater(m_time, m_way_name);
}

void bots::WayFloater::perform(Game* game, game::Actor* actor)
{
    if (!m_way)
    {
        if (game)
        {
            m_way = game->rendererForMainThread()->database("gamescreen")->objectByName<sad::p2d::app::Way>(m_way_name);
        }
    }
    if (m_way && game)
    {
        double step = 1000.0 / (game->rendererForMainThread()->fps());
        while (m_time > m_way->totalTime())
        {
            m_time -= m_way->totalTime();
        }
        actor->move(m_way->getPointInTime(m_time, step) - actor->middle());
        m_time += step;
    }
}