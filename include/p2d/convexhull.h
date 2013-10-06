/*! \file convexhull.h
	\author HiddenSeeker

	Defines  a convex hull and set of operations on it
 */
#pragma once
#include "axle.h"
#include "grahamscan.h"

#include "../templates/maybe.hpp"
#include "../templates/svector.h"

namespace sad
{

namespace p2d
{

class CollisionShape;
/*! A convex hull is defined as a set of operations
 */
class ConvexHull
{
 private: 
	 sad::vector<p2d::Point> m_set;
	 /*!  Inserts axle to container if not found in container
		  \param[in, out] container container with axis
		  \param[in] axle one axle
	  */
	 void tryInsertAxle(sad::vector<p2d::Axle> & container, 
						const p2d::Axle & axle) const;
	 /*! Appends axis for specified side in container
		  \param[in, out] container container with axis
		  \param[in] number number of current side of convex hull
	  */
	 void appendAxisForSide(sad::vector<p2d::Axle> & container, int number) const;
	 /*! Creates a collision axis for all sides
		 \param[in] container container with axis
	  */
	 void appendAxisForCollision(sad::vector<p2d::Axle> & container) const;
 public:
	 /*! Creates empty convex hull
	  */
	 ConvexHull();
	 /*! Inserts a points from shape
		 \param[in] s shape
      */
	 void insertPointsFromShape(p2d::CollisionShape * s);
	 /*! Builds a hull for a hull
	  */
	 void buildHull();
	 /*! Creates a nex convex hull from specified set of points
		 \param[in] set set of points
	  */
	 ConvexHull(const sad::vector<p2d::Point> & set);
	 /*! Creates new hull, don't performing creating algorithm on set
		 \param[in] set set of points
		 \return new convex hull
	  */
	 static ConvexHull  uncheckedCreate(const sad::vector<p2d::Point> & set);
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
	 /*! Returns set from hull
	  */
	 inline const sad::vector<p2d::Point> & set() { return m_set; }
};

/*! Projects a set of points of specified container to axle
	\param[in] container a container
	\param[in] size an amount of elements in container
	\param[in] axle axle, where everything is projected to
	\return resulting cutter
 */
template<typename T>
p2d::Cutter1D projectPointSet(const T & container, unsigned int size, const p2d::Axle & axle)
{
	double min = 0;
	double max = 0;
	bool minisset = false;
	bool maxisset = false;
	double a2 = p2d::scalar(axle, axle);
	for(size_t i = 0; i < size ; i++)
	{
		double p = p2d::scalar(container[i], axle) / a2;
		if (p < min || !minisset)
		{
			minisset = true;
			min = p;
		}
		if (p > max || !maxisset)
		{
			maxisset = true;
			max = p;
		}
	}
	return p2d::Cutter1D(min, max);
}

}

}
