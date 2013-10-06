/*! \file grahamscan.h
	\author HiddenSeeker

	Contains implementation of graham scan algorithm
	for building a convex hull.
 */
#pragma once
#include "point.h"

#include "../sadvector.h"


namespace sad
{

namespace p2d
{
	/*! Runs graham scan algorithm on given set for building a convex
		hull out of data
		\param[in] set set of points
		\return points in convex hull
	 */
	sad::Vector<p2d::Point> graham_scan(const sad::Vector<p2d::Point> & set);
}

}
