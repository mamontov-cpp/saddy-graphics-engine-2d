#include "p2d/rectangle.h"
#include "extra/geometry2d.h"
#include <log/log.h>
#include <algorithm>

DECLARE_SOBJ_INHERITANCE_WITH_INDEX(p2d::Rectangle, p2d::CollisionShape, 0);

const hPointF & p2d::Rectangle::point(int index) const
{
	assert(index > -1 && index< 4);
	return m_rect[index];
}

p2d::CollisionShape * p2d::Rectangle::clone(int count) const
{
	p2d::Rectangle * b = new p2d::Rectangle[count]();
	std::fill_n(b, count, *this);
	return b;
}


p2d::Point p2d::Rectangle::center() const
{
	hPointF c = m_rect[0]; 
	c += m_rect[2]; 
	c/=2;
	return c;
}

void p2d::Rectangle::rotate(double angle)
{
	if (fabs(angle) > S2D_FP_PRECISION)
	{
		::rotate((float)angle, m_rect);
	}
}

void p2d::Rectangle::move(const p2d::Vector & d)
{
	::moveBy(d, m_rect);
}


p2d::ConvexHull p2d::Rectangle::toHull() const
{
	sad::vector<p2d::Point> set;
	for(int i = 0 ; i < 4; i++)
	{
		set << m_rect[i];
	}
	return p2d::ConvexHull(set);
}


p2d::Cutter1D p2d::Rectangle::project(const p2d::Axle & a) const
{
	return p2d::projectPointSet(m_rect, 4, a);
}



sad::vector<p2d::Point> p2d::Rectangle::points() const
{
	sad::vector<p2d::Point> points;
	for(int i = 0; i < 4; i++)
	{
		points << m_rect[i];
	}
	return points;
}

size_t p2d::Rectangle::sizeOfType() const
{
	return sizeof(p2d::Rectangle);
}

void p2d::Rectangle::populatePoints(sad::vector<p2d::Point> & v) const
{
	for(int i = 0; i < 4; i++)
	{
		v << m_rect[i];
	}
}


void p2d::Rectangle::normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n)
{
	p2d::ConvexHull h;
	h.insertPointsFromShape(this);
	h.buildHull();
	n = h.getSumOfNormalsFor(p);
}

hst::string p2d::Rectangle::dump() const
{
	return str(fmt::Format("Rectangle:\n[{0}, {1} - {2}, {3}]\n[{4}, {5} - {6}, {7}]\n")
							<< m_rect[0].x() << m_rect[0].y()
							<< m_rect[1].x() << m_rect[1].y()
							<< m_rect[2].x() << m_rect[2].y()
							<< m_rect[3].x() << m_rect[3].y()
			  );
}


void p2d::Rectangle::makeConvex()
{
	p2d::ConvexHull h;
	h.insertPointsFromShape(this);
	h.buildHull();
	for(int i = 0; i < h.set().size(); i++)
	{
		this->m_rect[i] = h.set()[i];
	}
}
