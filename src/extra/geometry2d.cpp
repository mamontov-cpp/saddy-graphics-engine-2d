#include "../../include/extra/geometry2d.h"
#include "../../include/p2d/vector.h"
#include "../../include/p2d/collides1d.h"

bool projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2)
{
	p2d::Vector axle = pivot2-pivot1;
	axle = p2d::unit(axle);
	double  test_projection = p2d::scalar(test,axle);
	double  pivot1_projection = p2d::scalar(pivot1,axle);
	double  pivot2_projection = p2d::scalar(pivot2,axle);
	return p2d::collides1D(test_projection, test_projection , pivot1_projection, pivot2_projection);
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
	float cosa = cos(angle);
	float sina = sin(angle);
	for(int i = 0; i < 4; i++)
	{
		hPointF vi = r[i] - c;
		float x = (float)(vi.x()*cosa - vi.y()*sina);
		float y = (float)(vi.x()*sina + vi.y()*cosa);
		r[i] = c; 
		r[i] += hPointF(x,y);
	}
}

void moveAndRotateNormalized(float angle, hPointF & result, hRectF & v)
{
	float cosa = cos(angle);
	float sina = sin(angle);
	for(int i = 0; i < 4; i++) {
		float x = (float)(v[i].x()*cosa - v[i].y()*sina);
		float y = (float)(v[i].x()*sina + v[i].y()*cosa);
		v[i] = result + hPointF(x,y);
	}
}


bool equal(const hPointF & p1, const hPointF & p2, float precision)
{
	return fabs(p1.x() - p2.x()) < precision && fabs(p1.y() - p2.y()) < precision; 
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

