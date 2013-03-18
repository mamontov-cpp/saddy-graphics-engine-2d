/** \file   screenlabel.h
	\author HiddenSeeker

	Describes a screen label, which can be worked with data
 */
#include "abstractscreenobject.h"
#include "ftfont.h"
#pragma once



/** A label on a screen, that can be used as a template for data
 */
class ScreenLabel: public AbstractScreenObject
{
 private:
	 hst::string  m_font_name;    //!< Font name data
	 unsigned int m_font_size;    //!< Size of font
	 hst::color   m_font_color;   //!< Color of font
	 hst::string  m_text;         //!< Text of label
	 float        m_angle;        //!< Angle of label
	 hPointF      m_point;        //!< Upper-left point of label

	 FTFont               * m_font; //!< Font to draw
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
	 virtual hst::string typeName();
	 /*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual hst::string _description();
	/*! Whether we can resize object
	 */
	virtual bool resizable();
	 /*! Returns a region, where object is placed. 
	     \param[in] p point
		 \return whether point is within region
      */
	 virtual bool isWithin(const hPointF & p);
	 /*! Returns a region of object. The region is not AABB
		 \return region of object
	  */
	 virtual hRectF region();
	 /*! Validates, whether object is valid in context of template.
	    This check contains, whether all resources is in template.

		Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] t template
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * db);
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db);
	/*! Moves a center of object to a following point
		\param[in] p point
	 */
	virtual void moveCenterTo(const hPointF & p);
};

