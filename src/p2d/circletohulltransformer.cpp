#include "p2d/circletohulltransformer.h"
#include "p2d/circle.h"

p2d::CircleToHullTransformer p2d::CircleToHullTransformer::m_default(8);

p2d::CircleToHullTransformer * p2d::CircleToHullTransformer::ref()
{
	return &p2d::CircleToHullTransformer::m_default;
}

p2d::CircleToHullTransformer::~CircleToHullTransformer()
{
}

p2d::ConvexHull p2d::CircleToHullTransformer::toHull(const p2d::Circle * c)
{
	hst::vector<p2d::Point> set;
	populate(c, set);
	return p2d::ConvexHull::uncheckedCreate(set);
}

void p2d::CircleToHullTransformer::populate(const p2d::Circle * c, hst::vector<p2d::Point> &  v) const
{
	p2d::Point center = c->center();
	double r = c->radius();
	for(int i = 0;i < m_sides; i++)
	{
		double parts = M_PI * 2 / m_sides;
		double angle = parts * i;
		v <<  center + hPointF(cos(angle), sin(angle)) * r;  
	}
}


