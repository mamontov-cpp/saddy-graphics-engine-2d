#include "movingsmile.h"

#include <renderer.h>
#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(MovingSmile, sad::p2d::app::Object)

MovingSmile::MovingSmile(sad::p2d::app::Way * w) : m_way(w), m_time(0)
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(177, 0), sad::Point2D(265, 88)),
        sad::Rect2D(sad::Point2D(-14, -14), sad::Point2D(14, 14))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(14));
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

