/*! \file   hpoint.h
    \author HiddenSeeker

	\brief  Definition of point.

	This file contains a definition of point.
*/
#define _USE_MATH_DEFINES
#include <math.h>
#pragma once

namespace hst
{
/*! Defines dimensions list
*/
enum dimension
{
    D2,
    D3
};
/*! \class point
    \brief template 2d point

    This class describes a 2d point
*/
template< hst::dimension dim, typename T>
class point
{

};

/*! \class point
    \brief template 2d point

    This class describes a 2d point
*/
template<typename T>
class point<hst::D2, T>
{
  private:
            T m_x;        //!< First coordinate
            T m_y;        //!< Second coordinate
  public:
            /*! Default constructor
            */
            point();
            /*! Parametric constructor
                \param[in] x coordinate
                \param[in] y coordinate
            */
            point(T x, T y);
            /*! Copy constructor
                \param[in] o other point
            */
            point(const point& o);
            /*! Assignment overload
                \param[in] o other point
            */
            point & operator=(const point& o);
            /*! Destructor
            */
            ~point();
            /*! Equiualence if all coordinates are equal
                \param[in] o other point
                \return true if equal
            */
            inline bool operator==(const point& o) const;
            /*! Equiualence if all coordinates are equal
                \param[in] o other point
                \return true if not equal
            */
            inline bool operator!=(const point& o) const;
            /*! Addition of both coordinates
                \param[in] o operand
                \return result
            */
            point operator+(const point & o) const;
            /*! Substraction of both coordinates
                \param[in] o operand
                \return result
            */
            point operator-(const point & o) const;
            /*! Multiplication of both coordinates
                \param[in] o operand
                \return result
            */
            point operator*(T o) const;
            /*! Division of both coordinates
                \param[in] o operand
                \return result
            */
            point operator/(T o) const;

           /*! Addition of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator+=(const point & o);
            /*! Substraction of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator-=(const point & o);
            /*! Multiplication of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator*=(T o);
            /*! Division of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator/=(T o);

            inline T x() const; //!< Returns first coordinate
            inline T y() const; //!< Returns second coordinate

            /*! Sets X coordinate
                \param[in] x coordinate
                \return self-reference
            */
            inline point& setX(T x);
            /*! Sets Y coordinate
                \param[in] y coordinate
                \return self-reference
            */
            inline point& setY(T y);
};
/*! \class point
    \brief template 3 point

    This class describes a 3d point
*/
template<typename T>
class point<hst::D3,T>
{
  private:
            T m_x;        //!< First coordinate
            T m_y;        //!< Second coordinate
            T m_z;        //!< Third coordinate
  public:
            /*! Default constructor
            */
            point();
            /*! Parametric constructor
                \param[in] x coordinate
                \param[in] y coordinate
                \param[in] z coordinate
            */
            point(T x, T y, T z);
            /*! Copy constructor
                \param[in] o other point
            */
            point(const point& o);
            /*! Assignment overload
                \param[in] o other point
            */
            point & operator=(const point& o);
            /*! Destructor
            */
            ~point();
            /*! Equiualence if all coordinates are equal
                \param[in] o other point
                \return true if equal
            */
            inline bool operator==(const point& o) const;
            /*! Equiualence if all coordinates are equal
                \param[in] o other point
                \return true if not equal
            */
            inline bool operator!=(const point& o) const;

            /*! Addition of both coordinates
                \param[in] o operand
                \return result
            */
            point operator+(const point & o) const;
            /*! Substraction of both coordinates
                \param[in] o operand
                \return result
            */
            point operator-(const point & o) const;
            /*! Multiplication of both coordinates
                \param[in] o operand
                \return result
            */
            point operator*(T o) const;
            /*! Division of both coordinates
                \param[in] o operand
                \return result
            */
            point operator/(T o) const;

           /*! Addition of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator+=(const point & o);
            /*! Substraction of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator-=(const point & o);
            /*! Multiplication of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator*=(T o);
            /*! Division of both coordinates
                \param[in] o operand
                \return self-reference
            */
            inline point& operator/=(T o);

            inline T x() const; //!< Returns first coordinate
            inline T y() const; //!< Returns second coordinate
            inline T z() const; //!< Returns third coordinate
            /*! Sets X coordinate
                \param[in] x coordinate
            */
            inline point& setX(T x);
            /*! Sets Y coordinate
                \param[in] y coordinate
            */
            inline point& setY(T y);
            /*! Sets Z coordinate
                \param[in] z coordinate
            */
            inline point& setZ(T z);
};


/*! Converts to polar coordinate
    \param[in] o point
    \return converted point
*/
template<class T>
point<hst::D2,T> toPolar(const point<hst::D2,T> & o);

}

/*! typedef hst::point2<int> point
 *  This typedef is provided for convenience
 */
 typedef hst::point<hst::D2,int> hPoint;
/*! typedef hst::point2<double> pointF
 *  This typedef is provided for convenience
 */
 typedef hst::point<hst::D2,double> hPointF;
/*! typedef hst::point2<double> pointf;
*   Small conveniernce define
*/
 typedef hst::point<hst::D2,double> pointf;

#ifndef HI_POINT_H
     #define  HI_POINT_H
     #include "hpoint_src.h"
#endif

/*! Converts 3d point to 2d point	
	 \param[in] p point
	 \return a new 2d point
 */
template<typename _From, typename _To>
inline 
typename hst::point<hst::D2, _To > _(
	const hst::point<hst::D3, _From > & p
)
{
	return typename hst::point<hst::D2, _To>(
		(_To)p.x(), 
		(_To)p.y()
	);
}


/*! Converts 2d point to 3d point	
	 \param[in] p point
	 \return a new 3d point
 */
template<typename _From, typename _To>
inline 
typename hst::point<hst::D3, _To> _(
	const hst::point<hst::D2, _From> & p
)
{
	return typename hst::point<hst::D3, _To>(
		(_To)p.x(), 
		(_To)p.y(),
		(_To)0.0
	);
}
