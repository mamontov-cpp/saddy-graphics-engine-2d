#include "os/systemeventdispatcher.h"

#include "renderer.h"
#include "mainloop.h"
#include "window.h"
#ifndef _WIN32
    #include "glcontext.h"
#endif

#include "log/log.h"

#include "os/systemwindowevent.h"
#include "os/keydecoder.h"

#include "input/events.h"
#include "input/controls.h"

#ifndef _WIN32
    #include <cstring>
#endif

#ifdef WIN32
#include <windowsx.h>
#endif


//#define EVENT_LOGGING

sad::os::SystemEventDispatcher::SystemEventDispatcher() //-V730
: m_renderer(nullptr),
m_decoder_for_keypress_events(new sad::os::KeyDecoder()),
m_decoder_for_keyrelease_events(new sad::os::KeyDecoder())
#ifdef WIN32
, m_is_in_window(false)
#endif
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
    sad::MaybePoint3D pt = m_renderer->cursorPosition();
    m_is_in_window = pt.exists();
    // Force window to track mouse leave
    if (m_is_in_window)
    {
        TRACKMOUSEEVENT e;
        e.cbSize = sizeof(TRACKMOUSEEVENT);
        e.dwFlags = TME_LEAVE;
        e.hwndTrack = m_renderer->window()->handles()->WND;
        e.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&e);

        sad::input::MouseEnterEvent ev;
        ev.Point = sad::Point2D(pt.value().x(), pt.value().y());
        ev.Point3D = pt.value();
        m_renderer->mainLoop()->pushDispatch([=]() {
            m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseEnter, ev);
        });
    }
    sad::Rect2I  r = m_renderer->window()->rect();
    m_old_window_size = sad::Size2I(static_cast<unsigned int>(r.width()), static_cast<unsigned>(r.height()));
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
        case WM_MOUSELEAVE:
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
        default: break;
    };
    return result;
}

#endif

sad::Point2D  sad::os::SystemEventDispatcher::toClient(const sad::Point2D& p) const
{
#ifndef WIN32
    return p;
#else
    RECT window; 
    GetWindowRect(m_renderer->window()->handles()->WND, &window);
    POINT begin;
    begin.x = 0; 
    begin.y = 0;
    ClientToScreen(m_renderer->window()->handles()->WND, &begin);
    LONG x_offset = begin.x - window.left;
    LONG y_offset = begin.y - window.top;
    return {p.x() + x_offset, p.y() + y_offset};
#endif
}

#ifdef X11

void sad::os::SystemEventDispatcher::reset()
{
    m_alt_is_held = false;
    m_in_doubleclick = false;
    m_doubleclick_timer.start();
    sad::Rect2I  r = m_renderer->window()->rect();
    m_old_window_size = sad::Size2I(r.width(), r.height());

    sad::MaybePoint3D pnt = m_renderer->cursorPosition();
    if (pnt.exists())
    {
        sad::input::MouseEnterEvent ev;
        ev.Point = sad::Point2D(pnt.value().x(), pnt.value().y()); // Dumb, but what else?
        ev.Point3D = pnt.value();
        m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseEnter, ev);
    }
}

sad::os::SystemWindowEventDispatchResult sad::os::SystemEventDispatcher::dispatch(
    sad::os::SystemWindowEvent & e
)
{
    sad::os::SystemWindowEventDispatchResult result;
    XEvent & xev = e.Event;
    sad::String atomname;
    char* rawatomname  = nullptr;
    switch(xev.type)
    {
        case ClientMessage:
            rawatomname = XGetAtomName(
                m_renderer->window()->handles()->Dpy,
                xev.xclient.message_type
            );
            atomname = rawatomname;
            XFree(rawatomname);
            if (atomname == "WM_PROTOCOLS")
            {
                processQuit(e);
            }
            break;
        case MappingNotify:
            XRefreshKeyboardMapping(&xev.xmapping);
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
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_Quit, ev);
#ifdef _WIN32
        });
#endif
        m_renderer->mainLoop()->stop();
    }
}


void sad::os::SystemEventDispatcher::processActivate(sad::os::SystemWindowEvent & e)
{
    if (!m_renderer->window()->active())
    {
#ifdef EVENT_LOGGING
        SL_LOCAL_INTERNAL("Triggered ActivateEvent()", *m_renderer);
#endif
        sad::input::ActivateEvent ev;
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_Activate, ev);
#ifdef _WIN32
        });
#endif
        m_renderer->window()->setActive(true);
    }
}

void sad::os::SystemEventDispatcher::processDeactivate(sad::os::SystemWindowEvent & e)
{
    if (m_renderer->window()->active())
    {
#ifdef EVENT_LOGGING
        SL_LOCAL_INTERNAL("Triggered DeactivateEvent()", *m_renderer);
#endif
        sad::input::ActivateEvent ev;
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_Deactivate, ev);
#ifdef _WIN32
        });
#endif
        m_renderer->window()->setActive(false);
    }
}

void sad::os::SystemEventDispatcher::processMouseMove(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
    sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
    sad::Point3D op = m_renderer->mapToViewport(p);
    if (!m_is_in_window)
    {
        m_is_in_window = true;

        // Force window to track data
        TRACKMOUSEEVENT trackmouseevent;
        trackmouseevent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackmouseevent.dwFlags = TME_LEAVE;
        trackmouseevent.hwndTrack = m_renderer->window()->handles()->WND;
        trackmouseevent.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&trackmouseevent);

        sad::input::MouseEnterEvent ev;
        ev.Point = this->toClient(p);
        ev.Point3D = op;
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
            m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseEnter, ev);
#ifdef _WIN32
        });
#endif
#ifdef EVENT_LOGGING
        SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseEnterEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
#endif
    }
    sad::input::MouseMoveEvent mmev;
    mmev.Point = this->toClient(p);
    mmev.Point3D = op;
#ifdef EVENT_LOGGING
        SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseMoveEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
#endif
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseMove, mmev);
#ifdef _WIN32
        });
#endif
#endif
#ifdef X11
    sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
    sad::Point3D op = m_renderer->mapToViewport(p);
    sad::input::MouseMoveEvent mmev;
    mmev.Point = this->toClient(p);
    mmev.Point3D = op;
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseMove, mmev);
#endif
}

void sad::os::SystemEventDispatcher::processMouseLeave(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
    m_is_in_window = false;
#endif
    sad::input::MouseLeaveEvent mlev;
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseLeave, mlev);
#ifdef _WIN32
    });
#endif
#ifdef EVENT_LOGGING
    SL_LOCAL_INTERNAL("Triggered MouseLeaveEvent()", *m_renderer);
#endif
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::os::SystemEventDispatcher::processMouseWheel(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
    double delta=GET_WHEEL_DELTA_WPARAM(e.WParam)/static_cast<double>(WHEEL_DELTA);
    sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
    sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif


#ifdef X11
    m_in_doubleclick = false;
    double delta = 0;
    if (e.Event.xbutton.button == Button4)
    {
        delta  = m_renderer->controls()->wheelTickSensitivity();
    }
    else
    {
        // e.Event.xbutton.button  should be Button5 here
        delta  = m_renderer->controls()->wheelTickSensitivity() * -1;
    }
    sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
    sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

    sad::input::MouseWheelEvent ev;
    ev.Point = this->toClient(p);
    ev.Point3D = viewportpoint;
    ev.Delta = delta;
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseWheel, ev);
#ifdef _WIN32
    });
#endif
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
            || size.Height != m_old_window_size.Height)
        {
            sad::input::ResizeEvent ev;
            ev.OldSize = m_old_window_size;
            ev.NewSize = size;
#ifdef _WIN32
            m_renderer->mainLoop()->pushDispatch([=]() {
#endif
            m_renderer->controls()->postEvent(sad::input::EventType::ET_Resize, ev);
            m_renderer->reshape(size.Width, size.Height);
            m_old_window_size = size;
#ifdef _WIN32
            });
#endif
        }
    }
    else
    {
        m_renderer->window()->setMinimized(true);
    }
#endif

#ifdef X11
    sad::Size2I size(e.Event.xconfigure.width,e.Event.xconfigure.height);
#ifdef EVENT_LOGGING
    SL_LOCAL_INTERNAL(
        fmt::Format("Triggered Resize({0}, {1}), ({2}, {3})")
        << size.Width
        << size.Height
        << m_old_window_size.Width
        << m_old_window_size.Height,
        *m_renderer
    );
#endif
    if (size.Width != m_old_window_size.Width
            || size.Height != m_old_window_size.Height)
    {
        sad::input::ResizeEvent ev;
        ev.OldSize = m_old_window_size;
        ev.NewSize = size;
        m_renderer->controls()->postEvent(sad::input::EventType::ET_Resize, ev);
        m_renderer->reshape(size.Width, size.Height);
        m_old_window_size = size;
    }
#endif
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::os::SystemEventDispatcher::processKeyPress(
    sad::os::SystemWindowEvent & e
)
{
    sad::KeyboardKey key = m_decoder_for_keypress_events->decode(&e);
    sad::input::KeyPressEvent ev;
    ev.ReadableKey = m_decoder_for_keypress_events->convert(&e, m_renderer->window());
    ev.Key = key;
#ifdef WIN32
    short ctrlstate = GetAsyncKeyState(VK_CONTROL);
    short lctrlstate =  GetAsyncKeyState(VK_LCONTROL);
    short rctrlstate =  GetAsyncKeyState(VK_RCONTROL);
    ev.CtrlHeld  = ctrlstate < 0 || lctrlstate < 0 || rctrlstate < 0;
    ev.AltHeld   = GetAsyncKeyState(VK_MENU) < 0;
    ev.ShiftHeld = GetAsyncKeyState(VK_SHIFT) < 0 || GetAsyncKeyState(VK_LSHIFT) < 0 || GetAsyncKeyState(VK_RSHIFT) < 0;
#endif

#ifdef X11
    if (ev.Key == sad::KeyboardKey::LeftAlt || ev.Key == sad::KeyboardKey::RightAlt)
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
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_KeyPress, ev);
#ifdef _WIN32
    });
#endif
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
    if (ev.Key == sad::KeyboardKey::LeftAlt || ev.Key == sad::KeyboardKey::RightAlt)
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
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_KeyRelease, ev);
#ifdef _WIN32
    });
#endif
}

void sad::os::SystemEventDispatcher::processMousePress(sad::os::SystemWindowEvent & e)
{
    sad::Maybe<sad::input::MouseDoubleClickEvent> maybedcev;
#ifdef WIN32
    sad::MouseButton btn = sad::MouseButton::MouseLeft;
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(e.MSG)
    {
	    case WM_RBUTTONDOWN: btn = sad::MouseButton::MouseRight; break;
		case WM_MBUTTONDOWN: btn = sad::MouseButton::MouseMiddle; break;
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

    sad::MouseButton btn = sad::MouseButton::MouseLeft;
    switch(e.Event.xbutton.button)
    {
        case Button1: btn = sad::MouseButton::MouseLeft; break;
        case Button2: btn = sad::MouseButton::MouseMiddle; break;
        case Button3: btn = sad::MouseButton::MouseRight; break;
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
                dlclev.Point = this->toClient(p);
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
    ev.Point = this->toClient(p);
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
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MousePress, ev);
#ifdef _WIN32
    });
#endif
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
#ifdef _WIN32
        m_renderer->mainLoop()->pushDispatch([=]() {
#endif
        m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseDoubleClick, maybedcev.value());
#ifdef _WIN32
        });
#endif
    }

}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::os::SystemEventDispatcher::processMouseRelease(sad::os::SystemWindowEvent & e)
{
#ifdef WIN32
    sad::MouseButton btn = sad::MouseButton::MouseLeft;
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(e.MSG)
    {
        case WM_RBUTTONUP: btn = sad::MouseButton::MouseRight; break;
        case WM_MBUTTONUP: btn = sad::MouseButton::MouseMiddle; break;
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
    sad::MouseButton btn = sad::MouseButton::MouseLeft;
    switch(e.Event.xbutton.button)
    {
        case Button2: btn = sad::MouseButton::MouseRight; break;
        case Button3: btn = sad::MouseButton::MouseMiddle; break;
    };
    sad::Point2D p(e.Event.xbutton.x, e.Event.xbutton.y);
    sad::Point3D viewportpoint = m_renderer->mapToViewport(p);
#endif

    sad::input::MouseReleaseEvent ev;
    ev.Point = this->toClient(p);
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
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseRelease, ev);
#ifdef _WIN32
    });
#endif
}


#ifdef X11

void sad::os::SystemEventDispatcher::processMouseEnter(sad::os::SystemWindowEvent & e)
{
    sad::Point2D p(e.Event.xcrossing.x, e.Event.xcrossing.y);
    sad::Point3D op = m_renderer->mapToViewport(p);
    sad::input::MouseEnterEvent ev;
    ev.Point = this->toClient(p);
    ev.Point3D = op;
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseEnter, ev);
#ifdef EVENT_LOGGING
    SL_LOCAL_INTERNAL(fmt::Format("Triggered MouseEnterEvent({0}, {1}, {2})") << op.x() << op.y() << op.z(), *m_renderer);
#endif
}

#endif

#ifdef WIN32


// ReSharper disable once CppMemberFunctionMayBeConst
void sad::os::SystemEventDispatcher::processMouseDoubleClick(sad::os::SystemWindowEvent & e)
{
    sad::MouseButton btn = sad::MouseButton::MouseLeft;
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(e.MSG)
    {
        case WM_RBUTTONDBLCLK: btn = sad::MouseButton::MouseRight; break;
        case WM_MBUTTONDBLCLK: btn = sad::MouseButton::MouseMiddle; break;
    };
    sad::Point2D p(GET_X_LPARAM(e.LParam), GET_Y_LPARAM(e.LParam));
    sad::Point3D viewportpoint = m_renderer->mapToViewport(p);

    sad::input::MouseDoubleClickEvent ev;
    ev.Point = this->toClient(p);
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
#ifdef _WIN32
    m_renderer->mainLoop()->pushDispatch([=]() {
#endif
    m_renderer->controls()->postEvent(sad::input::EventType::ET_MouseDoubleClick, ev);
#ifdef _WIN32
    });
#endif
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
) const
{

    sad::os::SystemWindowEventDispatchResult  result;
    result.setValue(DefWindowProcA(e.WND, e.MSG, e.WParam, e.LParam));
    if (!m_renderer->window()->fixed())
        return result;
    bool isusertriestoresize = false;
    for (long windowresizepoint : windowresizepoints)
        isusertriestoresize = isusertriestoresize || (windowresizepoint == result.value());

    if (isusertriestoresize)
    {
        result = HTBORDER;
    }

    return result;
}

#endif
