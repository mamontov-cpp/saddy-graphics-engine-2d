/*!  \file geometry2d.h
	 \author HiddenSeeker

	 Geometry collision functions for 2d points and rectangles.
	 Can be used to perform commin geometrical operations.
 */
#pragma once
#include "fuzzy_equal.h"

#include "../p2d/point.h"
#include "../p2d/axle.h"
#include "../sadrect.h"


namespace sad
{

/*! Tests, whether point projection on axle, defined as [pivot1, pivot2] is within cutter defined by pivot1 
	and pivot 2 projections
	\param[in] test test data
	\param[in] pivot1 first pivot
	\param[in] pivot2 second pivot
	\return whether projection is within following cutter
 */
bool projectionIsWithin(const sad::Point2D & test, const sad::Point2D & pivot1, const sad::Point2D & pivot2);

/*! Tests, whether point is within rectangle
	\param[in] p point
	\param[in] r rectangle
	\return whether it's within
 */
bool isWithin(const sad::Point2D & p, const sad::Rect2D & r);

/*! Moves a rectangle by a distance vector, stored in dp
	\param[in] dp distance point
	\param[in] r rectangle
 */
void moveBy(const sad::Point2D & dp , sad::Rect2D & r);

/*! Rotates counter-clockwise a rectangle 
	around his center by specified angle
	\param[in] angle angle
	\param[in] r rectangle
 */
void rotate(float angle, sad::Rect2D & r);


/*! Moves a rectangle around by point by angle. 
    A rectangle must have center at (0,0)
	\param[in] angle angle
	\param[in] result resulting
	\param[in] r rectangle
 */
void moveAndRotateNormalized(float angle, sad::Point2D & result, sad::Rect2D & r);


/*! Tests, whether two points are equal (precision based)
	\param[in] p1 first point
	\param[in] p2 second point
	\param[in] precision precision for  comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Point2D & p1, const sad::Point2D & p2, float precision = S2D_FP_PRECISION);
/*! Tests, whether two rects are equal (precision based)
	\param[in] p1 first rect
	\param[in] p2 second rect
	\param[in] precision for comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Rect2D & p1, const sad::Rect2D & p2, float precision = S2D_FP_PRECISION);
/*! Similar behaviour as atan2, only in range of [0..2 * M_PI].
	Also handles (0, 0) as zero
	\param[in] x x coordinate
	\param[in] y y coordinate
	\return result of computation
 */
double angle_of(double x, double y);

}
