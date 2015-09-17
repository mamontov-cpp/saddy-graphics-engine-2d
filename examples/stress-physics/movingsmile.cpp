#include "movingsmile.h"
#include <renderer.h>

DECLARE_SOBJ_INHERITANCE(MovingSmile, sad::p2d::app::Object)

MovingSmile::MovingSmile(sad::p2d::app::Way * w) : m_link(w)
{
    this->initFromConstants<MovingSmile>();
}

void MovingSmile::step(double time)
{
    sad::p2d::Point p;
    m_link.LinkedWay->step(&m_link, time, p);
    this->body()->setCurrentPosition(p);
}



void MovingSmile::render()
{
    sad::p2d::Point p;
    this->body()->TimeStep = 1.0 / sad::Renderer::ref()->fps();
    m_link.LinkedWay->step(&m_link, this->body()->TimeStep , p);
    this->body()->setCurrentPosition(p);
    this->sad::p2d::app::Object::render();
}

