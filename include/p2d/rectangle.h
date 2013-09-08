/*! \file rectangle.h
	\author HiddenSeeker

	Declares a rectangle as collision shape
 */
#include "collisionshape.h"
#include "../primitives/hrect.h"
#pragma once

namespace p2d
{
/*! Describes a rectangle as collision shape
 */
class Rectangle: public p2d::CollisionShape
{
	SAD_OBJECT
protected:
	hRectF m_rect; //!< Real rectangle
public:
	inline Rectangle() {}
	/*! Sets new inner rectangle
		\param[in] rect rectangle
	 */
	inline void setRect(const hRectF & rect) { m_rect = rect; } 
	/*! Returns inner rectangle
		\return rectangle
	 */ 
	inline const hRectF & rect() const { return m_rect; }
	/*! Returns a point by index
		\param[in] index index of point
		\return point of rectangle
	 */
	const hPointF & point(int index) const;
	/*! Returns new identical rectangle
		\return rectangle
	 */
	p2d::CollisionShape * clone() const;
	/*! Returns a center of rectangle
		\return center of rectangle
	 */
	p2d::Point center() const;
	/*! Rotates a shape around it's center
		\param[in] angle angle to rotate
	 */
	void rotate(double angle);
	/*! Moves a rectangle by specified vector
		\param[in] d distance to move
	 */
	void move(const p2d::Vector & d);
	/*! Convers a rectangle to convex hull
		\return convex hull
	 */
	p2d::ConvexHull toHull() const;
	/*! Projects a rectangle to axle
		\param[in] a axle
		\return cutter
	 */
	p2d::Cutter1D project(const p2d::Axle & a) const;
	/*! Returns a points from rectangle
		\return rectangle
	 */
	hst::vector<p2d::Point> points() const;
	/*! Returns size of current type
		\param[in] size of type
	 */
	virtual size_t sizeOfType() const;
	/*! Populates a vector two pooints, belonging to a border of bound
		\param[in] v vector
	 */
	virtual void populatePoints(hst::vector<p2d::Point> & v) const;
	/*! Fetches a normal for nearest point to surface
		\param[in] p point
		\param[in] n normal
	 */
	virtual void normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n);
};

}

