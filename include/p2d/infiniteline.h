/*! \file infiniteline.h
    \author HideenSeeker

    Describes an infinite line and functions for working with other data
 */
#pragma once
#include "axle.h"
#include "point.h"

#include "../maybe.h"

namespace sad
{

namespace p2d
{

typedef sad::Maybe<sad::p2d::Point> MaybePoint;

/*! Infinite line is described as equation m_kx * x + m_ky * y  + m_b = 0  
 */
class InfiniteLine
{
public:
     /*! Default line is y = 0
      */
     inline InfiniteLine() { m_kx = 0; m_ky = 1; m_b = 0; }
     inline InfiniteLine(double kx, double ky, double b)
     : m_kx(kx), m_ky(ky), m_b(b) {  }
     /*! Constructs a line from cutter
         \param[in] c cutter
         \return constructed cutter
      */
     static  InfiniteLine fromCutter(const p2d::Cutter2D & c);
     /*! Constructs a line from point and vector
         \param[in] p point
         \param[in] v vector
         \return line if can construct
      */
     static InfiniteLine appliedVector(const p2d::Point & p, const p2d::Vector & v);
     /*! Computes an intersection with infinite line.
         If line matches other line, random point is returned
         \param[in] a other line
         \return point if has one
      */
     p2d::MaybePoint intersection(const InfiniteLine & a) const;
     /*! Computes an intersection with cutter. If cutter matches a line
         returns first of its point
         \param[in] a other line
         \return point if has one
      */
     p2d::MaybePoint intersection(const p2d::Cutter2D & a) const;
     /*! Whether line has a point
         \param[in] p point
         \return result
      */
     bool hasPoint(const p2d::Point & p) const;
     /*! Determines, whether is same line
         \param[in] a line
         \return result
      */
     bool isSame(const InfiniteLine & a) const; 
     /*! Determines, whether is collinear line
         \param[in] a line
         \return result
      */
     bool isCollinear(const InfiniteLine & a) const;

     inline double kx() const { return m_kx; }
     inline double ky() const { return m_ky; }
     inline double b() const { return m_b; }
     p2d::Vector direction() const;
private:
     double  m_kx; 
     double  m_ky;
     double  m_b;
     /*! Constructs a random point of line
         \return a random line
      */
     p2d::Point randomPoint() const;
};

/*! Computes intersection of two cutters if can
    \param[in] a first cutter
    \param[in] b second cutter
    \return point if can be computed
 */
p2d::MaybePoint intersection(const p2d::Cutter2D & a, const p2d::Cutter2D & b);

/*! Computes an intersection  of line starting from x directed by v, with cutter
    c. If they are same line, it returns nearest point of c, otherwise
    intersection.
    \param[in] x point
    \param[in] v vector
    \param[in] c cutter
 */
p2d::MaybePoint intersection(
    const p2d::Point & x, 
    const p2d::Vector & v, 
    const p2d::Cutter2D & c
);

}

}
