/** \file   screenlabel.h
	\author HiddenSeeker

	Describes a screen label, which can be worked with data
 */
#include "abstractscreenobject.h"
#include "freetype/font.h"
#pragma once



/** A label on a screen, that can be used as a template for data
 */
class ScreenLabel: public AbstractScreenObject
{
 SAD_OBJECT
 private:
	 sad::String  m_font_name;    //!< Font name data
	 unsigned int m_font_size;    //!< Size of font
	 sad::Color   m_font_color;   //!< Color of font
	 sad::String  m_text;         //!< Text of label
	 float        m_angle;        //!< Angle of label
	 sad::Point2D      m_point;        //!< Upper-left point of label
	 int m_alpha;                 //!< Alpha of screen label

	 sad::freetype::Font  * m_font; //!< Font to draw
 protected:
	 /** Renders an object. NOTE: Overload this to do actual render of object
	 */
	 virtual void _render();
 public:
	 /**! Creates empty screen label
	  */
	 ScreenLabel();

	 /*! Returns a string type
			\return a string type of object
	  */
	 virtual sad::String typeName();
	 /*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual sad::String _description();
	/*! Whether we can resize object
	 */
	virtual bool resizable();
	 /*! Returns a region, where object is placed. 
	     \param[in] p point
		 \return whether point is within region
      */
	 virtual bool isWithin(const sad::Point2D & p);
	 /*! Returns a region of object. The region is not AABB
		 \return region of object
	  */
	 virtual sad::Rect2D region();
	 /*! Validates, whether object is valid in context of template.
	     This check contains, whether all resources is in template.

		 Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] db a template database, which is label is tested against
		\param[out] errors errors of objects
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * db, sad::Vector<sad::String> * errors = NULL);
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db);
	/*!	Initialize graphic part of data
		\param[in] db database
	 */
	virtual void initializeGraphicAfterLoad(FontTemplateDatabase * db);
	/*! Moves a center of object to a following point
		\param[in] p point
	 */
	virtual void moveCenterTo(const sad::Point2D & p);
};

