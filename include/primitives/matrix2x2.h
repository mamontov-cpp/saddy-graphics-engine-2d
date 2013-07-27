/*! \file matrix2x2.h
	\author HiddenSeeker

	Defines a 2x2 matrix, used for mutiplication and rotation
 */
#pragma once
#include "hpoint.h"
#include "../templates/hpair.hpp"
#include <assert.h>

namespace hst 
{
/*! A 2x2 matrix can be used for multiplication and rotation
 */
template<typename T>
class matrix2x2
{
private:
	T m_o[2][2]; //!< An array used to store all of data
public:
	typedef hst::pair<unsigned int, unsigned int> index;
	/*! Constructs a new matrix
		\param[in] a11 upper left element
		\param[in] a12 upper right element
		\param[in] a21 lower left element
		\param[in] a22 lower right element
	 */
	matrix2x2(T a11, T a12, T a21, T a22)
	{
		m_o[0][0] = a11; 
		m_o[0][1] = a12;
		m_o[1][0] = a21;
		m_o[1][1] = a22;
	}
	/*! Constructs a rotation matrix for angle
		\param[in] angle angle for rotation
		\return matrix
	 */
	static matrix2x2  rotation(T angle)
	{
		float cosa = cos(angle);
		float sina = sin(angle);
		return matrix2x2(cosa, sina, -sina, cosa);
	}
	/*! Returns a point by parameters
	 */
	T operator[](const typename matrix2x2<T>::index & i) const
	{
		if (i.p1() >= 2 || i.p2() >= 2) return (T)0;
		return m_o[i.p1()][i.p2()];
	}
};

}

template<typename T>
typename hst::point<hst::D2, T> 
operator*
(
const typename hst::point<hst::D2, T> & p,
const typename hst::matrix2x2<T> & m
)
{
	T x = p.x() * m[typename hst::matrix2x2<T>::index(0,0)] 
	    + p.y() * m[typename hst::matrix2x2<T>::index(1,0)];
	T y = p.x() * m[typename hst::matrix2x2<T>::index(0,1)] 
	    + p.y() * m[typename hst::matrix2x2<T>::index(1,1)];
	return typename hst::point<hst::D2, T>(x,y);
}

