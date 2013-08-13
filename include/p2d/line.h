/*! \file line.h
	\author HiddenSeeker

	Defines a line as a collision shape
 */
#include "collisionshape.h"
#include "point.h"
#pragma once

namespace p2d
{
/*! Describes a line as collision shape
 */
class Line: public p2d::CollisionShape
{
	SAD_OBJECT
protected:
	p2d::Cutter2D m_c; //!< An inner cutter
public:
	inline Line() {}
	/*! Sets new cutter
		\param[in] c cutter
	 */
	inline void setCutter(const p2d::Cutter2D & c) { m_c = c; }
	/*! Returns a cutter
		\return cutter
	 */ 
	inline const p2d::Cutter2D & cutter() const { return m_c; }
	/*! Returns new identical cutter
		\return cutter
	 */
	p2d::CollisionShape * clone() const;
	/*! Returns a center of cutter
		\return center of cutter
	 */
	p2d::Point center() const;
	/*! Rotates a lline around it's center
		\param[in] angle angle to rotate
	 */
	void rotate(double angle);
	/*! Moves a line by specified vector
		\param[in] d distance to move
	 */
	void move(const p2d::Vector & d);
	/*! Converts a cutter to convex hull
		\return convex hull
	 */
	p2d::ConvexHull toHull() const;
	/*! Projects cutter to axle
		\param[in] a axle
		\return cutter
	 */
	p2d::Cutter1D project(const p2d::Axle & a) const;
};

}

