#include <p2d/circle.h>
#include <p2d/circletohulltransformer.h>

p2d::CollisionShape * p2d::Circle::clone() const
{
	return new p2d::Circle(*this);
}

p2d::Point p2d::Circle::center() const
{
	return m_center;
}

void p2d::Circle::rotate(double angle)
{
}


void p2d::Circle::move(const p2d::Vector & d)
{
	m_center += d;
}


p2d::ConvexHull p2d::Circle::toHull() const
{
	p2d::CircleToHullTransformer * t = this->m_transformer;
	if (t == NULL) t = p2d::CircleToHullTransformer::ref();
	return t->toHull(this);
}

p2d::Cutter1D p2d::Circle::project(const p2d::Axle & a) const
{
	p2d::Vector v = p2d::unit(a);
	hPointF left = m_center - v * m_radius;
	hPointF right = m_center + v * m_radius;
	double leftr = p2d::scalar(left, v);
	double rightr = p2d::scalar(right, v);
	if (leftr > rightr)
		std::swap(leftr, rightr);
	return p2d::Cutter1D(leftr, rightr);
}


