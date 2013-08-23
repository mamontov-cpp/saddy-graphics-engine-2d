#include "wall.h"
#include <renderer.h>
#include <p2d/circle.h>
#include "player.h"

DECLARE_SOBJ(Wall);

Wall::Wall(WallType w) : m_type(w)
{
}

void Wall::tryTeleport(Player * p)
{
  assert( p->body()->currentShape()->metaData()->name() == "p2d::Circle") ;
  p2d::Circle * c = hst::checked_cast<p2d::Circle>(p->body()->currentShape());
  p2d::Point pos = this->position(c);
  p->setPosition(pos);
}

#define PADDING 2

p2d::Point Wall::position(p2d::Circle * c)
{
	double w = sad::Renderer::ref()->settings().width();
	double h = sad::Renderer::ref()->settings().height();
	p2d::Point ce =  c->center();
	double r = c->radius(); 
	p2d::Point result;
	if (m_type == WT_LEFT)
	{
		result =  p2d::Point(w - r - PADDING, ce.y() ); 
	}
	if (m_type == WT_RIGHT)
	{
		result =  p2d::Point(r + PADDING, ce.y() ); 
	}
	if (m_type == WT_DOWN)
	{
		result =  p2d::Point(ce.x(), h - r - PADDING ); 
	}
	if (m_type == WT_UP)
	{
		result =  p2d::Point(ce.x(), r + PADDING ); 
	}
	return result;
}
