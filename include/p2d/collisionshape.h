/*! \file collisionshape.h
	\author HiddenSeeker

	Describes a shape, used to determine collisions with objects
 */
#include "convexhull.h"
#pragma once

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
	/*! Computes nearest points to specified convex hull
		\param[in] hull specified hull
		\return point set
	 */
	virtual hst::vector<p2d::Point> nearestPointsTo(const p2d::ConvexHull & hull) const = 0;
	virtual ~CollisionShape();
};
}
