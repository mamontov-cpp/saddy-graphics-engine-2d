/*! \file collisionshape.h
	\author HiddenSeeker

	Describes a shape, used to determine collisions with objects
 */
#pragma once
#include "convexhull.h"
#include "../primitives/object.h"


namespace sad
{

namespace p2d
{
/*! Describes a shape, used to determine collisions with objects
 */
class CollisionShape: public sad::Object
{
SAD_OBJECT
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
	/*! Dumps object to string for debug
		\return string
	 */
	virtual hst::string dump() const = 0;
	virtual ~CollisionShape();
};

}

}

