/*! \file objectborders.h
	\author HiddenSeeker

	Contains an object borders for some dynamic rendering
 */
#include <input.h>
#include "../../editorcore/editorbehaviourshareddata.h"
#include <config/sprite2dconfig.h>
#pragma once

class AbstractScreenObject;

// A hotspots are the places, where user can click performing some stuff
enum BorderHotSpots 
{
	BHS_LEFT = 0,
	BHS_RIGHT = 1,
	BHS_TOP = 2,
	BHS_BOTTOM = 3,
	BHS_REMOVE = 4
};

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
	 /** Creates all of hotspots for item
	  */
	 hst::vector<hRectF> createHotSpots(AbstractScreenObject * o, bool canDelete);
	 /** Determines, whether object marked by this border is removable
	     Default true
	  */
	 virtual bool removable();
	 /** Determines, whether object, marked by this border is resizable
	     Default false
	  */
	 virtual bool resizable();
 public:
	 inline ObjectBorder(EditorBehaviourSharedData  * data)
	 {
		 m_data = data;
	 }
	 /** Tests, whether hotspot point is within
	  */
	 virtual hst::vector<BorderHotSpots> isWithin(const hPointF & p, AbstractScreenObject * o);
	 /** Tests, whether is within
	  */
	 virtual hst::vector<BorderHotSpots> isWithin(const hPointF & p) = 0;
};

class ActiveObjectBorder: public ObjectBorder
{
 protected:
	 /** Determines, whether object marked by this border is deletable
	  */
	 virtual bool removable();
 public:
	 inline ActiveObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		
	 }
	 /*! Renders an active object border as blue border
		 \return success of task
	  */
	 bool tryPerform();
	 /** Tests, whether is within
	  */
	 virtual hst::vector<BorderHotSpots> isWithin(const hPointF & p);
};

class SelectedObjectBorder: public ObjectBorder
{
 protected:
    /** Determines, whether object marked by this border is deletable
	  */
	 virtual bool resizable();
 public:
	 inline SelectedObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		 
	 }
	 /*! Renders a selected object border as render border
		 \return success of task
	  */
	 bool tryPerform();
	 /** Tests, whether is within
	  */
	 virtual hst::vector<BorderHotSpots> isWithin(const hPointF & p);
};


