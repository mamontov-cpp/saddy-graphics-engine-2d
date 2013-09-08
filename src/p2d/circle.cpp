#include <p2d/circle.h>
#include <p2d/circletohulltransformer.h>
#include <log/log.h>

DECLARE_SOBJ_INHERITANCE_WITH_INDEX(p2d::Circle, p2d::CollisionShape, 1);

p2d::CollisionShape * p2d::Circle::clone() const
{
	return new p2d::Circle(*this);
}

const p2d::Point & p2d::Circle::centerRef() const
{
	return m_center;
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
	double a2 = p2d::scalar(a, a);
	double rcenter = p2d::scalar(m_center, a) / a2;
	return p2d::Cutter1D(rcenter - m_radius, rcenter + m_radius);
}


size_t p2d::Circle::sizeOfType() const
{
	return sizeof(p2d::Circle);
}

void p2d::Circle::populatePoints(hst::vector<p2d::Point> & v) const
{
	p2d::CircleToHullTransformer * t = this->m_transformer;
	if (t == NULL) t = p2d::CircleToHullTransformer::ref();
	t->populate(this, v);
}


void p2d::Circle::normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n)
{
	n = p;
	n -= m_center;
	p2d::mutableUnit(n);
}

hst::string p2d::Circle::dump() const
{
	return str(fmt::Format("Circle with center ({0},{1}) and radius {2}")
							<< m_center.x() << m_center.y() << m_radius
			  );
}

