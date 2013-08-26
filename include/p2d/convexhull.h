/*! \file convexhull.h
	\author HiddenSeeker

	Defines  a convex hull and set of operations on it
 */
#include "grahamscan.h"
#include "../templates/maybe.hpp"
#include "../templates/hlvector.hpp"
#include "axle.h"
#pragma once

namespace p2d
{
/*! A convex hull is defined as a set of operations
 */
class ConvexHull
{
 private: 
	 hst::vector<p2d::Point> m_set;
	 /*!  Inserts axle to container if not found in container
		  \param[in, out] container container with axis
		  \param[in] axle one axle
	  */
	 void tryInsertAxle(hst::vector<p2d::Axle> & container, 
						const p2d::Axle & axle) const;
	 /*! Appends axis for specified side in container
		  \param[in, out] container container with axis
		  \param[in] number number of current side of convex hull
	  */
	 void appendAxisForSide(hst::vector<p2d::Axle> & container, int number) const;
	 /*! Creates a collision axis for all sides
		 \param[in] container container with axis
	  */
	 void appendAxisForCollision(hst::vector<p2d::Axle> & container) const;
 public:
	 /*! Creates empty convex hull
	  */
	 ConvexHull();
	 /*! Creates a nex convex hull from specified set of points
		 \param[in] set set of points
	  */
	 ConvexHull(const hst::vector<p2d::Point> & set);
	 /*! Creates new hull, don't performing creating algorithm on set
		 \param[in] set set of points
		 \return new convex hull
	  */
	 static ConvexHull  uncheckedCreate(const hst::vector<p2d::Point> & set);
	 /*! Computes a union for two hulls
		 \param[in] o1 first hull
		 \param[in] o2 second hull
	  */
	 static ConvexHull getUnion(const ConvexHull & o1, const ConvexHull & o2);
	 /*! Returns a count of sides in convex hull
		 \return count of sides
	  */
	 size_t sides() const;
	 /*! Returns a count of points in convex hull
		 \return count of points
	 */
	 size_t points() const;
	 /*! Returns a side with specified number
		 \param[in] number number of side
		 \return a cutter with number
	  */
	 p2d::Cutter2D side(int number) const;
	 /*! Returns a normal to selected side
		 \param[in] number number of side
		 \return a vector
	  */
	 p2d::Vector  normal(int number) const;
	 /*! Whether two convex hulls collide
		 \param[in] c other convex hull
		 \return  whether they are colliding
	  */ 
	 bool collides(const ConvexHull & c) const;
	 /*! Projects a convex hull on specified axle
		 \param[in] axle axle
		 \return cutter
	  */
	 Cutter1D project(const p2d::Axle & axle) const;
	 /*! Returns sum of normals nearest to specified points
		 \param[in] p point
		 \return vector with normal sum
	  */
	 p2d::Vector getSumOfNormalsFor(const p2d::Point & p) const;
	 /*! Returns a center of figure
		 \return center
	  */
	 p2d::Point center() const;
};

}
