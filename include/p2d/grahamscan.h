/*! \file grahamscan.h
	\author HiddenSeeker

	Contains implementation of graham scan algorithm
	for building a convex hull.
 */
#include "../templates/hlvector.hpp"
#include "../p2d/point.h"
#pragma once

namespace p2d
{
	/*! Runs graham scan algorithm on given set for building a convex
		hull out of data
		\param[in] set set of points
		\return points in convex hull
	 */
	hst::vector<p2d::Point> graham_scan(const hst::vector<p2d::Point> & set);
}