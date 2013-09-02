#include "p2d/line.h"
#include "primitives/matrix2x2.h"

DECLARE_SOBJ_INHERITANCE_WITH_INDEX(p2d::Line, p2d::CollisionShape, 2);


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
	double a2 = p2d::scalar(a, a);
	double p1 = p2d::scalar(m_c.p1(), a) / a2;
	double p2 = p2d::scalar(m_c.p2(), a) / a2;
	if (p1 > p2)
		std::swap(p1, p2);
	return p2d::Cutter1D(p1, p2);
}

hst::vector<p2d::Point> p2d::Line::points() const
{
	hst::vector<p2d::Point> result;
	result << m_c.p1();
	result << m_c.p2();
	return result;
}



size_t p2d::Line::sizeOfType() const
{
	return sizeof(p2d::Line);
}

void p2d::Line::populatePoints(hst::vector<p2d::Point> & v) const
{
	v << m_c.p1();
	v << m_c.p2();
}

