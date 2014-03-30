/** \file   screensprite.h
	\author HiddenSeeker

	Describes a screen sprite object
 */
#include "abstractscreenobject.h"
#include <config/sprite2dconfigloader.h>
#include <config/sprite2dobserver.h>
#pragma once


class ScreenSprite: public AbstractScreenObject 
{
SAD_OBJECT
protected:
	/*! A color to work with
	 */
	sad::Color  m_color;
	/*! An alpha channel to work with
	 */
	int         m_alpha;
	/*! A config to work with
	 */
	sad::String m_config;
	/** A group to work with
	 */
	sad::String m_group;
	/*! Defines an index of sprite in group
	 */
	int  m_index;
	/*! A rectangle to work with
		\param[in] rect rectangle
	 */
	sad::Rect2D		m_rect;
	/** An angle to work with
	 */
	float m_angle;
	/** Observer, used to render object
	 */
	sad::Sprite2DConfigObserver * m_observer;
	/** Renders a screen sprite
	 */
	virtual void _render();
 public:
	ScreenSprite();
	/*! Returns angle data
		\return angle 
	 */
	inline float angle() const { return m_angle;}
	/*! Returns bounding rectangle for data
		\return bounding rectangle
	 */
	inline const sad::Rect2D & rect() const { return m_rect;}

	inline sad::Sprite2DConfigObserver * observer() { return m_observer; }
	/*! Returns a string type
		\return a string type of object
	 */
	virtual sad::String typeName();
	/*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual sad::String _description();
	/*! Returns a region, where object is placed. 
	     \param[in] p point
		 \return whether point is within region
      */
	virtual bool isWithin(const sad::Point2D & p);
	 /*! Validates, whether object is valid in context of template.
	    This check contains, whether all resources is in template.

		Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] db a template database, which is label is tested against
		\param[out] errors errors of objects
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * db, sad::Vector<sad::String> * errors = NULL);
	/*!	Initialize graphic part of data
		\param[in] db database
	 */
	virtual void initializeGraphicAfterLoad(FontTemplateDatabase * db);
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual void reloadNoSize(FontTemplateDatabase * db);
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db);
	/*! Returns a region for data
	 */
	virtual sad::Rect2D region();
	/*! Sets a point for data
	 */
	virtual void moveCenterTo(const sad::Point2D & point);
	/*! Sets an already rotated rectangle, restoring all data
	 */
	virtual void setRotatedRectangle(const sad::Rect2D & rotatedrectangle, float angle);
	/*! Destroys a screen sprite
	 */
	~ScreenSprite();
};