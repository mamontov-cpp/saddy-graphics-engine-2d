/*! \file   sadrect.h
    

    \brief  Definition of rectangle, used to be a bounding box or define shape.

    This file contains a definition of rectangle, used 
    to be a bounding box or define shape.
*/
#pragma once
#include "sadpoint.h"
#include <algorithm>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

#pragma pack(push, 1)

namespace sad
{

/*! \class Rect

    A basic rectangle, defined as polygon with four points. It will not necessarily be a 
    rectangle, but always will contain four points.

    Also inner array of his was devectorized to improve speed of getting certain points.
*/
template<class _Point>
class Rect
{
 public:
    /*! Creates a rectangle, where all points are filled with zeros
     */
    inline Rect()
    {
    }
    /*! Creates a rectangle from two points
        \param[in] p1 first (top-left) point
        \param[in] p2 second (bottom-right) point
     */
    inline Rect(const _Point & p1,const _Point & p2)
    : m_p0(p1), m_p1(p1), m_p3(p2), m_p2(p2)
    {
        m_p1.setX(p2.x());
        m_p3.setX(p1.x());
    }
    /*! Creates a rectangle from two points
        \param[in] x1 X coordinate of top-left point
        \param[in] y1 Y coordinate of top-left point
        \param[in] x2 X coordinate of bottom-right point
        \param[in] y2 Y coordinate of bottom-right point
     */
    inline Rect(double x1, double y1, double x2, double y2)
    : m_p0(x1, y1), m_p1(x2, y1), m_p2(x2, y2), m_p3(x1, y2)
    {
    }
    /*! Creates a rectangle from four points
         \param[in] p1 top-left point
         \param[in] p2 top-right point
         \param[in] p3 bottom-right point
         \param[in] p4 bottom-left point
     */
    inline Rect(const _Point & p1,
                const _Point & p2,
                const _Point & p3,
                const _Point & p4)
    : m_p0(p1), m_p1(p2), m_p2(p3), m_p3(p4)
    {
    }
    /*! Returns a difference between maximum and minimum x coordinates
        \return result
     */
    inline double width()  const 
    { 
        double maxw = std::max(std::max(m_p0.x(), m_p1.x()), std::max(m_p2.x(), m_p3.x()));
        double minw = std::min(std::min(m_p0.x(), m_p1.x()), std::min(m_p2.x(), m_p3.x()));
        return maxw - minw; 
    }
    /*! Returns a difference between maximum and minimum y coordinates
        \return result
     */
    inline double height() const 
    { 
        double maxh = std::max(std::max(m_p0.y(), m_p1.y()), std::max(m_p2.y(), m_p3.y()));
        double minh = std::min(std::min(m_p0.y(), m_p1.y()), std::min(m_p2.y(), m_p3.y()));
        return maxh - minh;  
    }
    /*! Returns a reference to a point. Note that this function is unchecked due to
        performance issus, so you must check manually for errors
        Here is a map:
        \par [0]---.-----.---[1]
        \par [3]---.-----.---[2]
        \param[in] i index
        \return reference
     */
    inline const _Point & operator[](unsigned int i) const
    {
        // Don't care about alignment
        const _Point * p = reinterpret_cast<const _Point*>(this);
        return *(p + i);
    }
    /*! Returns a reference to a point. Note that this function is unchecked due to
        performance issus, so you must check manually for errors
        Here is a map:
        \par [0]---.-----.---[1]
        \par [3]---.-----.---[2]
        \param[in] i index
        \return reference
     */
    inline _Point & operator[](unsigned int i)
    {
        // Don't care about alignment
        _Point * p = reinterpret_cast<_Point*>(this);
        return *(p + i);
    }
    /*! Returns top-left point
        \return point
     */
    inline const _Point & p0() const 
    { 
        return m_p0; 
    }
    /*! Returns top-right point
        \return point
     */
    inline const _Point & p1() const 
    { 
        return m_p1; 
    }
    /*! Returns bottom-right point
        \return point
     */
    inline const _Point & p2() const 
    { 
        return m_p2; 
    }
    /*! Returns bottom-left point
        \return point
     */
    inline const _Point & p3() const 
    { 
        return m_p3; 
    }
 private:
    /* Array of points, defining a rectangle. 
       \par [0]---.-----.---[1]
       \par [3]---.-----.---[2]
     */
    /*! A top-left point
     */
    _Point m_p0;
    /*! A top-right point
     */
    _Point m_p1;
    /*! A bottom-right point
     */
    _Point m_p2;
    /*! A bottom-left point
     */
    _Point m_p3;
};

/*! A shortcut for rectangle, which is defined by points in two dimensions
 */
typedef sad::Rect<sad::Point2D> Rect2D;
/*! A shortcut for rectangle, which is defined by integral points in two dimenions
 */
typedef sad::Rect<sad::Point2I> Rect2I;

/*! Converts a double precision rectangle to integral dimensioned rectangle
    \param[in] r rectangle
    \return rectangle
 */
inline sad::Rect2I _(const sad::Rect2D & r)
{
    sad::Rect2I result;
    for(int i = 0; i < 4; i++)
    {
        result[i] = sad::Point2I( (int)(r[i].x()), (int)(r[i].y()));
    }
    return result;
}

}

#pragma pack(pop)

#ifdef _MSC_VER
#pragma warning( pop )
#endif

