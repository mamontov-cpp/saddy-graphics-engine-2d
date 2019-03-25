/*! \file collisionshape.h
    

    Describes a shape, used to determine collisions with objects
 */
#pragma once
#include "convexhull.h"
#include "../object.h"


namespace sad
{

namespace p2d
{
/*! Describes a shape, used to determine collisions with objects
 */
class CollisionShape
{
public:
    /*! Rotates a shape around it's center
        \param[in] angle angle to rotate
     */
    virtual void rotate(double angle) = 0;
    /*! Moves a shape by specified vector
        \param[in] d distance to move
     */
    virtual void move(const p2d::Vector & d) = 0;
    /*! Clones a shape
        \param[in] count count of cloned shape
        \return copy of shape
     */
    virtual p2d::CollisionShape * clone(int count = 1) const = 0;
    /*! Returns a center of shape
        \return center of shape
     */
    virtual p2d::Point center() const = 0;
    /*! Convers a shape to convex hull
        \return convex hull
     */
    virtual p2d::ConvexHull toHull() const = 0;
    /*! Returns size of current type
        \return size of type in bytes
     */
    virtual size_t sizeOfType() const = 0;
    /*! Projects shape on axle
        \param[in] a axle
        \return projection
     */
    virtual p2d::Cutter1D project(const p2d::Axle & a) const = 0;
    /*! Populates a vector two pooints, belonging to a border of bound
        \param[in] v vector
     */
    virtual void populatePoints(sad::Vector<p2d::Point> & v) const = 0;
    /*! Computes a normal to point on surface. Can be used to work around with 
        bouncing.
        Note that function, does not check, which it belongs to surface or not.
        You must check this manually in order to get to work with it.
        \param[in] p point
        \param[out] n resulting normal
     */
    virtual void normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n) = 0;
    /*! Resizes a shape by specified value
        \param[in] v value
     */
    virtual void resizeBy(const sad::p2d::Vector& v);
    /*! Dumps object to string for debug
        \return string
     */
    virtual sad::String dump() const = 0;
    /*! Could be inherited
     */
    virtual ~CollisionShape();
    /*! Return private meta-type index for identifying
        type of object, since we must keep shapes as POD as possible
     */
    virtual unsigned int metaIndex() = 0;
    /*! Frees block of memory, correctly, casting an object to specified type
     *  \param[in] block a block of memory
     */
    virtual void deleteBlock(sad::p2d::CollisionShape* block) = 0;
};

}

}