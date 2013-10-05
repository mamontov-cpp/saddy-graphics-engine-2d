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
	/*! Sets new cutter
		\param[in] p1 first point
		\param[in] p2 second point
	 */
	inline void setCutter(const p2d::Point & p1, const p2d::Point & p2) { setCutter(p2d::Cutter2D(p1, p2)); }
	/*! Sets new cutter
		\param[in] x1  x of first point
		\param[in] y1  y of first point
		\param[in] x2  x of second point
		\param[in] y2  y of second point
	 */
	inline void setCutter(double x1, double y1, double x2, double y2)
	{
		setCutter(p2d::Point(x1, y1), p2d::Point(x2, y2));
	}
	/*! Returns a cutter
		\return cutter
	 */ 
	inline const p2d::Cutter2D & cutter() const { return m_c; }
	/*! Returns new identical cuttes
		\param[in] count how many cutters we should create
		\return raw array of cutters
	 */
	p2d::CollisionShape * clone(int count) const;
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
	/*! Returns a points from a line
		\return points
	 */
	sad::vector<p2d::Point> points() const;
	/*! Returns size of current type
		\return size of type in bytes
	 */
	virtual size_t sizeOfType() const;
	/*! Returns first point of line
		\return first point
	 */
	const hPointF & p1() const { return m_c.p1();}
	/*! Returns second point of line
		\return second point
	 */
	const hPointF & p2() const { return m_c.p2();}
	/*! Populates a vector two pooints, belonging to a border of bound
		\param[in] v vector
	 */
	virtual void populatePoints(sad::vector<p2d::Point> & v) const;
	/*! In any case, returns normal to a bound. A normal is returned as rotation
		of unit vector of line by 90 degrees. So if line will be like [p1, p2] and normal is like (n1, n2)
		the following cases will be valid:
		[ (0, 0), (1, 0) ] - (0, -1),
		[ (1, 0), (0, 0) ] - (0, 1),
		[ (0, 0), (0, 1) ] - (1, 0),
		[ (0, 1), (0, 0) ] - (-1, 0).
		\param[in] p point
		\param[out] n resulting normal
	 */
	virtual void normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n) ;
	/*! Dumps object to string
		\return string
	 */
	virtual hst::string dump() const; 
};

}

