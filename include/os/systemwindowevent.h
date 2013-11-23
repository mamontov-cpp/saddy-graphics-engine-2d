/*! \file systemwindowevent.h
	\author HiddenSeeker

	Describes an event, received by window as structure for representing all data,
	that other classes needed to dispatch it
 */
#pragma once
#include "windowhandles.h"
#include "../sadpoint.h"
#include "../sadsize.h"

namespace sad
{

namespace os
{

/*! An event, received by window as structre for representing all data, that
	other classes needed to dispatch it
 */
struct SystemWindowEvent
{
public:

#ifdef WIN32
	/*! Creates new window event with specified parameters
		\param[in] wnd window handle parameters
		\param[in] msg window message type
		\param[in] wparam first window parameter type
		\param[in] lparam second window parameter type
 	 */
	SystemWindowEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif

#ifdef X11
	/*! Creates new window event with specified parameters
		\param[in] event x11 event
	 */
	SystemWindowEvent(XEvent x11event);
#endif
	/*! Converts mouse position from event to point
		\return mouse position from event 
	 */
	sad::Point2D mousePositionToPoint() const;
	/*! Converts size event properties to size
		\return size 
	 */
	sad::Size2D toSize() const;
#ifdef WIN32
	/*! Window handle parameters 
	 */
	HWND  WND;
	/*! Window message type
	 */
	UINT  MSG;
	/*! First window parameter type
	 */
	WPARAM WParam;
	/*! Second window parameter type
	 */
	LPARAM LParam;
#endif

#ifdef X11
	/*! A boxed X11 window event type
	 */
	XEvent Event;
#endif

};

}

}