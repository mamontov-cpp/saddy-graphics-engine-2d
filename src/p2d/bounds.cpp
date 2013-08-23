#include "bounds.h"

DECLARE_SOBJ_INHERITANCE(p2d::Bound, p2d::CollisionShape);

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
