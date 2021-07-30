/*! \file findcontactpoints.h
    

    Contains a multimethod, which can be used to determine a contact points
    for shapes
 */
#pragma once
#include "collisionmultimethod.h"
#include "infiniteline.h"
#include "point.h"
#include "rectangle.h"
#include "circle.h"
#include "line.h"
#include "bounds.h"

#include <stdexcept>

namespace sad
{

namespace p2d
{

/*! An exception, emitted, when p2d::FindContactPoints can't determine
    contact points set
 */
class CannotDetermineContactPoints: public std::logic_error
{
 public:
    CannotDetermineContactPoints();
};

typedef sad::Vector<p2d::PointsPair> SetOfPointsPair;
/*! Inserts unique pair of points
    \param[in,out] set set of points
    \param[in] p1 first point
    \param[in] p2 second point
 */
void insertUnique(
    p2d::SetOfPointsPair & set, 
    const p2d::Point & p1,
    const p2d::Point & p2
);
/*! Inserts unique pair of points
    \param[in,out] set set of points
    \param[in] pair a pair of points
 */
void insertUnique(
    p2d::SetOfPointsPair & set,
    const p2d::PointsPair & pair
);
/*! Merges two pairs of points into one pair, honoring uniqueness
    \param[in,out] set1 first set of points
    \param[in] set2 second set of points
 */
void merge(p2d::SetOfPointsPair & set1, const p2d::SetOfPointsPair & set2);
/*! Filters optimal set of points pair, with case when p1 of each pair
    moves toward p2 with speed v. It'll leave only points with minimal t
    where t = |p2 - p1| / | v |.  
    \param[in,out] set set of pairs
    \param[in] v   vector of speed
 */ 
void filterOptimalSet(p2d::SetOfPointsPair & set, const p2d::Vector & v);
/*! Finds a contact point set between two shapes
 */
class FindContactPoints: 
public p2d::CollisionMultiMethodWithArg<p2d::SetOfPointsPair, p2d::Vector>
{
 private:
     /*! Find a contact points for two convex hulls, moving with
         speeds v1 and v2
         \param[in] c1 first convex hull
         \param[in] v1 speed of first convex hull
         \param[in] c2 second convex hull
         \param[in] v2 speed of second convex hull
         \return set of pairs if any exists
      */
     static p2d::SetOfPointsPair exec(
         const p2d::ConvexHull & c1, 
         const p2d::Vector & v1,
         const p2d::ConvexHull & c2,
         const p2d::Vector & v2
     );
     /*! Find a contact points for circle and convex hull, moving with
         speeds v1 and v2
         \param[in] c1 convex hull
         \param[in] v1 speed of circle
         \param[in] c2 circle
         \param[in] v2 speed of convex hull
         \return set of pairs if any exists
      */
     static p2d::SetOfPointsPair exec(
         const p2d::ConvexHull & c1, 
         const p2d::Vector & v1,
         const p2d::Circle * c2,
         const p2d::Vector & v2
     );
     /*! Find a contact points for two circles, moving with
         speeds v1 and v2
         \param[in] c1 first circle
         \param[in] v1 speed of first circle
         \param[in] c2 second circle
         \param[in] v2 speed of second circle
         \return set of pairs if any exists
      */
     static p2d::SetOfPointsPair exec(
         const p2d::Circle * c1, 
         const p2d::Vector & v1,
         const p2d::Circle * c2,
         const p2d::Vector & v2
     );

     static p2d::SetOfPointsPair getRtoC(
         p2d::Rectangle * s1, 
         const p2d::Vector & v1,
         p2d::Circle * s2,
         const p2d::Vector & v2
     );
     static p2d::SetOfPointsPair getRtoR(
         p2d::Rectangle * s1, 
         const p2d::Vector & v1,
         p2d::Rectangle * s2,
         const p2d::Vector & v2
     );
     static p2d::SetOfPointsPair getRtoL(
         p2d::Rectangle * s1, 
         const p2d::Vector & v1,
         p2d::Line * s2,
         const p2d::Vector & v2
     );
     
     static p2d::SetOfPointsPair getCtoC(
         p2d::Circle * s1, 
         const p2d::Vector & v1,
         p2d::Circle * s2,
         const p2d::Vector & v2
     );
     static p2d::SetOfPointsPair getLtoC(
         p2d::Line * s1, 
         const p2d::Vector & v1,
         p2d::Circle * s2,
         const p2d::Vector & v2
     );
     static p2d::SetOfPointsPair getLtoL(
         p2d::Line * s1, 
         const p2d::Vector & v1,
         p2d::Line * s2,
         const p2d::Vector & v2
     );
     static p2d::SetOfPointsPair getBtoB(
         p2d::Bound * s1, 
         const p2d::Vector & v1,
         p2d::Bound * s2,
         const p2d::Vector & v2
     );
     template<typename _Polygon>
     static p2d::SetOfPointsPair getBtoP(
         p2d::Bound * s1,
         const p2d::Vector & v1,
         _Polygon * s2,
         const p2d::Vector & v2
     );

     static p2d::SetOfPointsPair getBtoC(
         p2d::Bound * s1,
         const p2d::Vector & v1,
         p2d::Circle * s2,
         const p2d::Vector & v2	
     );
protected:
        /*! Reverses a set of points if needed
            \param[in,out] pairs a set of pairs to be reversed
         */
        virtual void reverse(p2d::SetOfPointsPair & pairs) override;
        /*! Initializes a callbacks in set
         */
        virtual void init() override;
};

/*! Computes intersection for normal, emitted from p, to a line, determined
    by cutter
    \param[in] p point, where normal is emitted
    \param[in] c cutter
    \return intersection
 */
p2d::Point intersectionWithNormalFrom(
    const p2d::Point & p,
    const p2d::Cutter2D & c
);

/*! Computes an intersection points of line and circle
    \param[in] l line
    \param[in] c circle
    \return intersection points
 */
sad::Vector<p2d::Point> intersection(
    const p2d::InfiniteLine & l,
    const Circle * c
); 
/*! Computes contact points for cutter c, moving with speed v towards 
    circle ci
    \param[in] c cutter
    \param[in] v vector
    \param[in] ci circle
 */
p2d::SetOfPointsPair findContacts(
    const p2d::Cutter2D & c,
    const p2d::Vector & v,
    const Circle * ci
);
/*! Determines, whether specific pair is in set
    \param[in] set set, to find pair in
    \param[in] x1 x of first point
    \param[in] y1 y of first point
    \param[in] x2 x of second point
    \param[in] y2 y of second point
    \return whether pair is in set
 */
bool hasPair(const p2d::SetOfPointsPair & set,
                double x1, double y1,
                double x2, double y2);

}

}
