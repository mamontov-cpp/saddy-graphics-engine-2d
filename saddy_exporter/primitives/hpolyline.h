/*! \file    hpolyline.h
    \author  HiddenSeeker
	
	\brief   Definition of polyline.
	
	This file contains a definition of polyline drawing parameters.
*/
#include "hcolor.h"
#include "hpoint.h"
#include "hlinestyle.h"
#include "../templates/hdeque.hpp"
#pragma once

//! Defines a length of arrow part
#define ARROW_LEN 11.0
//! Defines an angle for arrow leaves drawing
#define ARROW_ANGLE 0.34
//! Length of small arrows
#define ULTRASMALL_ARROW_LEN 6.0
//! Angle between small arrowed lines
#define ULTRASMALL_ARROW_ANGLE 0.30
//! Coefficient of anchor width increasing radius
#define ANCHOR_RAD 1.3


namespace hst
{
/*! \class polyline
    \brief class of polyline

	Polyline class
*/
class polyline
{
 private:
	      hdeque<pointf> m_points;  //!< Points list, line come through them		  
		  linestyle      m_style;   //!< Style
 public:
		 /*! Default constructor
		 */
	    polyline();
		 /*! Parametric constructor
		     \param[in] list   list of points
			 \param[in] style  style of line
	     */
		 polyline(const hdeque<pointf> & list, const linestyle  & style);
		 /*! Copy constructor
		     \param[in] o other object
		 */
		 polyline(const polyline & o);
		 /*! Assignment operator
		     \param[in] o other objecdt
		 */
		 polyline & operator=(const polyline & o);
		 /*! Default destructor
		 */
		 ~polyline();

		 /*! Returns point list
		     \return constant reference to point list
		 */
		 inline const hdeque<pointf> & points() const;
		 /*! Returns point list
		     \return reference to point list
		 */
		 inline hdeque<pointf> & points();

		 /*! Returns style
		     \return constant referencre to style
		 */
		 inline const linestyle & style() const;
		 /*! Returns style
		     \return referencre to style
		 */
		 inline linestyle & style();
};

//Here comes some inlined functions
//=====================================
inline const hdeque<pointf> & polyline::points() const {return m_points;}
inline hdeque<pointf> &       polyline::points()       {return m_points;}

inline const linestyle &      polyline::style() const {return m_style;}
inline linestyle &            polyline::style()       {return m_style;}
//=====================================

}

/*! \fn typedef hst::polyline hPolyLine;
   Small typedef for convenience
*/
typedef hst::polyline hPolyLine;