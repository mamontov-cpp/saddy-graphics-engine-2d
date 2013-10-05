#include "../../include/p2d/axle.h"
#include "../../include/p2d/collides1d.h"
#include "../../include/p2d/point.h"

p2d::Axle p2d::axle(const hPointF & p1, const hPointF & p2)
{
	return p2d::unit(p2-p1);
}

p2d::Cutter1D p2d::project(const p2d::Cutter2D & c, 
					  const p2d::Axle & a)
{
	double x1 = p2d::scalar(c.p1(), a);
	double x2 = p2d::scalar(c.p2(), a);
	return p2d::Cutter1D(x1, x2);
}

p2d::Cutter1D p2d::project(const hPointF & p1,
						   const hPointF & p2,
						   const p2d::Axle & a)
{
	p2d::Cutter2D c(p1, p2);
	return p2d::project(c, a);
}

bool p2d::collides(const p2d::Cutter1D & c1, 
			       const p2d::Cutter1D & c2)
{
	return p2d::collides1D(c1.p1(), c1.p2(), c2.p1(), c2.p2());
}


p2d::PointsPair p2d::swap(const p2d::PointsPair & c)
{
	return p2d::PointsPair(c.p2(), c.p1());
}

void p2d::swap(sad::vector<p2d::PointsPair> & pairs)
{
	for(size_t i = 0 ; i < pairs.size(); i++)
	{
		pairs[i] = p2d::swap(pairs[i]);
	}
}


p2d::Cutter2D p2d::cutter(double x1, double y1, double x2, double y2)
{
	return p2d::Cutter2D(p2d::Point(x1, y1), p2d::Point(x2, y2));
}
