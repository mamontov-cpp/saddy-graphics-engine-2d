/*! \file   hrect.h
    \author HiddenSeeker

	\brief  Definition of rectangle.

	This file contains a definition of rectangle.
*/
#include "hpoint.h"
#pragma once

namespace hst
{

/*! \class rect
    \brief template 2d rectangle

    This class describes a 2d rectangle, which can be used with 2d rectangle
*/
template<class T>
class rect
{
 private:
         /*! Array of points, defining a rectangle
             \par [0]---.-----.---[1]
             \par [3]---.-----.---[2]
         */
         T  m_p[4];
 public:
	     inline const T *      a()     const { return &(m_p[0]);   }              
	     inline T *            a()           { return &(m_p[0]);   }   
		 inline const T &      p()      const { return m_p[0];}
	     inline double width()  const { return m_p[1].x()-m_p[0].x(); }
		 inline double height() const { return m_p[2].y()-m_p[1].y(); }
         /*! Default constructor
         */
         rect();
         /*! Copy constructor
             \param[in] o other rectangle
         */
         rect(const rect& o);
         /*! Assignment operator
             \param[in] o other rectangle
         */
         rect& operator=(const rect& o);
         /*! Equiualence operator
            \param[in] o other rectangle
            \return true if equal
         */
         bool operator==(const rect& o);
         /*! Equiualence operator
            \param[in] o other rectangle
            \return true if not equal
         */
         bool operator!=(const rect& o);
         /*! Default destructor
         */
         ~rect();
         /*! Creates a rectangle from two points
             \param[in] p1 first point
             \param[in] p2 second point
         */
         rect(const T & p1,const T & p2);
         /*! Creates a rectangle from two points
             \param[in] p1 first point
             \param[in] p2 second point
         */
         rect(const T & p1,const T & p2,const T & p3,const T & p4);
         /*! Returns a constant reference to a point. Here is a map:
             \par [0]---.-----.---[1]
             \par [3]---.-----.---[2]
             \param[in] i index
             \return constant reference
        */
         const T & operator[](unsigned int i) const;
         /*! Returns a reference to a point. Here is a map:
             \par [0]---.-----.---[1]
             \par [3]---.-----.---[2]
             \param[in] i index
             \return constant reference
        */
         T & operator[](unsigned int i);
};

}

/*! Convenience typedef
*/
typedef hst::rect<hst::point<hst::D2,double> > hRectF;

#ifndef HI_RECTANGLE_H
     #define  HI_RECTANGLE_H
     #include "hrect_src.h"
#endif
