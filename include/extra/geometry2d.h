/*!  \file geometry2d.h
	 \author HiddenSeeker

	 Geometry collision functions for 2d points and rectangles.
	 Can be used to perform commin geometrical operations.
 */

#include "../p2d/point.h"

namespace p2d
{
	/*! A vector is defined  by two coordinates, so we can define it as simple floating point
	 */
	typedef hPointF Vector;
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

/*! Returns a scalar multiplication result of two vectors
	\param[in] v1 first vector
	\param[in] v2 second vector
	\return scalar multiplication result
 */
inline float scalar(const s2d::vec & v1, const s2d::vec & v2)
{
	return (float)(v1.x() * v2.x() + v1.y() * v2.y());
}
/*! Tests, whether point projection on axle, defined as [pivot1, pivot2] is within cutter defined by pivot1 
	and pivot 2 projections
	\param[in] test test data
	\param[in] pivot1 first pivot
	\param[in] pivot2 second pivot
	\return whether projection is within following cutter
 */
bool projectionIsWithin(const hPointF & test, const hPointF & pivot1, const hPointF & pivot2);

/*! Tests, whether point is within rectangle
	\param[in] p point
	\param[in] r rectangle
	\return whether it's within
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

/*! Moves a rectangle by a distance vector, stored in dp
	\param[in] dp distance point
	\param[in] r rectangle
 */
void moveBy(const hPointF & dp , hRectF & r);

/*! Rotates a rectangle around his center by angle
	\param[in] angle angle
	\param[in] r rectangle
 */
void rotate(float angle, hRectF & r);


/*! Moves a rectangle around by point by angle. A rectangle must have center at (0,0)
	\param[in] angle angle
	\param[in] result resulting
	\param[in] r rectangle
 */
void moveAndRotateNormalized(float angle, hPointF & result, hRectF & r);


/*! A precision for comparing two floating point numbers
 */
#define S2D_FP_PRECISION  1.0E-6


/*! Tests, whether two points are equal (precision bases)
	\param[in] p1 first point
	\param[in] p2 second point
	\param[in] precision precision for  comparing two numbers
	\return whether they are equal
 */
bool equal(const hPointF & p1, const hPointF & p2, float precision = S2D_FP_PRECISION);


/*! Tests, whether two rects are equal (precision bases)
	\param[in] p1 first rect
	\param[in] p2 second rect
	\return whether they are equal
 */
bool equal(const hRectF & p1, const hRectF & p2, float precision = S2D_FP_PRECISION);

/*! Tests, whether x modulo is lesser than precision
	\param[in] x a number
	\return whether x modulo is lesser than precision
 */
bool is_fuzzy_zero(float x, float precision = S2D_FP_PRECISION);
/*! Tests, whether x modulo is greater than precision
	\param[in] x a number
	\return whether x modulo is greater than precision
 */
bool non_fuzzy_zero(float x, float precision = S2D_FP_PRECISION);

/*! Converts 3D point to 2D 
	\param[in] p point
	\return 2d point
 */
inline hPointF _(const ::s3d::point & p)
{
	return hPointF(p.x(), p.y());
}

/*! Converts 2D point to 3D 
	\param[in] p point
	\return 3d point
 */
inline ::s3d::point   _(const hPointF & p)
{
	return ::s3d::point(p.x(), p.y(), 0.0f);
}
