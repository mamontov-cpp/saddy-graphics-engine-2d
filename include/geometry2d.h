/*!  \file geometry2d.h
	 \author HiddenSeeker

	 A misc 2D geometry functions for 2d points and rectangles.
 */
#pragma once
#include "fuzzyequal.h"

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


/*! Moves a rectangle and rotates a rectangle around by point by angle. 
    A rectangle must have center at (0,0)
	\param[in] angle angle
	\param[in] result resulting
	\param[in] r rectangle
 */
void moveAndRotateNormalized(float angle, sad::Point2D & result, sad::Rect2D & r);
/*! Similar behaviour as atan2, only in range of [0..2 * M_PI].
	Also handles (0, 0) as zero
	\param[in] x x coordinate
	\param[in] y y coordinate
	\return result of computation
 */
double angle_of(double x, double y);
/*! Computes scalar multiplication of two vectors stored in points
	\param[in] p1 first vector
	\param[in] p2 second vector
 */
bool scalar(const sad::Point3D & p1, const sad::Point3D & p2);
/*! Tests whether four points of rectangle is on same plane
	\param[in] rect tested rectangle
	\return whether points lay on plain
 */
bool isOnSamePlane(const sad::Rect<sad::Point3D> & rect);
/*! Tests, whether four points of rectangle create a rectangle.
	Note, that implementation skips degenerated cases, like a point
	\param[in] rect tested rectangle
	\return whether points is valid
 */
bool isValid(const sad::Rect<sad::Point3D> & rect);
/*! Computes arccosine of value
	\param[in] x x value
	\return value (NaN if cannot be computed)
 */
double acos(double x);
/*! Computes rectangle which lays in plane parallel to OXY, given a rotated rectangle
	and angles by which he should be rotated in OXY and OYZ sequentially to result given
	rectangle
	\param[in] rect a rectangle
	\param[out] base a rectangle, which lays in plane parallel to OXY
	\param[out] alpha a rotation angle for OXY plane
	\param[out] theta a rotation angle for OYZ angle
 */
void getBaseRect(
	const sad::Rect<sad::Point3D> & rect, 
	sad::Rect<sad::Point3D> & base,
	double & alpha,
	double & theta
);

}
