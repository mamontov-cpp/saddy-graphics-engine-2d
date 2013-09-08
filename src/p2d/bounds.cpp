#include "p2d/bounds.h"
#include <limits>
#include <log/log.h>

#undef min
#undef max

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

size_t p2d::Bound::sizeOfType() const
{
	return sizeof(p2d::Bound);
}

void p2d::Bound::populatePoints(hst::vector<p2d::Point> & v) const
{
	if (this->type() == BT_LEFT || this->type() == BT_RIGHT)
	{
		// Evade overflow
		v << p2d::Point(m_p, std::numeric_limits<double>::max() * - 0.99999);
		v << p2d::Point(m_p, std::numeric_limits<double>::max());
	}
	else
	{
		v << p2d::Point(std::numeric_limits<double>::max() * - 0.99999, m_p);
		v << p2d::Point(std::numeric_limits<double>::max(), m_p);
	}
}

void p2d::Bound::normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n)
{
	// This function is duplicated for sake of optimization
	double x;
	double y;
	switch(this->m_type)
	{
		case BT_LEFT:  { x =  1; y =  0;  break; }
		case BT_RIGHT: { x = -1; y =  0;  break; }
		case BT_DOWN:  { x =  0; y =  1;  break; }
		case BT_UP:    { x =  0; y = -1;  break; }
	}
	n.setX(x);
	n.setY(y);
}

hst::string p2d::Bound::dump() const
{
	hst::string type;
	switch(this->m_type)
	{
		case BT_LEFT:  { type = "Left";  break; }
		case BT_RIGHT: { type = "Right";  break; }
		case BT_DOWN:  { type = "Down";  break; }
		case BT_UP:    { type = "Up";  break; }
	}
	return str(fmt::Format("{0} bound at {1}")
							<< type << m_p
			  );
}
