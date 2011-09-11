/*! \file    hshapestyle.h
    \author  HiddenSeeker
	
	\brief   Defines a abstract style of drawing primitive
	
	This file contains a definition of class of drawing primitive
*/
#include "hcolor.h"
#include "hlinestyle.h"
#pragma once

namespace hst
{

/*! \enum shapetype 
    Enum that defines a shape type
*/
enum shapetype
{
	Rect=0,       //!< Rectangle
	Ellipse=1,    //!< Ellipse
    RoundRect=2,  //!< Rounded rectangle
	Barrel=3      //!< Barrel roll. 
};

/*! \enum carvstyle
    Defines, whether primitive will be carved or not.
    Will work, if primitive is a rectangle
*/
enum carvstyle
{
	Convex=0,  //!< Convexed style
	Concave=1, //!< Concaved style
	None=2    //!< No style, draw only self
};
/*! \class  shapestyle
    \brief  Class of style of drawing primitive
	
	A basic primitive class
	
*/
class shapestyle
{
 private:
	     carvstyle m_carved; //!< Defines, whether primitive is carved
		 shapetype m_shape;  //!< Shape type
		 linestyle m_lstyle; //!< Border line style
		 acolor    m_fill;   //!< Fill color
 public: 
	    /*! Empty style
	    */
		shapestyle(); 
		/*! Parametric constructor
			\param[in] shape  shape type
			\param[in] lstyle style of line
			\param[in] fill  fill color
	        \param[in] carved carved style
		*/
	    shapestyle(shapetype shape, const linestyle & lstyle, const acolor & fill=acolor(0,0,0,255),carvstyle carved=None);

        /*! Copy constructor
			\param[in] o other style
		*/
		shapestyle(const shapestyle & o);
        /*! Assignment operator
		    \param[in] o other style
			\return self-reference
		*/
		shapestyle & operator=(const shapestyle & o);
		
		/*! Destructor
		*/
		~shapestyle();

		/*! Returns carving style
		    \return carving style
		*/
		inline carvstyle carving() const;
		/*! Returns shape type
		    \return shape type
		*/
		inline shapetype type() const;
		/*! Returns line style
		    \return line style
		*/
		inline const hst::linestyle & lineStyle() const;
		/*! Returns fill color
		    \return fill color
		*/
		inline const  acolor & fill() const;

		/*! Sets carving style
		    \param[in] s style
		*/
		inline void setCarving (carvstyle s);
		/*! Sets shape type
		    \param[in] t shape type
		*/
		inline void setType(shapetype t);
		/*! Sets line style
		    \param[in] lstyle lines style
		*/
		inline void setLineStyle(const hst::linestyle & lstyle);
		/*! Sets fill style
		    \param[in] a color
		*/
		inline void setFill(const acolor & a);
};


//Here comes source code for inlined functions
//============================================
inline carvstyle shapestyle::carving() const {return m_carved;}
inline shapetype shapestyle::type() const  {return m_shape;}
inline const hst::linestyle & shapestyle::lineStyle() const { return m_lstyle;}
inline const  acolor & shapestyle::fill() const { return m_fill;}

inline void shapestyle::setCarving (carvstyle s) {m_carved=s;}
inline void shapestyle::setType(shapetype t) {m_shape=t;}
inline void shapestyle::setLineStyle(const hst::linestyle & lstyle) {m_lstyle=lstyle;}
inline void shapestyle::setFill(const acolor & a) {m_fill=a;}
//============================================

}

/*! \fn typedef hst::shapetype hShapeType;
    Convenience typedef
*/
typedef hst::shapetype hShapeType;
/*! \fn typedef hst::carvstyle hCarvStyle;
    Convenience typedef
*/
typedef hst::carvstyle hCarvStyle;
/*! \fn typedef hst::shapestyle hShapeStyle;
    Convenience typedef
*/
typedef hst::shapestyle hShapeStyle;
