/*! \file objectborders.h
	\author HiddenSeeker

	Contains an object borders for some dynamic rendering
 */
#include <pipeline/pipelineprocess.h>
#include "shared.h"
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

class ObjectBorder: public sad::pipeline::AbstractProcess
{
public:
	 inline ObjectBorder(core::Shared  * data)
	 {
		 m_data = data;
	 }
	 /** Tests, whether hotspot point is within
	  */
	 virtual sad::Vector<BorderHotSpots> isWithin(const sad::Point2D & p, AbstractScreenObject * o);
	 /** Tests, whether is within
	  */
	 virtual sad::Vector<BorderHotSpots> isWithin(const sad::Point2D & p) = 0;
protected:
	core::Shared * m_data; //!< Data for rendering object
	/** Miscellaneos function for drawing a sprite at specified position of image
	*/
	void renderSpot(const char * c, const sad::Rect2D & r, int r0, int r1, int r2, int r3, float dangle);
	/** Renders hot spots of data
		\param[in] o object
		\param[in] canDelete where we can delete some data
	*/
	void renderHotSpots(AbstractScreenObject * o, bool canDelete);
	/** Creates all of hotspots for item
	*/
	sad::Vector<sad::Rect2D> createHotSpots(AbstractScreenObject * o, bool canDelete);
	/** Determines, whether object marked by this border is removable
	     Default true
	 */
	virtual bool removable();
	/** Determines, whether object, marked by this border is resizable
	     Default false
	 */
	virtual bool resizable();
};

class ActiveObjectBorder: public ObjectBorder
{
public:
	 inline ActiveObjectBorder(core::Shared* data):ObjectBorder(data)
	 {
		
	 }
	 /** Tests, whether is within
	  */
	 virtual sad::Vector<BorderHotSpots> isWithin(const sad::Point2D & p);
protected:
	 /** Determines, whether object marked by this border is deletable
	  */
	 virtual bool removable();
 	 /*! Renders a selected object border as render border
	  */
	 void _process();
	 /*! Frees linked object if any
	  */
	 virtual ~ActiveObjectBorder();
};

class SelectedObjectBorder: public ObjectBorder
{
public:
	 inline SelectedObjectBorder(core::Shared* data):ObjectBorder(data)
	 {
		 
	 }
	 /** Tests, whether is within
	  */
	 virtual sad::Vector<BorderHotSpots> isWithin(const sad::Point2D & p);
protected:
    /** Determines, whether object marked by this border is deletable
	  */
	 virtual bool resizable();
	 /*! Renders a selected object border as render border
	  */
	 void _process();
};


