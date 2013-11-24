/*! \file events.h
	\author HiddenSeeker

	This file contains implementations for all window events, that are performed in engine.
	Events can contain useful data about cursor position, key part and other.
 */
#pragma once
#include "../keycodes.h"
#include "../sadpoint.h"

namespace sad
{

namespace input
{

/*! An abstract event, which is base for all events of engine.
	Can be used for marking or run-time check of events
 */
class AbstractEvent
{
public:
	/*! Kept, for purpose of inheritance
	 */
	virtual ~AbstractEvent();
};

/*! This type of events raised, when sad::Renderer exits sad::Renderer::run()
 */
class QuitEvent: public sad::input::AbstractEvent
{
public:
	/*! Constructs new quit event
	 */
	QuitEvent();
	/*! Kept, for purpose of inheritance
	 */
	virtual ~QuitEvent();
};

/*! This type of events raised, when user presses or releases key
 */
class KeyEvent: public sad::input::AbstractEvent
{
public:
	sad::KeyboardKey Key;   //!< Code of key, pressed by user
	bool   AltHeld;		    //!< True, if user held ALT key, when pressed
	bool   ShiftHeld;	    //!< True, if user held SHIFT key, when pressed
	bool   CtrlHeld;	    //!< True, if user held CTRL key, when pressed  
	bool   CapsLockPressed; //!< True, if user pressed caps lock
	
	/*! Creates new empty key event
	 */
	inline KeyEvent() 
	: Key(sad::KeyNone), 
	  AltHeld(false), 
	  ShiftHeld(false), 
	  CtrlHeld(false), 
	  CapsLockPressed(false)
	{
	
	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~KeyEvent();
};

/*! A basic event for all mouse events, like sad::input::MouseEvent,
	sad::input::WheelEvent
 */ 
class MouseCursorEvent: public sad::input::AbstractEvent
{
public:
	/*! A point in 3-dimensional space, for those, who don't use orthographic projections
	 */
	sad::Point3D Point3D;

	inline MouseCursorEvent() : Point3D()
	{

	}
	/*! Kept, for purpose of inheritance
	 */	
	virtual ~MouseCursorEvent();
	/*! Sets a point for event. This method is used by renderer to set coordinates of
		cursor. 

		Also it fills a 2-dimensional point field automatically
		
		\param[in] p 3-dimensional point
	 */
	inline void setPoint(const sad::Point3D & p)
	{
		this->Point3D = p;
	}
	/*! Returns position of cursor in 2D space
		\return position of cursor
	 */
	inline const sad::Point2D & pos2D()
	{
		return this->Point3D;
	}

};

/*! This type of events raised, when user moves a mouse, 
	presses a mouse button, releases a mouse button or performs a double click
 */
class MouseEvent: public sad::input::MouseCursorEvent
{
public:
	sad::MouseButton  Button;  //!< A button, which is pressed or held by user

	/*! Inits event with no mouse pressed and zero points
	 */
	inline MouseEvent() : MouseCursorEvent(), Button(sad::MouseNone) 
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~MouseEvent();
};

/*! This type of events raised, when user moves a wheel
 */
class WheelEvent: public sad::input::MouseCursorEvent
{
public:
	/*! How much wheel has scrolled. Bigger than zero, means, that wheel was scrolled up,
		Lesser than zero, means, that wheel was scrolled down
	 */
	double Delta;  

	/*! Inits wheel event with zero delta
	 */
	inline WheelEvent() : MouseCursorEvent(), Delta(0)
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~WheelEvent();
};

/*! Event, which is raised, when window is being resized, maximized or minimized 
 */
class ResizeEvent: public sad::input::AbstractEvent
{
public:
	unsigned int OldWidth;   //!< A previous width of window
	unsigned int OldHeight;  //!< A previous height of window
	unsigned int NewWidth;   //!< A new width of window
	unsigned int NewHeight;  //!< A new height of window

	/*! A Constructs new event
	 */
	inline ResizeEvent() 
	: OldWidth(0), OldHeight(0), NewWidth(0), NewHeight(0)
	{

	}
	/*! Kept, for purpose of inheritance
	 */
	virtual ~ResizeEvent();
};

}

}
