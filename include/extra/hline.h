/*! \file    hline.h
    \author  HiddenSeeker
	
	\brief   Definition of line and important structures for line collisions.
	
	This file contains a definition of axle and lines.
*/
#include "hpoint.h"
#include "hxyrect.h"
#pragma once
/*! \fn typedef float axle_t
 *  We define axle as a clockwise rotation angle around center of coordinates . axle_t a=0 is an X axle 
 */
typedef float axle_t;
/*! \struct hsline
	\brief  Straight analitical line
	
    Defines a straight line in a format of equation k1*x+k2*y=b
*/
struct hsline
{
	float k1;                //!< X coefficient
	float k2;                //!< Y coefficient
	float b;                 //!< Free term
};
/*! \class h1DLine
	\brief 1D-intercept

	Defines a one-dimension intercept
*/
class h1DLine
{
 private:
	float x1;                                //!< First coordinate
	float x2;                                //!< Second coordinate
 public:
	 h1DLine();                              //!< Default constructor with x1=0,x2=0
	 /*! Parametric constructor with x1 coordinate as minimum, x2 coordinate as maximum. No checking.
	     \param[in] x1 minimum coordinate
		 \param[in] x2 maximum coordinate
	 */
	 h1DLine(float x1,float x2);
	 /*! Copy constructor
	     \param[in] o other line
	 */
	 h1DLine(const h1DLine & o);
     /*! Assignment operator overload
         \param[in] o other line	 
	 */
	 h1DLine& operator=(const h1DLine& o);   
	 ~h1DLine();                             //!< Default destructor
	 inline float p1() const;                       //!< Returns a x1 point
	 inline float p2() const;                       //!< Returns a x2 point
	 /*! Extends a line, through setting x1 as minimum of all points of lines, 
	     x2 as maximum of lines
		 \param[in] o extending line
	 */
	 void addLine(const h1DLine & o);        
};
/*! \class hLine
	\brief 2D-Line

    Defines a two-dimension intercept
	\sa h1DLine , hsline
*/
class  hLine
{
 private:
	hPointF p1;                                     //!< First point
	hPointF p2;								        //!< Second point
 public:
	 hLine();                                       //!< Default constructor
	 /*! Parametric constructor of [p1,p2] intercept
	     \param[in] p1 first point
		 \param[in] p2 second point
	 */
	 hLine(const hPointF & p1,const hPointF & p2);  
	 /*! Copy constructor
	     \param[in] o other line
	 */
	 hLine(const hLine & o);                     
	 /*! Assignment operator overload
	     \param[in] o other line
	 */
	 hLine& operator=(const hLine& o);          
	 inline const hPointF & point1() const;         //!< Simply returns a point1
	 inline const hPointF & point2() const;         //!< Simply returns a point2
	 axle_t getAxle()    const;                     //!< Returns the axis, where rests the intercept       
	 axle_t getPerAxle() const;                     //!< Returns the axis, perpendicular to rectangle 
	 /*! Proects a line to axle
	    \param[in] axle  Axle to proect to
		\return one-dimensional intercept
	 */
	 class h1DLine proect(axle_t axle) const;      
	 /*! Extracts a straight line, where an intercept rests.
	    \return a straight line struct
	 */
	 struct hsline analyze() const;
	 ~hLine();                                       //!< Default destructor
};

/*!
   Proects a rectangle to axle
   \param[in]  rect  rectangle to proect
   \param[in]  axle  axle to proect to
   \return  one-dimensional intercept, containing a proection
*/
class h1DLine  proect(axle_t axle,const hXYRect & rect);


//Here comes a source code for inlined functions
//=================================================
inline float h1DLine::p1()   const {return x1;}
inline float h1DLine::p2()   const {return x2;}
inline const hPointF & hLine::point1() const {return p1;}
inline const hPointF & hLine::point2() const {return p2;}
//=================================================
