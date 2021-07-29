/*! \file matrix3x3.h
    

    Defines a 3x3 matrix, used for multiplication and rotation operations in sprites
    and rectangles
 */
#pragma once
#include "sadpoint.h"
#include "sadpair.h"
#include <cmath>

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
	    const double cosa = cos(alpha);
	    const double sina = sin(alpha);
	    const double cost = cos(theta);
	    const double sint = sin(theta);
        return {
	                                  cosa, -1 * sina * cost,      sina * sint,
                                      sina,      cosa * cost, -1 * cosa * sint,
                                       0.0,             sint,             cost
        };
    }
    

    /*! Returns a value by position
        \param[in] i index of row in matrix
        \param[in] j index of column in matrix
        \return value, stored in matrix (0 if out of range)
     */
    T get(unsigned int i , unsigned int j) const
    {
        if (i >= 3 || j >= 3) return static_cast<T>(0);
        return m_o[i][j];
    }

    /*! Returns a value by position
        \param[in] i index of value in matrix
        \return value, stored in matrix (0 if out of range)
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
sad::Point3<T> 
operator*
(
 const sad::Point3<T> & p,
 const sad::Matrix3x3<T> & m
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
    return sad::Point3<T>(x, y, z);
}



/*! Multiplies a matrix by a point, resulting a point. A current rotation functions use
    this multiplication to apply transformations.
    \param[in] p point
    \param[in] m matrix
    \return point
 */
template<typename T>
sad::Point3<T> 
operator*
(
 const sad::Matrix3x3<T> & m,
 const sad::Point3<T> & p
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
    return sad::Point3<T>(x, y, z);
}
