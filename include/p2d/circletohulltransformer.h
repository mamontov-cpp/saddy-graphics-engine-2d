/*! \file circletohulltransformer.h
    

    Defines a transformer for approximated transformation from circle to
    convex hull
 */
#pragma once
#include "convexhull.h"
#include "point.h"

#include "../sadvector.h"

namespace sad
{

namespace p2d
{
class Circle;
class CircleToHullTransformer
{
public:
    /*! Creates a transformer with arbitrary precision
        \param[in] sides amount of sides in selected polygon
     */
    inline CircleToHullTransformer(int sides) : m_sides(sides) {}
    inline CircleToHullTransformer(const CircleToHullTransformer & o) = default;
    /*! Just copies sides of other transformer to current
        \param[in] o operator
        \return self-reference
     */
    inline sad::p2d::CircleToHullTransformer& operator=(const sad::p2d::CircleToHullTransformer & o)
    {
        if (this == &o)
        {
            return *this;
        }
        m_sides = o.m_sides;
        return *this;
    }
    /*! Returns a default circle to hull transformer
        \return default transformer
     */
    static p2d::CircleToHullTransformer * ref();
    /*! Converts a circle to convex hull with selected precision
        \param[in] c circle
        \return converted hull
     */
    virtual p2d::ConvexHull toHull(const p2d::Circle * c);
    /*! Returns sides of transformer
        \return sides
     */
    inline int sides() const { return m_sides; }
    /*! Populates vector with a points of circle
        \param[in] c circle
        \param[out] v populated vector
     */
    void populate(const p2d::Circle * c, sad::Vector<p2d::Point> &  v) const;
    /*! Could be inherited
     */
    virtual ~CircleToHullTransformer();
private:
    int m_sides;    //!< Amount of sides to transform to
    static p2d::CircleToHullTransformer m_default;  //!< Default transformer
};

}

}
