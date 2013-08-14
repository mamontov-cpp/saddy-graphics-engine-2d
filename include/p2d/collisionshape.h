/*! \file collisionshape.h
	\author HiddenSeeker

	Describes a shape, used to determine collisions with objects
 */
#include "convexhull.h"
#include "../primitives/object.h"
#pragma once

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
		\return copy of shape
	 */
	virtual p2d::CollisionShape * clone() const = 0;
	/*! Returns a center of shape
		\return center of shape
	 */
	virtual p2d::Point center() const = 0;
	/*! Convers a shape to convex hull
		\return convex hull
	 */
	virtual p2d::ConvexHull toHull() const = 0;
	/*! Projects shape on axle
		\param[in] a axle
		\return projection
	 */
	virtual p2d::Cutter1D project(const p2d::Axle & a) const = 0;
	virtual ~CollisionShape();
};
}
