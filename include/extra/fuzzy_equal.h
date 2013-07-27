/*! \file   fuzzy_equal.h
	\author HiddenSeeker

	A fuzzy equal functions is placed here
 */
#pragma once

/*! A precision for comparing two floating point numbers
 */
#define S2D_FP_PRECISION  1.0E-6

/*! Tests, whether two floats are equal with fuzzy
	\param[in] x1 first float
	\param[in] x2 second float
	\return whether they are equal
 */
bool is_fuzzy_equal(double x1, double x2, double precision = S2D_FP_PRECISION);
/*! Tests, whether x modulo is lesser than precision
	\param[in] x a number
	\return whether x modulo is lesser than precision
 */
bool is_fuzzy_zero(double x, double precision = S2D_FP_PRECISION);
/*! Tests, whether x modulo is greater than precision
	\param[in] x a number
	\return whether x modulo is greater than precision
 */
bool non_fuzzy_zero(double x, double precision = S2D_FP_PRECISION);
