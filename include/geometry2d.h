/*!  \file geometry2d.h
	 \author HiddenSeeker

	 A misc 2D geometry functions for 2d points and rectangles.
 */
#pragma once
#include "fuzzyequal.h"
#include "maybe.h"

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
	\param[in,out] r rectangle
 */
void rotate(float angle, sad::Rect2D & r);
/*! Rotates counter-clockwise a vector 
	around his center by specified angle
	\param[in] angle angle
	\param[in, out] v vector
 */
void rotate(float angle, sad::Vector2D & v);
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
double angleOf(double x, double y);

/*! Computes arccosine of value
	\param[in] x x value
	\return value (NaN if cannot be computed)
 */
double acos(double x);

/*! Computes arcsine of value
	\param[in] x inse value
	\return value (NaN if cannot be computed)
 */
double asin(double x);

/*! Makes angle fall in range from zero to 2 * M_PI
	\param[in] x angle
	\param[in] y angle
 */
double normalizeAngle(double x);

/*! Given sine and cosine, function will try to determine angle
	\param[in] sina sine
	\param[in] cosa cosine
	\return result if any
 */
sad::Maybe<double> findAngle(double sina, double cosa);
}
