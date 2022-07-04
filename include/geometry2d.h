/*!  \file geometry2d.h
     

     A misc 2D geometry functions for 2d points and rectangles.
 */
#pragma once
#include "fuzzyequal.h"
#include "maybe.h"
#include "sadvector.h"

#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif
#include <cmath>
#ifdef _MSC_VER
    #ifndef M_PI
        #include <corecrt_math_defines.h>
    #endif
#endif
#ifndef M_PI
    #ifndef _MATH_DEFINES_DEFINED
        #define M_PI (3.14159265358979323846)
    #endif
#endif

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
bool isWithin(const sad::Point2D& p, const sad::Rect2D& r);

/*! Tests, whether point is within one of rectang;es
    \param[in] p point
    \param[in] r rectangle list
    \return whether it's within
 */
bool isWithin(const sad::Point2D& p, const sad::Vector<sad::Rect2D>& r);

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
void rotate(sad::Rect2D & r, float angle);
/*! Rotates counter-clockwise a vector 
    around his center by specified angle
    \param[in,out] v vector
    \param[in] angle angle
 */
void rotate(sad::Vector2D & v, float angle);
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
    \return normalized angle in range of [0, 2 * M_PI) 
 */
double normalizeAngle(double x);

/*! Given sine and cosine, function will try to determine angle
    \param[in] sina sine
    \param[in] cosa cosine
    \return result if any
 */
sad::Maybe<double> findAngle(double sina, double cosa);

/*! Tests, whether four points of rectangle create a rectangle.
    Note, that implementation skips degenerated cases, like a point
    \param[in] rect tested rectangle
    \return whether points is valid
 */
bool isValid(const sad::Rect2D & rect);	

/*! Tests, whether rectangle is axis aligned
    \param[in] rect rectangle
    \return whether points is valid
 */
bool isAABB(const sad::Rect2D& rect);

/*! Computes rectangle which is axis-aligned to OXY and rotation angle for it
    \param[in] rect a rectangle element
    \param[out] base an-axis aligned element
    \param[out] alpha an alpha angle
    \param[out] error a error value
 */
void getBaseRect(
    const sad::Rect2D & rect, 
    sad::Rect2D & base,
    double & alpha,
    bool * error = nullptr
);

/*! Determines, whether two 1-dimensional cutters collide
    \param[in] x11 first point of first cutter
    \param[in] x12 second point of first cutter
    \param[in] x21 first  point of second cutter
    \param[in] x22 second point of second cutter
    \return whether two 1D cutters collide
*/
template<typename _Numeric>
inline bool collides1D(_Numeric x11, _Numeric x12, _Numeric x21, _Numeric x22)
{
    if (x11>x12) { _Numeric tmp=x11; x11=x12; x12=tmp; }
    if (x21>x22) { _Numeric tmp=x21; x21=x22; x22=tmp; }
    return (x21<=x12) && (x22>=x11);
}

}
