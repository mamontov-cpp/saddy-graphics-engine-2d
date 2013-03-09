/*! \file objectborders.h
	\author HiddenSeeker

	Contains an object borders for some dynamic rendering
 */
#include <input.h>
#include "../../editorcore/editorbehaviourshareddata.h"
#include <config/sprite2dconfig.h>
#pragma once

class AbstractScreenObject;

class ObjectBorder: public sad::RepeatingTask
{
 protected:
	 EditorBehaviourSharedData * m_data; //!< Data for rendering object
	 /** Miscellaneos function for drawing a sprite at specified position of image
	  */
	 void renderSpot(const char * c, const hRectF & r, int r0, int r1, int r2, int r3, float dangle);
	 /** Renders hot spots of data
		 \param[in] o object
		 \param[in] canDelete where we can delete some data
	  */
	 void renderHotSpots(AbstractScreenObject * o, bool canDelete);
 public:
	 inline ObjectBorder(EditorBehaviourSharedData  * data)
	 {
		 m_data = data;
	 }
};

class ActiveObjectBorder: public ObjectBorder
{
 public:
	 inline ActiveObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		
	 }
	 /*! Renders an active object border as blue border
		 \return success of task
	  */
	 bool tryPerform();
};

class SelectedObjectBorder: public ObjectBorder
{
 public:
	 inline SelectedObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		 
	 }
	 /*! Renders a selected object border as render border
		 \return success of task
	  */
	 bool tryPerform();
};


