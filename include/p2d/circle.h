/*! \file circle.h
	\author HiddenSeeker

	Defines a circle as collision shape
 */
#include "collisionshape.h"
#include "point.h"
#pragma once

namespace p2d
{

class CircleToHullTransformer;
/*! Describes a circle as collision shape
 */
class Circle: public p2d::CollisionShape
{
	SAD_OBJECT
protected:
	p2d::Point m_center; //!< Center of circle
	double m_radius; //!< Radius of circle
	/*! Proxy, used to transform circle to hull
	 */
	p2d::CircleToHullTransformer *  m_transformer; 
public:
	inline Circle() { m_transformer = NULL; }
	/*! Sets new center
		\param[in] p point
	 */
	inline void setCenter(const p2d::Point & p) { m_center = p; }
	/*! Sets radius of circle
		\param[in] radius radius of circle
	 */
	inline void setRadius(double radius) { m_radius = radius; }
	/*! Sets a tranformer for circle
		\param[in] t new tranformer
	 */
	inline void setTransformer(
		p2d::CircleToHullTransformer * t	
	) { m_transformer = t; }
	/*! Returns radius
		\return radius
	 */ 
	inline double radius() const { return m_radius; }
	/*! Returns new identical circle
		\return rectangle
	 */
	p2d::CollisionShape * clone() const;
	const p2d::Point & centerRef() const;
	/*! Returns a center of circle
		\return center of rectangle
	 */
	p2d::Point center() const;
	/*! Does absolutely nothing
		\param[in] angle angle to rotate
	 */
	void rotate(double angle);
	/*! Moves a circle by specified vector
		\param[in] d distance to move
	 */
	void move(const p2d::Vector & d);
	/*! Converts circle to convex hull, using specified 
		CircleToHullTransformer. If not set, uses default transformer
		\return convex hull
	 */
	p2d::ConvexHull toHull() const;
	/*! Projects circle to axle
		\param[in] a axle
		\return cutter
	 */
	p2d::Cutter1D project(const p2d::Axle & a) const;
	/*! Returns size of current type
		\param[in] size of type
	 */
	virtual size_t sizeOfType() const;
	/*! Populates a vector two pooints, belonging to a border of bound
		\param[out] v vector
	 */
	virtual void populatePoints(hst::vector<p2d::Point> & v) const;
};

}

