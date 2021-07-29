/*! \file geometry3d.h
    

    Describes an operations in 3D space
 */
#pragma once
#include "geometry2d.h"

namespace sad
{
/*! Computes scalar multiplication of two vectors stored in points
    \param[in] p1 first vector
    \param[in] p2 second vector
 */
double scalar(const sad::Point3D & p1, const sad::Point3D & p2);

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
/*! Rotates a 3D rectangle around in centers around Z axis and Y axis
    sequentially
    \param[in] rect a rectangle to be rotated
    \param[out] out_rectangle a rectangle, which will be rotated
    \param[in] alpha an angle for counter-clockwise rotation around Z axis
    \param[in] theta an angle for counter-clockwise rotation around Y axis
 */
void rotate(
    const sad::Rect<sad::Point3D> & rect,
    sad::Rect<sad::Point3D> & out_rectangle,
    double alpha,
    double theta
);
/*! Computes rectangle which lays in plane parallel to OXY, given a rotated rectangle
    and angles by which he should be rotated in OXY and OYZ sequentially to result given
    rectangle
    \param[in] rect a rectangle
    \param[out] base a rectangle, which lays in plane parallel to OXY
    \param[out] alpha a rotation angle for OXY plane
    \param[out] theta a rotation angle for OYZ angle
    \param[out] error whether error is set to true
 */
void getBaseRect(
    const sad::Rect<sad::Point3D> & rect, 
    sad::Rect<sad::Point3D> & base,
    double & alpha,
    double & theta,
    bool * error = nullptr
);

}
