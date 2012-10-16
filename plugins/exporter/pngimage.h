/*! \file    pngimage.h
    \author  HiddenSeeker
	
	\brief   Image, which should be drawn, using Cairo and Pango library.
	
	This file contains a definition of class of image, drawed with cairo and pango.
*/
#include "stdafx.h"
#include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include <glib.h>
#include "primitives/hfont.h"
#include "primitives/hshape.h"
#include "primitives/hpolyline.h"
//#include "image_config.h"


#pragma once

namespace hst
{

/*! \class  pngimage
    \brief  Class of image, which should be drawn, using Cairo and Pango
	
	A basic image class. It uses cairo for drawing primitives and pango for
	rendering text. Also, you need a Glib library to compile it.
*/
class pngimage
{
 private:
	     cairo_surface_t * m_surface;        //!< Cairo surface, which used in drawing
		 cairo_t *         m_cr;             //!< Special cairo context function
		 int m_width;                        //!< Width of image
		 int m_height;                       //!< Height of image
		 hString m_filename;                 //!< Name of file, where it should be saved
		 
		 /*! Sets a content from other image
		    \param[in] str name of file, where it should be saved
		    \param[in] surf surface
			\param[in] cr   context
			\param[in] w    width
			\param[in] h    height
		 */
         pngimage(const hString & str,cairo_surface_t * surf, cairo_t * cr, int w ,int h);
		 /*! Creates a bunch of control points for bezier
		     \param[in] p1 first point
			 \param[in] p2 second point
			 \param[in] p3 third point
			 \param[in] p4 fourth point
			 \param[out] q1 control point 1
			 \param[out] q2 control point 2
		 */
		 void bezier(const hPointF & p1,const hPointF & p2,const hPointF & p3,const hPointF & p4,hPointF & q1,hPointF & q2);
		 /*! Draw fields of rectangle
		     \param[in] rect   rectangle
			 \param[in] field  field length
		 */
		 void drawRectFields(const hst::xyrect & rect,double field);
		 /*! Draws arc clockwise, in following rect, without stroking or closing path
		     \param[in] rect bounding rectangle
			 \param[in] ba   begin angle
			 \param[in] ea   end   angle
			 \param[in] move determines, whether we should move point before drawing
		 */
		 void addArcToContext(const hst::xyrect & rect,double ba,double ea,bool move=true);
		 /*! Returns angle of segment of ellipse, that belongs to a field
		     \param[in]  rect  bounding rectangle
			 \param[in]  field field length
		 */
		 double calcFieldEllipse(const hst::xyrect & rect,double field);
		 /*! Draws left ellipse field
		     \param[in] rect   rectangle
			 \param[in] alpha  angle, result of calcFieldEllipse
		 */
		 void drawLeftEllipseField(const hst::xyrect & rect,double alpha);
		 /*! Draws right ellipse field
		     \param[in] rect   rectangle
			 \param[in] alpha  angle, result of calcFieldEllipse
		 */
		 void drawRightEllipseField(const hst::xyrect & rect,double alpha);
		 /*! Draws arc clockwise, in following rect, without stroking or closing path
		     This function uses cairo_arc_negative to draw negative arc
		     \param[in] rect bounding rectangle
			 \param[in] ba   begin angle
			 \param[in] ea   end   angle
			 \param[in] move determines, whether we should move point before drawing
		 */
		 void addNegativeArcToContext(const hst::xyrect & rect,double ba,double ea,bool move=true);
		 /*! Converts pair string - font to marked up pango representation
		     \param[in] str  string
			 \param[in] font  font
		 */
         static hString convert(const hString & str, const font & font);
		 /*! Draws on image, using pango
		     \param[in] p       coordinate
			 \param[in] result  string,marked up
		 */
		 void drawLayout(const pointf & p , const hString & result);
		 /*! Gets size of layout
		     \param[in] result  marked up string
		 */
		 xyrect sizeOf(const hString & result);
		 /*! Sets a basic linestyle
		     \param[in] line linestyle
		 */
		 void setLineStyle(const linestyle & line);
		 /*! Sets a color
		     \param[in] clr color
		 */
		 void setColor(const acolor & clr);
		 /*! Strokes
		 */
		 void stroke();
		 /*! Draws arrow lines
		     \param[in] plast        pre-last points
			 \param[in] last        last points
			 \param[in] arrow_len   length of arrow
			 \param[in] arrow_angle angle of arrow
		 */
		 void drawArrowLines(const pointf & plast, const pointf & last, double arrow_len,double arrow_angle );
		 /*!  Shifts an rectangle, to fit it in image
		      \param[in] rect rectangle
		 */
		 void shift(xyrect & rect );
		 /*! Draw an ellipse
		     \param[in] rect rectangle
		 */
		 void drawEllipse(const xyrect & rect);
		 /*! Draws an ellipse fields
		     \param[in] rect bounding rect index
			 \param[in] field field length
		 */
		 void drawEllipseField(const xyrect & rect,double field);
		 /*! Draw a rect
		     \param[in] rect bounding rectangle
		 */
		 void drawRect(const xyrect & rect);
		 /*! Draw a round rect
		     \param[in] rect bounding rectangle
		 */
		 void drawRoundRect(const xyrect & rect);
		 /*! Draws a fields for barrels
		     \param[in] rect rectangle
			 \param[in] field field
		 */
		 void drawBarrelFields(const xyrect & rect,double field);
		 /*! Draw a barrel
		     \param[in] rect bounding rectangle
		 */
		 void drawBarrel(const xyrect & rect);
		 /*! Makes color slightly lighter. Often can make it more transparent
		     \param[in] clr color
			 \return lightly color
		 */
		 acolor increaseColor(const acolor & clr);
		 /*! Makes color slightly darker. Often can make it more transparent
		     \param[in] clr color
			 \return darker color
		 */
		 acolor decreaseColor(const acolor & clr);
		 /*! Concaves, drawing four lines
		     \param[in] rect rectangle
			 \param[in] clr  color
		 */
		 void concave(const xyrect & rect, const acolor & clr);
		 /*! Convexes, drawing four lines
		     \param[in] rect rectangle
			 \param[in] clr  color
		 */
		 void convex(const xyrect & rect, const acolor & clr);
		 /*! Draws a bezier line, through points
		     \param[in] p1  first point
			 \param[in] p2  second point
			 \param[in] p3  third point
			 \param[in] p4  fourth point
		 */
		 void drawBezier(const pointf & p1,const pointf & p2, const pointf & p3, const pointf & p4);
public:
	    /*! Creates an image with specified sizes
		    \param[in] szx    width of image
			\param[in] szy    height of image
			\param[in] name   filename
		*/
	    pngimage(const hString & name,int szx,int szy);
		/*! Creates an image from file
		    \param[in] fn filename
		*/
		pngimage(const hString & fn);
		/*! Simple destructor
		*/
		~pngimage();             
	    /*! Saves file to png
		*/
		void save();
		/*! Draw some text without styles, using pango
		    \param[in] p     coordinate of upper left corner of text
			\param[in] text  marked up text, without styles
		*/
		void drawText(const pointf & p,const MarkedupText & text);
		/*! Returns a size of marked up text without styles
		    \param[in] text marked up text
		*/
		static xyrect sizeOf(const MarkedupText & text);
#ifdef IMAGE_WITH_STYLES
		/*! Draw some text with styles, using pango
		    \param[in] p    coordinate of upper left corner text
			\param[in] text marked up text, with styles
		*/
		void drawText(const pointf & p,const MarkedText & text);
		/*! Returns a size of marked up text with styles
		    \param[in] text marked up text
		*/
		static xyrect sizeOf(const MarkedText & text);
#endif
		/*! Draws a polyline
		    \param[in] line        polyline, to be drawn
			\param[in] arrow_len   length of arrow
			\param[in] arrow_angle angle of arrow
		*/
		void drawLine(const polyline & line,double arrow_len=ARROW_LEN,double arrow_angle=ARROW_ANGLE);
		/*! Draws a shape
		    \param[in] shp    shape to be drawn
			\param[in] field  redrawn field, that will drawn line coloe
		*/
		void drawShape(const shape & shp,double field=0);
		/*! Draws an arc clockwise
		    \param[in] rect  bounding rectangle
			\param[in] an1   beginning angle
			\param[in] an2   end angle
			\param[in] lstyle style of line
		*/
		void drawArc( const hst::xyrect & rect,
				      double an1,
				      double an2,
				      const linestyle & lstyle
			        );
		/*! Loads image from file
		    \param[in] fn file
			\return true, if load was successful
		*/
		bool load(const hString & fn);
		/*! Sets new filename
		    \param[in] fn newfilename
		*/
		void setFileName(const hString & fn);
		/*! Returns a filename
		    \return filename
		*/
		const hString & fileName() const;
		/*! Merges two images horizontally
		    \param[in] to   filename, where it should be merged
		    \param[in] img1 left image
			\param[in] img2 right image
		*/
		static pngimage  mergeHorizontal(const hString & to,pngimage & img1, pngimage & img2);
		/*! Merges two images vertical
		    \param[in] to   filename, where it should be merged
			\param[in] img1 left image
			\param[in] img2 right image
		*/
		static pngimage  mergeVertical(const hString & to,pngimage & img1, pngimage & img2);
		/*! Returns width
		    \return width
		*/
		int width() const;
		/*! Returns height
		    \return height
		*/
		int height() const;
};

}
