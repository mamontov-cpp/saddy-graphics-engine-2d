#include "p2d/line.h"
#include "primitives/matrix2x2.h"

p2d::CollisionShape * p2d::Line::clone() const
{
	return new p2d::Line(*this);
}


p2d::Point p2d::Line::center() const
{
	p2d::Point r = m_c.p1();
	r += m_c.p2();
	r /= 2.0;
	return r;
}

void p2d::Line::rotate(double angle)
{
	p2d::Point center = this->center();	
	p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	p2d::Point x1 = (this->m_c.p1() - center )* m + center;
	p2d::Point x2 = (this->m_c.p2() - center )* m + center;
	setCutter(p2d::Cutter2D(x1 , x2) );
}

void p2d::Line::move(const p2d::Vector & d)
{
	m_c._1() += d;
	m_c._2() += d;
}

p2d::ConvexHull p2d::Line::toHull() const
{
	hst::vector<p2d::Point> set;
	set << m_c.p1();
	set << m_c.p2();

	return p2d::ConvexHull(set);
}

p2d::Cutter1D p2d::Line::project(const p2d::Axle & a) const
{
	return toHull().project(a);
}

