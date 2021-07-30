/*! \file axle.h
    

    Defines axle as a case of 2d vector
 */
#pragma once
#include "vector.h"
#include "../matrix2x2.h"
#include "../sadpair.h"
#include "../sadvector.h"

namespace sad
{

namespace p2d 
{
    /*! An axle is a special case of a vector
     */
    typedef p2d::Vector Axle;
    /*! A one-dimensional cutter is a pair of doubles
     */
    typedef sad::Pair<double, double> Cutter1D;
    /*! A two-dimensional cutter is a pair of points
     */
    typedef sad::Pair<sad::Point2D, sad::Point2D> Cutter2D;
    /*! A pair of points, which can be contacts 
     */
    typedef sad::Pair<sad::Point2D, sad::Point2D> PointsPair;
    /*! A matrix 2x2 for out options
     */
    typedef sad::Matrix2x2<double> Matrix2x2; 
    /*! Makes an axle from two points by sequentially creating
        a vector out of result of substraction of points and
        computing unit-vector
        \param[in] p1 first point
        \param[in] p2 second point
        \return resulting axle
     */
    p2d::Axle axle(const sad::Point2D & p1, const sad::Point2D & p2);
    /*! Projects a cutter to an axle
        \param[in] c cutter
        \param[in] a axle
        \return one-dimensional cutter
     */
    p2d::Cutter1D project(const p2d::Cutter2D & c, 
                          const p2d::Axle & a);
    /*! Projects a cutter to an axle
        \param[in] p1 first point of cutter
        \param[in] p2 second point of cutter
        \param[in] a axle
        \return one-dimensional cutter
     */
    p2d::Cutter1D project(const sad::Point2D & p1,
                          const sad::Point2D & p2,
                          const p2d::Axle & a);
    /*! Tests, whether two one-dimensional cutters collide
        \param[in] c1 first cutter
        \param[in] c2 second cutter
        \return whether they are colliding
     */
    bool collides(const p2d::Cutter1D & c1, 
                  const p2d::Cutter1D & c2);
    /*! Swaps one point with other
        \param[in] c pair
        \return pair of points
     */ 
    p2d::PointsPair swap(const p2d::PointsPair & c);
    /*! Swaps all points in pair
        \param[in,out] pairs a pairs of points
     */
    void swap(sad::Vector<p2d::PointsPair> & pairs);
    /*! Returns a cutter by two points
        \param[in] x1 x of first point
        \param[in] y1 y of first point
        \param[in] x2 x of second point
        \param[in] y2 y of second point
     */
    p2d::Cutter2D cutter(double x1, double y1, double x2, double y2);
}

}

