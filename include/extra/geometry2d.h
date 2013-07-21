/*!  \file geometry2d.h
	 \author HiddenSeeker

	 Geometry functions for 2d points and rectangles
	 Can be used to performs some tests on collision and also moving
 */
#include "rigid_body.h"

namespace s2d
{
	typedef hPointF vec;

}

/*! Normalizes a vector. If zero vector, returns (1/sqrt(1), 1 / sqrt(2))
	\param[in] v vector
	\return normalized vector
 */
s2d::vec normalize(const s2d::vec  & v);

/*! Returns normalized orthogonal vector, return (0,1) if (1,0)
	\param[in] v vector
	\return orthogonal vector
 */
s2d::vec ortho(const s2d::vec & v);

/*! Returns a scalar
	\param[in] v1 first vector
	\param[in] v2 second vector
	\return scalar multiplication result
 */
inline float scalar(const s2d::vec & v1, const s2d::vec & v2)
{
	return (float)(v1.x() * v2.x() + v1.y() * v2.y());
}
/*! Tests, whether point projection on [pivot1, pivot2] axle is within this small cutter
	\param[in] test test data
	\param[in] pivot1 first pivot
	\param[in] pivot2 second pivot
 */
bool projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2);

/*! Tests, whether point is within from rectangle
	\param[in] p point
	\param[in] r rectangle
 */
bool isWithin(const hPointF & p, const hRectF & r);

/*! Returns distance between two points
	\param[in] p1 first point
	\param[in] p2 second point
	\return distance
 */
inline float dist(const hPointF & p1, const hPointF & p2)
{
	float dx = (float)(p1.x() - p2.x());
	float dy = (float)(p1.y() - p2.y());
	return sqrtf(dx*dx + dy*dy);
}

/*! Moves a ditance by position
	\param[in] dp distance point
	\param[in] r rectangle
 */
void moveBy(const hPointF & dp , hRectF & r);

/*! Rotates a rectangle by angle
	\param[in] angle angle
	\param[in] r rectangle
 */
void rotate(float angle, hRectF & r);


/*! Moves a rectangle by angle. A rectangle must have center at (0,0)
	\param[in] angle angle
	\param[in] result resulting
	\param[in] r rectangle
 */
void moveAndRotateNormalized(float angle, hPointF & result, hRectF & r);


/*! Tests, whether two points are equal (precision bases)
	\param[in] p1 first point
	\param[in] p2 second point
 */
bool equal(const hPointF & p1, const hPointF & p2);


/*! Tests, whether two rects are equal (precision bases)
	\param[in] p1 first rect
	\param[in] p2 second rect
 */
bool equal(const hRectF & p1, const hRectF & p2);

/*! Tests, whether x modulo is lesser than 1.0E-6
	\param[in] x a number
	\return whether x modulo is lesser than 1.0E-6
 */
bool is_fuzzy_zero(float x);
/*! Tests, whether x modulo is lesser than 1.0E-6
	\param[in] x a number
	\return whether x modulo is lesser than 1.0E-6
 */
bool non_fuzzy_zero(float x);

/*! Converts 3D point to 2D 
	\param[in] p point
 */
inline hPointF _(const ::s3d::point & p)
{
	return hPointF(p.x(), p.y());
}

/*! Converts 3D point to 2D 
	\param[in] p point
 */
inline ::s3d::point   _(const hPointF & p)
{
	return ::s3d::point(p.x(), p.y(), 0.0f);
}
