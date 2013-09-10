#include "movingsmile.h"
#include <renderer.h>

DECLARE_SOBJ_INHERITANCE(MovingSmile, p2d::app::Object)

MovingSmile::MovingSmile(p2d::app::Way * w) : m_link(w)
{
	this->initFromConstants<MovingSmile>();
}

void MovingSmile::step(double time)
{
	p2d::Point p;
	m_link.LinkedWay->step(&m_link, time, p);
	this->body()->setCurrentPosition(p);
}



void MovingSmile::render()
{
	p2d::Point p;
	m_link.LinkedWay->step(&m_link, 1.0 / sad::Renderer::ref()->fps() , p);
	this->body()->setCurrentPosition(p);
	this->p2d::app::Object::render();
}

