#include "../../include/extra/geometry2d.h"



#define PRECISION  1.0E-6

s2d::vec  normalize(const s2d::vec  & v)
{
	if (fabs(v.x()) < PRECISION && fabs(v.y()) < PRECISION)
	{
		return s2d::vec(M_SQRT1_2,M_SQRT1_2);
	}
	else 
	{
		float l = sqrtf(v.x() * v.x() + v.y() * v.y() );
		return v / l;
	}
}


s2d::vec ortho(const s2d::vec & v)
{
	s2d::vec v2 = normalize(v);
	float x = v2.x();
	v2.setX(-1 * v2.y());
	v2.setY(v2.x());
	return v2;
}

bool projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2)
{
	s2d::vec axle = pivot2-pivot1;
	axle = normalize(axle);
	float  test_projection = scalar(test,axle);
	float  pivot1_projection = scalar(pivot1,axle);
	float  pivot2_projection = scalar(pivot2,axle);
	return collides1D(test_projection, test_projection , pivot1_projection, pivot2_projection);
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
		float x = vi.x()*cosa - vi.y()*sina;
		float y = vi.x()*sina + vi.y()*cosa;
		r[i] = c; 
		r[i] += hPointF(x,y);
	}
}

void moveAndRotateNormalized(float angle, hPointF & result, hRectF & v)
{
	float cosa = cos(angle);
	float sina = sin(angle);
	for(int i = 0; i < 4; i++) {
		float x = v[i].x()*cosa - v[i].y()*sina;
		float y = v[i].x()*sina + v[i].y()*cosa;
		v[i] = result + hPointF(x,y);
	}
}


bool equal(const hPointF & p1, const hPointF & p2)
{
	return fabs(p1.x() - p2.x()) < PRECISION && fabs(p1.y() - p2.y()) < PRECISION; 
}


bool equal(const hRectF & p1, const hRectF & p2)
{
	bool ok = true;
	for(int i = 0; i < 4; i++)
	{
		ok = ok && equal(p1[i], p2[i]);
	}
	return ok;
}

