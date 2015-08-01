/*! \file   fuzzyequal.h
	

	A functions for fuzzy comparing of numbers with floating point are placed here
 */
#pragma once
#include "sadpoint.h"
#include "sadrect.h"
/*! A precision for comparing two floating point numbers
 */
#define S2D_FP_PRECISION  1.0E-6

namespace sad
{

/*! Tests, whether two floats are equal with precision
	\param[in] x1 first float
	\param[in] x2 second float
	\param[in] precision for comparing two numbers
	\return whether they are equal
 */
bool is_fuzzy_equal(double x1, double x2, double precision = S2D_FP_PRECISION);
/*! Tests, whether x modulo is lesser than precision
	\param[in] x a number
	\param[in] precision for comparing two numbers
	\return whether x modulo is lesser than precision
 */
bool is_fuzzy_zero(double x, double precision = S2D_FP_PRECISION);
/*! Tests, whether x modulo is greater than precision
	\param[in] x a number
	\param[in] precision for comparing two numbers
	\return whether x modulo is greater than precision
 */
bool non_fuzzy_zero(double x, double precision = S2D_FP_PRECISION);

/*! Tests, whether two points are equal (precision based)
	\param[in] p1 first point
	\param[in] p2 second point
	\param[in] precision precision for  comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Point2D & p1, const sad::Point2D & p2, float precision = S2D_FP_PRECISION);
/*! Tests, whether two points are equal (precision based)
	\param[in] p1 first point
	\param[in] p2 second point
	\param[in] precision precision for  comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Point3D & p1, const sad::Point3D & p2, float precision = S2D_FP_PRECISION);
/*! Tests, whether two rects are equal (precision based)
	\param[in] p1 first rect
	\param[in] p2 second rect
	\param[in] precision for comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Rect2D & p1, const sad::Rect2D & p2, float precision = S2D_FP_PRECISION);
/*! Tests, whether two rects are equal (precision based)
	\param[in] p1 first rect
	\param[in] p2 second rect
	\param[in] precision for comparing two numbers
	\return whether they are equal
 */
bool equal(const sad::Rect<sad::Point3D> & p1, 
		   const sad::Rect<sad::Point3D> & p2, 
		   float precision = S2D_FP_PRECISION);

}
