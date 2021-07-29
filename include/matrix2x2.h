/*! \file matrix2x2.h
    

    Defines a 2x2 matrix, used for multiplication and rotation operations in sprites
    and rectangles
 */
#pragma once
#include "sadpoint.h"
#include "sadpair.h"

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
        return Matrix2x2(cosa, -sina, 
                         sina, cosa);
    }
    /*! Constructs a clockwise rotation matrix for angle
        \param[in] angle angle for rotation
        \return matrix
     */
    static Matrix2x2  clockwise(T angle)
    {
        return sad::Matrix2x2<T>::counterclockwise(-angle);
    }

    /*! Returns a value, stored in matrix by index
        \param[in] i index of element in matrix
        \return value
     */
    T operator[](const typename Matrix2x2<T>::index & i) const
    {
        return this->get(i.p1(), i.p2());
    }
    /*! Returns a value by index
        \param[in] i index of row in matrix
        \param[in] j index of column in matrix
        \return value of matrix, 0 on error
     */
    T get(unsigned int i, unsigned int j) const
    {
        if (i >= 2 || j >= 2) return static_cast<T>(0);
        return m_o[i][j];
    }
};

}

/*! Multiplies a point by matrix. Note that behaves not as canonical multiplication, as
    point behaves like  single-row matrix multiplied by a matrix. 
    \param[in] p point
    \param[in] m matrix
    \return point
 */
template<typename T>
sad::Point2<T> 
operator*
(
const sad::Point2<T> & p,
const sad::Matrix2x2<T> & m
)
{
    T x = p.x() * m.get(0, 0) 
        + p.y() * m.get(1, 0);
    T y = p.x() * m.get(0, 1) 
        + p.y() * m.get(1, 1);
    return sad::Point2<T>(x,y);
}

/*! Multiplies a matrix by a point. A current rotation functions use
    this multiplication to apply transformations.
    \param[in] m matrix
    \param[in] p point
    \return point
 */
template<typename T>
sad::Point2<T> 
operator*
(
const sad::Matrix2x2<T> & m,
const sad::Point2<T> & p
)
{
    T x = m.get(0, 0) * p.x() 
        + m.get(0, 1) * p.y();
    T y = m.get(1, 0) * p.x() 
        + m.get(1, 1) * p.y();
    return sad::Point2<T>(x,y);
}

