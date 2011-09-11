/*! \file    hshape.h
    \author  HiddenSeeker
	
	\brief   Definition of graphic primitive.
	
	This file contains a definition of graphic primitive.
*/
#include "hshapestyle.h"
#include "hxyrect.h"
#pragma once

namespace hst
{
/*! \class shape
    \brief class, defining a graphic primitive

	Shape class,  defined by his style and bounding box
*/
class shape
{
 private:
	     shapestyle m_style;  //!< Style of primitive
		 xyrect    m_bbox;   //!< Bbox type
 public:
		/*! Default constructor
		*/
	    shape();
		/*! Copy constructor
		    \param[in] o other object
		*/
		shape(const shape & o);
	    /*! Parametric constructor
		    \param[in] style style of element
			\param[in] rect  bounding rectangle
		*/
	    shape(const xyrect & rect, const shapestyle & style);

		/*! Assignment overload
		    \param[in] o     other object
			\return self-reference
		*/
		shape & operator=(const shape & o);
		
		
		/*! Access to style
		    \return style
		*/
		inline const shapestyle & style() const;
		/*! Access to bounding box
		     \return box
		*/
		inline const xyrect  & box() const;

		/*! Access to style
		    \return style
		*/
		inline shapestyle & style();
		/*! Access to bounding box
		     \return box
		*/
		inline xyrect  & box();
		

		/*! Default destructor
		*/
		~shape();
};


//Here comes a place for inlined functions
//========================================
inline const shapestyle & shape::style() const { return m_style; }
inline const xyrect    &   shape::box()  const { return m_bbox;  }

inline shapestyle & shape::style() { return m_style; }
inline xyrect    &   shape::box()  { return m_bbox;  }

//========================================
}

/*! \fn typedef hst::shape hShape;
    Convenience typedef
*/
typedef hst::shape hShape;