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


sad::os::SystemEventDispatcher::SystemEventDispatcher()
: m_renderer(NULL), m_keys(new sad::os::KeyDecoder())
{

}

sad::os::SystemEventDispatcher::~SystemEventDispatcher()
{
	delete m_keys;
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
	switch(e.MSG) {
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
	
}

sad::os::SystemWindowEventDispatchResult sad::os::SystemEventDispatcher::dispatch(
	sad::os::SystemWindowEvent & e
)
{
	sad::os::SystemWindowEventDispatchResult result;

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
		SL_LOCAL_INTERNAL("Triggered ActivateEvent()", *m_renderer);
		sad::input::ActivateEvent ev;
		m_renderer->controls()->postEvent(sad::input::ET_Activate, ev);
		m_renderer->window()->setActive(true);
	}
}

void sad::os::SystemEventDispatcher::processDeactivate(sad::os::SystemWindowEvent & e)
{
	if (m_renderer->window()->active() == true)
	{
		SL_LOCAL_INTERNAL("Triggered DeactivateEvent()", *m_renderer);
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
		SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseEnterEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
	}
	sad::input::MouseMoveEvent mmev;
	mmev.Point3D = op;
	m_renderer->controls()->postEvent(sad::input::ET_MouseMove, mmev);
#endif
}

void sad::os::SystemEventDispatcher::processMouseLeave(SystemWindowEvent & e)
{
#ifdef WIN32
	m_is_in_window = false;
	sad::input::MouseLeaveEvent mlev;
	m_renderer->controls()->postEvent(sad::input::ET_MouseLeave, mlev);
	SL_LOCAL_INTERNAL("Triggered MouseLeaveEvent()", *m_renderer);			
#endif
}

void sad::os::SystemEventDispatcher::processMouseWheel(SystemWindowEvent & e)
{
#ifdef WIN32
	float delta=GET_WHEEL_DELTA_WPARAM(e.WParam)/(float)WHEEL_DELTA;
	sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
	sad::Point3D viewportpoint = m_renderer->mapToViewport(p);

	sad::input::MouseWheelEvent ev;
	ev.Point3D = viewportpoint;
	ev.Delta = delta;
	m_renderer->controls()->postEvent(sad::input::ET_MouseWheel, ev);
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


void sad::os::SystemEventDispatcher::processResize(SystemWindowEvent & e)
{
#ifdef WIN32
	sad::Size2I size(LOWORD (e.LParam), HIWORD (e.LParam));
	if (e.WParam != SIZE_MINIMIZED)
	{
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
#endif
}

#ifdef WIN32

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
	SystemWindowEvent & e
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