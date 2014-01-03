#include "os/systemeventdispatcher.h"

#include "renderer.h"
#include "mainloop.h"
#include "window.h"
#include "glcontext.h"

#include "log/log.h"

#include "os/systemwindowevent.h"
#include "os/keydecoder.h"

#include "input/events.h"
#include "input/controls.h"

#include <cstring>

#ifdef WIN32
#include <windowsx.h>
#endif


//#define EVENT_LOGGING

sad::os::SystemEventDispatcher::SystemEventDispatcher()
: m_renderer(NULL), 
m_decoder_for_keypress_events(new sad::os::KeyDecoder()),
m_decoder_for_keyrelease_events(new sad::os::KeyDecoder())
#ifdef X11
, m_alt_is_held(false),
m_in_doubleclick(false)
#endif
{

}

sad::os::SystemEventDispatcher::~SystemEventDispatcher()
{
	delete m_decoder_for_keypress_events;
	delete m_decoder_for_keyrelease_events;
}

void sad::os::SystemEventDispatcher::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::os::SystemEventDispatcher::renderer() const
{
	return m_renderer;
}

#ifdef WIN32

void sad::os::SystemEventDispatcher::reset()
{
	m_is_in_window = m_renderer->cursorPosition().exists();
	// Force window to track mouse leave
	if (m_is_in_window)
	{
		TRACKMOUSEEVENT e;
		e.cbSize = sizeof(TRACKMOUSEEVENT);
		e.dwFlags = TME_LEAVE;
		e.hwndTrack = m_renderer->window()->handles()->WND;
		e.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&e);
	}
	sad::Rect2I  r = m_renderer->window()->rect();  
	m_old_window_size = sad::Size2I(r.width(), r.height());
}

sad::os::SystemWindowEventDispatchResult sad::os::SystemEventDispatcher::dispatch(
	sad::os::SystemWindowEvent & e
)
{
	sad::os::SystemWindowEventDispatchResult result;
	switch(e.MSG) 
	{
		case WM_QUIT: 
		case WM_CLOSE:
			processQuit(e); break;
		case WM_ACTIVATE:
			if (LOWORD(e.WParam) == WA_ACTIVE || LOWORD(e.WParam) == WA_CLICKACTIVE)
			{
				processActivate(e);
			}
			else
			{
				processDeactivate(e);
			}
			break;
		case WM_MOUSEMOVE:
			processMouseMove(e);
			break;
		case WM_NCMOUSELEAVE:
			processMouseLeave(e);
			break;
		case WM_MOUSEWHEEL:
			processMouseWheel(e);
			break;
		case WM_KEYDOWN:
			processKeyPress(e);
			break;
		case WM_KEYUP:
			processKeyRelease(e);
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			processMousePress(e);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			processMouseRelease(e);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
			processMouseDoubleClick(e);
			break;
		case WM_SIZE:
			processResize(e);
			break;
		case WM_NCHITTEST:
			result = processHitTest(e);
			break;		
	};
	return result;
}

#endif

#ifdef X11

void sad::os::SystemEventDispatcher::reset()
{
	m_alt_is_held = false;	
	m_in_doubleclick = false;
	m_doubleclick_timer.start();
	sad::Rect2I  r = m_renderer->window()->rect();  
	m_old_window_size = sad::Size2I(r.width(), r.height());
}

sad::os::SystemWindowEventDispatchResult sad::os::SystemEventDispatcher::dispatch(
	sad::os::SystemWindowEvent & e
)
{
	sad::os::SystemWindowEventDispatchResult result;
	XEvent & xev = e.Event;
	sad::String atomname;
	switch(xev.type)
	{
		case ClientMessage:
			atomname = XGetAtomName(
				m_renderer->window()->handles()->Dpy, 
				xev.xclient.message_type
			);
			if (atomname == "WM_PROTOCOLS")
			{
				processQuit(e);
			}
			break;
		case ConfigureNotify:
			processResize(e);
			break;
		case MapNotify:
			m_renderer->window()->setMinimized(false);
			break;
		case UnmapNotify:
			m_renderer->window()->setMinimized(true);
			break;
		case FocusIn:
			processActivate(e);
			break;
		case FocusOut:
			processDeactivate(e);
			break;
		case EnterNotify:
			processMouseEnter(e);
			break;
		case LeaveNotify:
			processMouseLeave(e);
			break;
		case MotionNotify:
			processMouseMove(e);
			break;
		case ButtonPress:
			processMousePress(e);
			break;
		case ButtonRelease:
			processMouseRelease(e);
			break;
		case KeyPress:
			processKeyPress(e);
			break;
		case KeyRelease:
			processKeyRelease(e);
			break;
	};
	return result;
}

#endif

void sad::os::SystemEventDispatcher::processQuit(sad::os::SystemWindowEvent & e)
{
	if (m_renderer->running())
	{
		sad::input::QuitEvent ev;
		m_renderer->controls()->postEvent(sad::input::ET_Quit, ev);
		m_renderer->mainLoop()->stop();
	}
}


void sad::os::SystemEventDispatcher::processActivate(sad::os::SystemWindowEvent & e)
{
	if (m_renderer->window()->active() == false)
	{
#ifdef EVENT_LOGGING
		SL_LOCAL_INTERNAL("Triggered ActivateEvent()", *m_renderer);
#endif
		sad::input::ActivateEvent ev;
		m_renderer->controls()->postEvent(sad::input::ET_Activate, ev);
		m_renderer->window()->setActive(true);
	}
}

void sad::os::SystemEventDispatcher::processDeactivate(sad::os::SystemWindowEvent & e)
{
	if (m_renderer->window()->active() == true)
	{
#ifdef EVENT_LOGGING
		SL_LOCAL_INTERNAL("Triggered DeactivateEvent()", *m_renderer);
#endif
		sad::input::ActivateEvent ev;
		m_renderer->controls()->postEvent(sad::input::ET_Deactivate, ev);
		m_renderer->window()->setActive(false);
	}
}

void sad::os::SystemEventDispatcher::processMouseMove(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D op = m_renderer->mapToViewport(p);
	if (m_is_in_window == false)
	{
		m_is_in_window = true;

		// Force window to track data
		TRACKMOUSEEVENT e;
		e.cbSize = sizeof(TRACKMOUSEEVENT);
		e.dwFlags = TME_LEAVE;
		e.hwndTrack = m_renderer->window()->handles()->WND;
		e.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&e);

		sad::input::MouseEnterEvent ev;
		ev.Point3D = op;
		m_renderer->controls()->postEvent(sad::input::ET_MouseEnter, ev);
#ifdef EVENT_LOGGING
		SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseEnterEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
#endif
	}
	sad::input::MouseMoveEvent mmev;
	mmev.Point3D = op;
	m_renderer->controls()->postEvent(sad::input::ET_MouseMove, mmev);
#endif
#ifdef X11
	sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
	sad::Point3D op = m_renderer->mapToViewport(p);	
	sad::input::MouseMoveEvent mmev;
	mmev.Point3D = op;
	m_renderer->controls()->postEvent(sad::input::ET_MouseMove, mmev);
#endif
}

void sad::os::SystemEventDispatcher::processMouseLeave(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
	m_is_in_window = false;
#endif
	sad::input::MouseLeaveEvent mlev;
	m_renderer->controls()->postEvent(sad::input::ET_MouseLeave, mlev);
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL("Triggered MouseLeaveEvent()", *m_renderer);			
#endif
}

void sad::os::SystemEventDispatcher::processMouseWheel(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
	double delta=GET_WHEEL_DELTA_WPARAM(e.WParam)/(double)WHEEL_DELTA;
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif


#ifdef X11
	m_in_doubleclick = false;
	double delta = 0;
	if (e.Event.xbutton.button == Button4)
	{
		delta  = m_renderer->controls()->wheelTickSensivity();
	}
	else
	{
		// e.Event.xbutton.button  should be Button5 here
		delta  = m_renderer->controls()->wheelTickSensivity() * -1;
	}
	sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

	sad::input::MouseWheelEvent ev;
	ev.Point3D = viewportpoint;
	ev.Delta = delta;
	m_renderer->controls()->postEvent(sad::input::ET_MouseWheel, ev);
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered MouseWheelEvent({0}, [{1}, {2}, {3}])") 
		<< delta
		<< viewportpoint.x() 
		<< viewportpoint.y() 
		<< viewportpoint.z(), 
		*m_renderer
	);
#endif	
}


void sad::os::SystemEventDispatcher::processResize(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
	sad::Size2I size(LOWORD (e.LParam), HIWORD (e.LParam));
	if (e.WParam != SIZE_MINIMIZED)
	{
		m_renderer->window()->setMinimized(false);
		if (size.Width != m_old_window_size.Width 
			&& size.Height != m_old_window_size.Height)
		{
			sad::input::ResizeEvent ev;
			ev.OldSize = m_old_window_size;
			ev.NewSize = size;
			m_renderer->controls()->postEvent(sad::input::ET_Resize, ev);
			m_renderer->reshape(size.Width, size.Height);
		}
	}
	else
	{
		m_renderer->window()->setMinimized(true);
	}
#endif

#ifdef X11
	sad::Size2I size(e.Event.xconfigure.width,e.Event.xconfigure.height);
	if (size.Width != m_old_window_size.Width 
			&& size.Height != m_old_window_size.Height)
	{
		sad::input::ResizeEvent ev;
		ev.OldSize = m_old_window_size;
		ev.NewSize = size;
		m_renderer->controls()->postEvent(sad::input::ET_Resize, ev);
		m_renderer->reshape(size.Width, size.Height);
	}
#endif
}

void sad::os::SystemEventDispatcher::processKeyPress(
	sad::os::SystemWindowEvent & e
)
{
	sad::KeyboardKey key = m_decoder_for_keypress_events->decode(&e);
	sad::input::KeyPressEvent ev;
	ev.ReadableKey = m_decoder_for_keypress_events->convert(&e, m_renderer->window());	
	ev.Key = key;
#ifdef WIN32
	ev.CtrlHeld  = GetAsyncKeyState(VK_CONTROL) < 0;
	ev.AltHeld   = GetAsyncKeyState(VK_MENU) < 0;
	ev.ShiftHeld = GetAsyncKeyState(VK_SHIFT) < 0;	
#endif

#ifdef X11
	if (ev.Key == sad::LeftAlt || ev.Key == sad::RightAlt)
	{
		m_alt_is_held = true;
	}
	ev.CtrlHeld  = (e.Event.xkey.state & ControlMask) != 0;
	ev.AltHeld   = m_alt_is_held;
	ev.ShiftHeld = (e.Event.xkey.state & ShiftMask) != 0;
#endif
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered KeyPressEvent({0}, {1}, [{2}, {3}, {4}])") 
		<< ev.Key
		<< (ev.ReadableKey.exists() ? ev.ReadableKey.value() : "non-printable") 
		<< ev.CtrlHeld 
		<< ev.AltHeld
		<< ev.ShiftHeld, 
		*m_renderer
	);
#endif
	m_renderer->controls()->postEvent(sad::input::ET_KeyPress, ev);
}

void sad::os::SystemEventDispatcher::processKeyRelease(
	sad::os::SystemWindowEvent & e
)
{
	sad::KeyboardKey key = m_decoder_for_keyrelease_events->decode(&e);
	sad::input::KeyReleaseEvent ev;
	ev.ReadableKey = m_decoder_for_keyrelease_events->convert(&e, m_renderer->window());	
	ev.Key = key;
#ifdef WIN32
	ev.CtrlHeld  = GetAsyncKeyState(VK_CONTROL) < 0;
	ev.AltHeld   = GetAsyncKeyState(VK_MENU) < 0;
	ev.ShiftHeld = GetAsyncKeyState(VK_SHIFT) < 0;	
#endif


#ifdef X11
	if (ev.Key == sad::LeftAlt || ev.Key == sad::RightAlt)
	{
		m_alt_is_held = false;
	}
	ev.CtrlHeld  = (e.Event.xkey.state & ControlMask) != 0;
	ev.AltHeld   = m_alt_is_held;
	ev.ShiftHeld = (e.Event.xkey.state & ShiftMask) != 0;
#endif

#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered KeyReleaseEvent({0}, {1}, [{2}, {3}, {4}])") 
		<< ev.Key
		<< (ev.ReadableKey.exists() ? ev.ReadableKey.value() : "non-printable") 
		<< ev.CtrlHeld 
		<< ev.AltHeld
		<< ev.ShiftHeld, 
		*m_renderer
	);
#endif
	m_renderer->controls()->postEvent(sad::input::ET_KeyRelease, ev);	
}

void sad::os::SystemEventDispatcher::processMousePress(sad::os::SystemWindowEvent & e)
{
	sad::Maybe<sad::input::MouseDoubleClickEvent> maybedcev;
#ifdef WIN32
	sad::MouseButton btn = sad::MouseLeft;
	switch(e.MSG) 
	{
		case WM_RBUTTONDOWN: btn = sad::MouseRight; break;
		case WM_MBUTTONDOWN: btn = sad::MouseMiddle; break;
	};
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

#ifdef X11
	// Separate an dispatch wheel events
	if (e.Event.xbutton.button == Button4 || e.Event.xbutton.button == Button5)
	{
		processMouseWheel(e);
		return;
	}

	sad::MouseButton btn = sad::MouseLeft;
	switch(e.Event.xbutton.button)
	{
		case Button1: btn = sad::MouseLeft; break;
		case Button2: btn = sad::MouseMiddle; break;
		case Button3: btn = sad::MouseRight; break;
	};

	sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);

	if (m_in_doubleclick)
	{
		m_in_doubleclick = false;
		if (btn == m_doubleclick_button)
		{
			m_doubleclick_timer.stop();
			if (m_doubleclick_timer.elapsed() <= m_renderer->controls()->doubleClickSensivity())
			{
				sad::input::MouseDoubleClickEvent dlclev;
				dlclev.Point3D = viewportpoint;
				dlclev.Button = btn;
				maybedcev.setValue(dlclev);
			}
		}
	}
	else
	{
		m_in_doubleclick = true;
		m_doubleclick_button = btn;
		m_doubleclick_timer.start();
	}
#endif

	

	sad::input::MousePressEvent ev;
	ev.Point3D  = viewportpoint;
	ev.Button = btn;
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered MousePressEvent({0}, [{1}, {2}, {3}])") 
		<< btn
		<< viewportpoint.x() 
		<< viewportpoint.y() 
		<< viewportpoint.z(), 
		*m_renderer
	);
#endif
	m_renderer->controls()->postEvent(sad::input::ET_MousePress, ev);

	// Pose double click if need to
	if (maybedcev.exists())
	{
#ifdef EVENT_LOGGING
		SL_LOCAL_INTERNAL(
			fmt::Format("Triggered MouseDoubleClickEvent({0}, [{1}, {2}, {3}])") 
			<< btn
			<< viewportpoint.x() 
			<< viewportpoint.y() 
			<< viewportpoint.z(), 
			*m_renderer
		);
#endif	
		m_renderer->controls()->postEvent(sad::input::ET_MouseDoubleClick, maybedcev.value());
	}

}

void sad::os::SystemEventDispatcher::processMouseRelease(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
	sad::MouseButton btn = sad::MouseLeft;
	switch(e.MSG) 
	{
		case WM_RBUTTONUP: btn = sad::MouseRight; break;
		case WM_MBUTTONUP: btn = sad::MouseMiddle; break;
	};
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

#ifdef X11
	if (e.Event.xbutton.button != Button1 
		&& e.Event.xbutton.button != Button2 
		&& e.Event.xbutton.button != Button3)
	{
		return;
	}
	sad::MouseButton btn = sad::MouseLeft;
	switch(e.Event.xbutton.button) 
	{
		case Button2: btn = sad::MouseRight; break;
		case Button3: btn = sad::MouseMiddle; break;
	};
	sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

	sad::input::MouseReleaseEvent ev;
	ev.Point3D  = viewportpoint;
	ev.Button = btn;
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered MouseReleaseEvent({0}, [{1}, {2}, {3}])") 
		<< btn
		<< viewportpoint.x() 
		<< viewportpoint.y() 
		<< viewportpoint.z(), 
		*m_renderer
	);
#endif
	m_renderer->controls()->postEvent(sad::input::ET_MouseRelease, ev);	
}


#ifdef X11

void sad::os::SystemEventDispatcher::processMouseEnter(sad::os::SystemWindowEvent & e)
{
	sad::Point2D p(e.Event.xcrossing.x, e.Event.xcrossing.y);
	sad::Point3D op = m_renderer->mapToViewport(p);
	sad::input::MouseEnterEvent ev;
	ev.Point3D = op;
	m_renderer->controls()->postEvent(sad::input::ET_MouseEnter, ev);
#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseEnterEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
#endif
}

#endif

#ifdef WIN32


void sad::os::SystemEventDispatcher::processMouseDoubleClick(sad::os::SystemWindowEvent & e)
{
	sad::MouseButton btn = sad::MouseLeft;
	switch(e.MSG) 
	{
		case WM_RBUTTONDBLCLK: btn = sad::MouseRight; break;
		case WM_MBUTTONDBLCLK: btn = sad::MouseMiddle; break;
	};
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);

	sad::input::MouseDoubleClickEvent ev;
	ev.Point3D  = viewportpoint;
	ev.Button = btn;

#ifdef EVENT_LOGGING
	SL_LOCAL_INTERNAL(
		fmt::Format("Triggered MouseDoubleClickEvent({0}, [{1}, {2}, {3}])") 
		<< btn
		<< viewportpoint.x() 
		<< viewportpoint.y() 
		<< viewportpoint.z(), 
		*m_renderer
	);
#endif
	m_renderer->controls()->postEvent(sad::input::ET_MouseDoubleClick, ev);	
}

// A window resize array
static const int windowresizepointscount = 8;
static  LRESULT windowresizepoints[windowresizepointscount] = {
	HTTOPLEFT,
	HTTOP,
	
	HTTOPRIGHT,
	HTRIGHT,
	HTLEFT,

	HTBOTTOMLEFT,
	HTBOTTOM,
	HTBOTTOMRIGHT
};

sad::os::SystemWindowEventDispatchResult  sad::os::SystemEventDispatcher::processHitTest(
	sad::os::SystemWindowEvent & e
)
{

	sad::os::SystemWindowEventDispatchResult  result;
	result.setValue(DefWindowProcA(e.WND, e.MSG, e.WParam, e.LParam));
	if (m_renderer->window()->fixed() == false)
		return result;
	bool isusertriestoresize = false;
	for(int i = 0; i < windowresizepointscount; i++)
		isusertriestoresize = isusertriestoresize || (windowresizepoints[i] == result.value());
		
	if (isusertriestoresize)
	{
		result = HTBORDER;
	}
		
	return result;
}

#endif