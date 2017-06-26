#include "movingsmile.h"
#include <renderer.h>

DECLARE_SOBJ_INHERITANCE(MovingSmile, sad::p2d::app::Object)

MovingSmile::MovingSmile(sad::p2d::app::Way * w) : m_way(w), m_time(0)
{
    this->initFromConstants<MovingSmile>();
}

void MovingSmile::step(double time)
{
    sad::p2d::Point p = m_way->getPointInTime(m_time, time);
    m_time += time;
    if (m_time > m_way->totalTime())
    {
        m_time -= m_way->totalTime();
    }
    this->body()->setCurrentPosition(p);
}



void MovingSmile::render()
{
    step(this->body()->TimeStep);
    this->sad::p2d::app::Object::render();
}

