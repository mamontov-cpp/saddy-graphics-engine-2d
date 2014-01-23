/*! \file matrix3x3.h
	\author HiddenSeeker

	Defines a 3x3 matrix, used for mutiplication and rotation operations in sprites
	and rectangles
 */
#pragma once
#include "sadpoint.h"
#include "sadpair.h"
#include <assert.h>
#include <math.h>

namespace sad
{
/*! A 3x3 matrix can be used for multiplication and rotation operations in sprites
    and rectangles
 */
template<typename T>
class Matrix3x3
{
private:
	T m_o[3][3]; //!< An array used to store all of data
public:
	typedef sad::Pair<unsigned int, unsigned int> index;
	/*! Constructs a new matrix
		\param[in] a11 element at first row, first column
		\param[in] a12 element at first row, second column
		\param[in] a13 element at first row, third column
		\param[in] a21 element at second row, first column
		\param[in] a22 element at second row, second column
		\param[in] a23 element at second row, third column
		\param[in] a31 element at third row, first column
		\param[in] a32 element at third row, second column
		\param[in] a33 element at third row, third column
	 */
	Matrix3x3(T a11, T a12, T a13,
			  T a21, T a22, T a23,
			  T a31, T a32, T a33)
	{
		m_o[0][0] = a11; 
		m_o[0][1] = a12;
		m_o[0][2] = a13;
		m_o[1][0] = a21;
		m_o[1][1] = a22;
		m_o[1][2] = a23;
		m_o[2][0] = a31;
		m_o[2][1] = a32;
		m_o[2][2] = a33;
	}

	/*! Rotates counter-clockwise rotation sequentially around X and Z axis
		\param[in] alpha rotation angle around Z axis
		\param[in] theta rotation angle around X axis
	 */
	static sad::Matrix3x3<double> rotationXandZ(double alpha, double theta)
	{
		double cosa = cos(alpha);
		double sina = sin(alpha);
		double cost = cos(theta);
		double sint = sin(theta);
		return sad::Matrix3x3<double>(			
									  cosa, -1 * sina * cost,      sina * sint,
									  sina,      cosa * cost, -1 * cosa * sint,
									   0.0,             sint,             cost);
	}
	

	/*! Returns point by index
		\param[in] i index
		\return index
	 */
	T get(unsigned int i , unsigned int j) const
	{
		if (i >= 3 || j >= 3) return (T)0;
		return m_o[i][j];
	}

	/*! Returns a point by parameters
		\param[in] i index
		\return index
	 */
	T operator[](const typename Matrix3x3<T>::index & i) const
	{
		return get(i.p1(), i.p2());
	}
};
	
}

/*! Multiplies a point as single-row matrix by matrix.
	\param[in] p point
	\param[in] m matrix
	\return point
 */
template<typename T>
typename sad::Point3<T> 
operator*
(
 const typename sad::Point3<T> & p,
 const typename sad::Matrix3x3<T> & m
)
{
	T x = p.x() * m.get(0, 0) 
	    + p.y() * m.get(1, 0)
		+ p.z() * m.get(2, 0);
	T y = p.x() * m.get(0, 1)  
	    + p.y() * m.get(1, 1) 
		+ p.z() * m.get(2, 1);
	T z = p.x() * m.get(0, 2)  
	    + p.y() * m.get(1, 2) 
		+ p.z() * m.get(2, 2);
	return typename sad::Point3<T>(x, y, z);
}



/*! Multiplies a matrix by a point, resulting a point. Used by rotation of 3D sprite
	\param[in] p point
	\param[in] m matrix
	\return point
 */
template<typename T>
typename sad::Point3<T> 
operator*
(
 const typename sad::Matrix3x3<T> & m,
 const typename sad::Point3<T> & p
)
{
	T x = m.get(0, 0) * p.x() 
	    + m.get(0, 1) * p.y()
		+ m.get(0, 2) * p.z();
	T y = m.get(1, 0) * p.x() 
	    + m.get(1, 1) * p.y()
		+ m.get(1, 2) * p.z();
	T z = m.get(2, 0) * p.x() 
	    + m.get(2, 1) * p.y()
		+ m.get(2, 2) * p.z();
	return typename sad::Point3<T>(x, y, z);
}
