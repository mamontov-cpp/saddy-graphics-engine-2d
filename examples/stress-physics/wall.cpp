#include "wall.h"
#include <renderer.h>
#include <p2d/circle.h>

DECLARE_SOBJ(Wall);

Wall::Wall(p2d::BoundType w) : m_type(w)
{
}

void Wall::tryTeleport(p2d::Body * b)
{
  p2d::Point pos = this->position(b->currentShape()->center());
  b->setCurrentPosition(pos);
}

#define PADDING 0

p2d::Point Wall::position(const p2d::Point & ce)
{
	double w = sad::Renderer::ref()->settings().width();
	double h = sad::Renderer::ref()->settings().height();
	double r = 15; 
	p2d::Point result;
	if (m_type == p2d::BT_LEFT)
	{
		result =  p2d::Point(w - r - PADDING, ce.y() ); 
	}
	if (m_type == p2d::BT_RIGHT)
	{
		result =  p2d::Point(r + PADDING, ce.y() ); 
	}
	if (m_type == p2d::BT_DOWN)
	{
		result =  p2d::Point(ce.x(), h - r - PADDING ); 
	}
	if (m_type == p2d::BT_UP)
	{
		result =  p2d::Point(ce.x(), r + PADDING ); 
	}
	return result;
}

Walls::Walls()
{
	double w = sad::Renderer::ref()->settings().width();
	double h = sad::Renderer::ref()->settings().height();
	
	hst::vector< minimal_t > pairs;
	pairs << minimal_t( p2d::BT_LEFT, 0 );
	pairs << minimal_t( p2d::BT_RIGHT, w );
	pairs << minimal_t( p2d::BT_UP, h );
	pairs << minimal_t( p2d::BT_DOWN, 0 );

	for(size_t i = 0; i < pairs.size(); i++)
	{
		p2d::Bound * bound = new p2d::Bound();
		bound->setPosition(pairs[i].p2());
		bound->setType(pairs[i].p1());
		
		p2d::Body * b = new p2d::Body();
		b->setShape(bound);
		m_bodies << b;

		Wall * w = new Wall(pairs[i].p1());
		b->setUserObject(w);
		w->setBody(b);
		m_walls << w;
	}

}

const hst::vector<p2d::Body *> Walls::bodies() const
{
	return m_bodies;
}


Walls::~Walls()
{
	for(size_t i = 0; i < m_walls.count(); i++)
	{
		delete m_walls[i];
	}
}
