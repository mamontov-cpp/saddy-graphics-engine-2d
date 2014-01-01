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
