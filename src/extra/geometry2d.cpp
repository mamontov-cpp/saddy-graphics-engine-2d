#include "../../include/extra/geometry2d.h"
#include "../../include/p2d/collides1d.h"

bool sad::projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2)
{
	sad::p2d::Axle axle = sad::p2d::axle(pivot1, pivot2);
	sad::p2d::Cutter1D c1 = p2d::project(test, test, axle); 
	sad::p2d::Cutter1D c2 = p2d::project(pivot1, pivot2, axle); 
	return p2d::collides(c1, c2);
}

bool sad::isWithin(const hPointF & p, const hRectF & r)
{
	bool a1 = sad::projectionIsWithin(p, r[0], r[1]);
	bool a2 = sad::projectionIsWithin(p, r[1], r[2]);
	return a1 && a2; 
}

void sad::moveBy(const hPointF & dp , hRectF & r)
{
	for(int i = 0; i < 4; i++)
	{
		r[i] += dp;
	}
}

void sad::rotate(float angle, hRectF & r)
{
	hPointF c = r[0]; c += r[2]; c/=2;
	sad::p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++)
	{
		hPointF vi = r[i] - c;
		hPointF result = vi * m;
		r[i] = c; 
		r[i] += result;
	}
}

void sad::moveAndRotateNormalized(float angle, hPointF & result, hRectF & v)
{
	sad::p2d::Matrix2x2 m = sad::p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++) {
		hPointF r = v[i] * m;
		v[i] = result + r;
	}
}


bool sad::equal(const hPointF & p1, const hPointF & p2, float precision)
{
	bool e1 = sad::is_fuzzy_equal(p1.x(), p2.x(), precision);
	bool e2 = sad::is_fuzzy_equal(p1.y(), p2.y(), precision);
	return e1 && e2; 
}


bool sad::equal(const hRectF & p1, const hRectF & p2, float precision)
{
	bool ok = true;
	for(int i = 0; i < 4; i++)
	{
		ok = ok && equal(p1[i], p2[i], precision);
	}
	return ok;
}

double sad::angle_of(double x, double y)
{
	if (sad::is_fuzzy_zero(y) && sad::is_fuzzy_zero(x))
	{
		return 0;
	}
	p2d::Vector u = p2d::unit(p2d::Vector(x, y));
	
	bool tests[4] = {
		sad::is_fuzzy_equal( u.x(), 1) && sad::is_fuzzy_equal( u.y(), 0),
		sad::is_fuzzy_equal( u.x(), 0) && sad::is_fuzzy_equal( u.y(), 1),
		sad::is_fuzzy_equal( u.x(), -1) && sad::is_fuzzy_equal( u.y(), 0),
		sad::is_fuzzy_equal( u.x(), 0) && sad::is_fuzzy_equal( u.y(), -1),
	};

	double angles[4] = { 0, M_PI / 2, M_PI, M_PI * 1.5};
	for(int i = 0; i < 4; i++)
	{
		if (tests[i]) return angles[i];
	}
	double angle =  0;
	double at = atan(y /x);
	if (x > 0)
	{
		if (y > 0 ) angle = at;
		else        angle = 2 * M_PI + at;
	}
	else
	{
		angle = M_PI + at;
	}
	return angle;
}
