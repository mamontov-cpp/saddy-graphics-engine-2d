/*! \file matrix2x2.h
	\author HiddenSeeker

	Defines a 2x2 matrix, used for mutiplication and rotation operations in sprites
	and rectangles
 */
#pragma once
#include "primitives/hpoint.h"
#include "sadpair.h"
#include <assert.h>

namespace sad 
{
/*! A 2x2 matrix can be used for multiplication and rotation operations in sprites
    and rectangles
 */
template<typename T>
class Matrix2x2
{
private:
	T m_o[2][2]; //!< An array used to store all of data
public:
	typedef sad::Pair<unsigned int, unsigned int> index;
	/*! Constructs a new matrix
		\param[in] a11 upper left element
		\param[in] a12 upper right element
		\param[in] a21 lower left element
		\param[in] a22 lower right element
	 */
	Matrix2x2(T a11, T a12, T a21, T a22)
	{
		m_o[0][0] = a11; 
		m_o[0][1] = a12;
		m_o[1][0] = a21;
		m_o[1][1] = a22;
	}
	/*! Constructs a counter-clockwise rotation matrix for angle
		\param[in] angle angle for rotation
		\return matrix
	 */
	static Matrix2x2  counterclockwise(T angle)
	{
		T cosa = cos(angle);
		T sina = sin(angle);
		return Matrix2x2(cosa, sina, 
			             -sina, cosa);
	}
	/*! Constructs a clockwise rotation matrix for angle
		\param[in] angle angle for rotation
		\return matrix
	 */
	static Matrix2x2  clockwise(T angle)
	{
		return sad::Matrix2x2<T>::counterclockwise(-angle);
	}

	/*! Returns a point by parameters
	 */
	T operator[](const typename Matrix2x2<T>::index & i) const
	{
		if (i.p1() >= 2 || i.p2() >= 2) return (T)0;
		return m_o[i.p1()][i.p2()];
	}
};

}
/*! Multiplies a point by matrix
	\param[in] p point
	\param[in] m matrix
	\return point
 */
template<typename T>
typename hst::point<hst::D2, T> 
operator*
(
const typename hst::point<hst::D2, T> & p,
const typename sad::Matrix2x2<T> & m
)
{
	T x = p.x() * m[typename sad::Matrix2x2<T>::index(0,0)] 
	    + p.y() * m[typename sad::Matrix2x2<T>::index(1,0)];
	T y = p.x() * m[typename sad::Matrix2x2<T>::index(0,1)] 
	    + p.y() * m[typename sad::Matrix2x2<T>::index(1,1)];
	return typename hst::point<hst::D2, T>(x,y);
}

