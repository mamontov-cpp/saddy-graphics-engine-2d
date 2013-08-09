/*!  \file geometry2d.h
	 \author HiddenSeeker

	 Geometry collision functions for 2d points and rectangles.
	 Can be used to perform commin geometrical operations.
 */
#include "../p2d/point.h"
#include "../p2d/axle.h"
#include "../primitives/hrect.h"
#include "fuzzy_equal.h"

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

/*! Moves a rectangle by a distance vector, stored in dp
	\param[in] dp distance point
	\param[in] r rectangle
 */
void moveBy(const hPointF & dp , hRectF & r);

/*! Rotates counter-clockwise a rectangle 
	around his center by specified angle
	\param[in] angle angle
	\param[in] r rectangle
 */
void rotate(float angle, hRectF & r);


/*! Moves a rectangle around by point by angle. 
    A rectangle must have center at (0,0)
	\param[in] angle angle
	\param[in] result resulting
	\param[in] r rectangle
 */
void moveAndRotateNormalized(float angle, hPointF & result, hRectF & r);


/*! Tests, whether two points are equal (precision based)
	\param[in] p1 first point
	\param[in] p2 second point
	\param[in] precision precision for  comparing two numbers
	\return whether they are equal
 */
bool equal(const hPointF & p1, const hPointF & p2, float precision = S2D_FP_PRECISION);
/*! Tests, whether two rects are equal (precision based)
	\param[in] p1 first rect
	\param[in] p2 second rect
	\return whether they are equal
 */
bool equal(const hRectF & p1, const hRectF & p2, float precision = S2D_FP_PRECISION);
