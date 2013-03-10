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
protected:
	/*! A config to work with
	 */
	hst::string m_config;
	/** A group to work with
	 */
	hst::string m_group;
	/** Defines an index of sprite in group
	 */
	int  m_index;
	/** A rectangle to work with
		\param[in] rect rectangle
	 */
	hRectF		m_rect;
	/** An angle to work with
	 */
	float m_angle;
	/** Observer, used to render object
	 */
	Sprite2DConfigObserver * m_observer;
	/** Renders a screen sprite
	 */
	virtual void _render();
 public:
	ScreenSprite();
	/*! Returns a string type
		\return a string type of object
	 */
	virtual hst::string type();
	/*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual hst::string description();
};