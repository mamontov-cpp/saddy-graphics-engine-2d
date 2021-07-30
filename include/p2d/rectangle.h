/*! \file rectangle.h
    

    Declares a rectangle as collision shape
 */
#pragma once
#include "collisionshape.h"
#include "../sadrect.h"

namespace sad
{

namespace p2d
{
/*! Describes a rectangle as collision shape
 */
class Rectangle: public p2d::CollisionShape
{
public:
    inline Rectangle() {}
    /*! Sets new inner rectangle
        \param[in] rect rectangle
     */
    inline void setRect(const sad::Rect2D & rect) { m_rect = rect; } 
    /*! Returns inner rectangle
        \return rectangle
     */ 
    inline const sad::Rect2D & rect() const { return m_rect; }
    /*! Returns a point by index
        \param[in] index index of point
        \return point of rectangle
     */
    const sad::Point2D & point(int index) const;
    /*! Returns new identical rectangle
        \return rectangle
     */
    p2d::CollisionShape * clone(int count) const override;
    /*! Returns a center of rectangle
        \return center of rectangle
     */
    p2d::Point center() const override;
    /*! Rotates a shape around it's center
        \param[in] angle angle to rotate
     */
    void rotate(double angle) override;
    /*! Moves a rectangle by specified vector
        \param[in] d distance to move
     */
    void move(const p2d::Vector & d) override;
    /*! Converts a rectangle to convex hull
        \return convex hull
     */
    p2d::ConvexHull toHull() const override;
    /*! Projects a rectangle to axle
        \param[in] a axle
        \return cutter
     */
    p2d::Cutter1D project(const p2d::Axle & a) const override;
    /*! Returns a points from rectangle
        \return rectangle
     */
    sad::Vector<p2d::Point> points() const;
    /*! Returns size of current type
        \return size of type in bytes
     */
    virtual size_t sizeOfType() const override;
    /*! Populates a vector two points, belonging to a border of bound
        \param[in] v vector
     */
    virtual void populatePoints(sad::Vector<p2d::Point> & v) const override;
    /*! Fetches a normal for nearest point to surface
        \param[in] p point
        \param[in] n normal
     */
    virtual void normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n) override;
    /*! Dumps object to string
        \return string
     */
    virtual sad::String dump() const override;
    /* Makes rectangle convex
     */
    void makeConvex();
    /*! Resizes a shape by specified value
        \param[in] v value
     */
    virtual void resizeBy(const sad::p2d::Vector& v) override;
    /*! Return private meta-type index for identifying
        type of object, since we must keep shapes as POD as possible
     */
    virtual unsigned int metaIndex() override;
    /*! Return private meta-type index for identifying
        type of object, since we must keep shapes as POD as possible
     */
    static unsigned int globalMetaIndex();
    /*! Frees block of memory, correctly, casting an object to specified type
        \param[in] block a block of memory
     */
    virtual void deleteBlock(sad::p2d::CollisionShape* block) override;
protected:
    sad::Rect2D m_rect; //!< Real rectangle
};

}

}
