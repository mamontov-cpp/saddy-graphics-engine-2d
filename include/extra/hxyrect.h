/*! \file    hxyrect.h
    \author  HiddenSeeker
	
	\brief   Definition of rectangle, whose sides are parallel to X and Y axis.
	
	This file contains a definition of colored rectangle, whose sides are parallel to X and Y axis.
*/
#include "../primitives/hcolor.h"
#include "../primitives/hpoint.h"
#pragma once

namespace hst
{
/*! \class xyrect
    \brief strictly orthogonal quadrangle
	
	Rectangle, whose sides are parallel to X and Y axis
    \sa   hPointF
*/
class xyrect
{
 private:
		 hPointF _p;            //!< Upper-left point of rectangle
		 double   _width;        //!< Width of rectangle
		 double   _height;       //!< Height of rectangle
 public:
	    xyrect();                                                                     //!< Default constructor of black rectangle with (0,0)-(0,0) upper-left and lower-right points
		/*! Constructs a rectangle with following parameters
         	\param[in] p      Upper-left point
			\param[in] width  Width of rectangle
			\param[in] height Height of rectangle
		*/
		xyrect(const hPointF & p,double width, double height);    
		/*! Constructs a black rectangle with following parameters
		    \param[in] p1     Upper-left point
			\param[in] p2     Lower-left point
		*/
		xyrect(const hPointF & p1, const hPointF & p2);     
		/*! Copy constructor
		    \param[in] o    rect to copy
		*/
		xyrect(const xyrect & o);                                             
		/*! Moves rectangle, by setting his _p property to p
		    \param[in] p settings point
		*/
		inline void moveTo(const hPointF & p);
        /*! Moves rectangle, by adding p to _p property	
		    \param[in] p settings point
		*/
		inline void moveBy(const hPointF & p);                        
		/*! Returns a point of rectangle
		  Here is a map of points which can be returned, depending from i:
		  
		  \par [0] --    .-------.  -- [1]
		  \par [3] --    .-------.  -- [2]
		  
		  Otherwise it returns (0,0)
		  \param[in] i point index
		  \return point from map, or (0,0)
		*/
		inline hPointF  operator[](unsigned int i) const;
		/*! Returns a upper left point of rectangle
		    \return upper left point of rectangle
		*/
		inline const hPointF & p() const;
		/*! Returns a upper left point of rectangle
		    \return upper left point of rectangle
		*/
		inline hPointF & p();
		/*! Assignment operator overload
		    \param[in] o other rectangle
			\return self-reference
		*/
		xyrect & operator=(const xyrect & o);               
		inline double height() const;                                   //!< Returns a height of rectangle
		inline double width() const;                                    //!< Returns a width of rectangle
		/*! Returns a rectangle, enlarged on the height and width, without changing
			the coordinates of the center point. Both sides are increasing on _w and 
			_h.
			\param[in] _w width
			\param[in] _h height
			\return enlarged rectangle
		*/
		xyrect enlarged(double _w,double _h) const;
		/*! Returns a rectangle, enlarged on on both sizes, without changing
			the coordinates of the center point, by a size of _a
			\param[in] _a width and height
			\return enlarged rectangle
		*/
		xyrect enlarged(double _a) const;
		/*! Sets a height of rectangle
		    \param[in] _h new height of rectangle
		*/
		inline void setHeight(double _h);                               
		/*! Sets a width of rectangle
		    \param[in] _w new width of rectangle
		*/
		inline void setWidth(double _w); 
		/*! Compares rectangle. Equal, when all props are equal
		    \param[in] o other rectangle
			\return true if equal
		*/
		bool operator==(const hst::xyrect & o);
};



//Here goes source code, for inlined functions
//=============================================
inline double xyrect::width()  const {return _width;}
inline double xyrect::height() const {return _height;}
inline void xyrect::setHeight(double _h)           {_height=_h;}
inline void xyrect::setWidth (double _w)           {_width=_w;}
inline void xyrect::moveTo(const hPointF & p)
{
	_p=p;
}
inline void xyrect::moveBy(const hPointF & p)
{
	_p=_p+p;
}
inline const hPointF & xyrect::p() const
{
	return _p;
}

inline hPointF & xyrect::p()
{
	return _p;
}

inline hPointF xyrect::operator[](unsigned int i) const
{
	switch (i)
	{
	 case 0: {return _p;break;}
	 case 1: {return hPointF(_p.x()+_width,_p.y());break;}
	 case 2: {return hPointF(_p.x()+_width,_p.y()+_height);break;}
	 case 3: {return hPointF(_p.x(),_p.y()+_height);break;}
	};
	return hPointF();
}
//=============================================

}
/*! \fn hst::xyrect hXYRect;
    Convenience typedef
*/
typedef hst::xyrect hXYRect;
