#include "p2d/bounds.h"

DECLARE_SOBJ_INHERITANCE_WITH_INDEX(p2d::Bound, p2d::CollisionShape, 3);

p2d::CollisionShape * p2d::Bound::clone() const
{
	p2d::Bound * b = new p2d::Bound();
	b->setPosition(this->position());
	b->setType(this->type());
	return b;
}

p2d::Point p2d::Bound::center() const
{
	return p2d::Point();
}


void p2d::Bound::rotate(double angle)
{

}

void p2d::Bound::move(const p2d::Vector & d)
{

}

p2d::ConvexHull p2d::Bound::toHull() const
{
	return p2d::ConvexHull();
}

p2d::Cutter1D p2d::Bound::project(const p2d::Axle & a) const
{
	return p2d::Cutter1D();
}	


p2d::Vector p2d::Bound::normal()
{
	p2d::Vector result;
	switch(this->m_type)
	{
		case BT_LEFT: { result = p2d::Vector(1, 0); break; }
		case BT_RIGHT: { result = p2d::Vector(-1, 0); break; }
		case BT_DOWN: { result = p2d::Vector(0, 1); break; }
		case BT_UP: { result = p2d::Vector(0, -1); break; }
	}
	return result;
}


p2d::InfiniteLine p2d::Bound::boundingLine()
{
	p2d::InfiniteLine result;
	if (this->type() == BT_LEFT || this->type() == BT_RIGHT)
	{
		result = p2d::InfiniteLine::appliedVector( p2d::Point(m_p, 0), p2d::Vector(0, 1) );
	}
	else
	{
		result = p2d::InfiniteLine::appliedVector( p2d::Point(0, m_p), p2d::Vector(1, 0) );
	}
	return result;
}

