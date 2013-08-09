/*! \file point.h
	\author HiddenSeeker

	Declares a point in context of 2d tiny physics engine
 */
#pragma once
#include "../primitives/hpoint.h"

namespace p2d 
{

/*! Computes a distance between two points
	\param[in] p1 first point
	\param[in] p2 second point
	\return distance between two points
*/
template<typename T>  
typename T distance(const typename hst::point<hst::D2, T> & p1, 
					const typename hst::point<hst::D2, T> & p2)
{
	T dx = p1.x() - p2.x();
	T dy = p1.y() - p2.y();
	return sqrt(dx*dx + dy*dy);
}

/*! An extended point class
 */ 
class Point: public hPointF 
{
	public:
	inline Point(double x, double y) : hPointF(x, y)
	{ }
	/*! Computes a distance to another point
		\param[in] p another point
		\return distance
	 */
	inline double distanceTo(const p2d::Point & p) const
	{
		return distance(*this, p);
	}
};

}
