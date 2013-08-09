#include "../../include/extra/geometry2d.h"
#include "../../include/p2d/collides1d.h"

bool projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2)
{
	p2d::Axle axle = p2d::axle(pivot1, pivot2);
	p2d::Cutter1D c1 = p2d::project(test, test, axle); 
	p2d::Cutter1D c2 = p2d::project(pivot1, pivot2, axle); 
	return p2d::collides(c1, c2);
}

bool isWithin(const hPointF & p, const hRectF & r)
{
	bool a1 = projectionIsWithin(p, r[0], r[1]);
	bool a2 = projectionIsWithin(p, r[1], r[2]);
	return a1 && a2; 
}

void moveBy(const hPointF & dp , hRectF & r)
{
	for(int i = 0; i < 4; i++)
	{
		r[i] += dp;
	}
}

void rotate(float angle, hRectF & r)
{
	hPointF c = r[0]; c += r[2]; c/=2;
	p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++)
	{
		hPointF vi = r[i] - c;
		hPointF result = vi * m;
		r[i] = c; 
		r[i] += result;
	}
}

void moveAndRotateNormalized(float angle, hPointF & result, hRectF & v)
{
	p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++) {
		hPointF r = v[i] * m;
		v[i] = result + r;
	}
}


bool equal(const hPointF & p1, const hPointF & p2, float precision)
{
	bool e1 = is_fuzzy_equal(p1.x(), p2.x(), precision);
	bool e2 = is_fuzzy_equal(p1.y(), p2.y(), precision);
	return e1 && e2; 
}


bool equal(const hRectF & p1, const hRectF & p2, float precision)
{
	bool ok = true;
	for(int i = 0; i < 4; i++)
	{
		ok = ok && equal(p1[i], p2[i], precision);
	}
	return ok;
}

