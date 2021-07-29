#include "os/windowimpl.h"
#include "renderer.h"

#ifdef X11
#include <X11/Xutil.h>
#include <GL/glx.h>
#include "3rdparty/glext/glxext.h"
#endif


sad::os::WindowImpl::WindowImpl()
:
m_handles(),
m_fixed(false),
m_fullscreen(false),
m_hidden(false),
m_active(true),
m_creation_size(320, 240),
m_window_rect_stack(),
m_renderer(nullptr),
#ifdef WIN32
m_style(0),
#endif
m_window_title("Saddy Engine"),
m_minimized(false)
#ifdef X11
,m_gl3compatible(false)
#endif
{

}

sad::os::WindowImpl::~WindowImpl()
{
    if (valid())
    {
        destroy();
    }
#ifdef X11
    if (m_handles.VisualInfo != nullptr)
    {
        XFree(m_handles.VisualInfo);
        m_handles.VisualInfo  = nullptr;
    }
#endif
}

void sad::os::WindowImpl::setRenderer(sad::Renderer * renderer)
{
    m_renderer = renderer;
}

sad::Renderer * sad::os::WindowImpl::renderer() const
{
    return m_renderer;
}

void sad::os::WindowImpl::setCreationSize(const sad::Size2I& size)
{
    m_creation_size = size;
}


bool sad::os::WindowImpl::create()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::create()", this->renderer());
    // Do not do anything, when window implementation is valid
    if (valid())
        return true;

    typedef bool (sad::os::WindowImpl::*CreationStep)(bool);
#ifdef WIN32

    const int creationstepscount = 4;

    CreationStep creationsteps[creationstepscount] = {
        &sad::os::WindowImpl::registerWindowClass,
        &sad::os::WindowImpl::adjustWindowRect,
        &sad::os::WindowImpl::makeWindowAndObtainDeviceContext,
        &sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor
    };

#endif

#ifdef X11
    const int creationstepscount = 3;

    CreationStep creationsteps[creationstepscount] = {
        &sad::os::WindowImpl::openConnectionAndScreen,
        &sad::os::WindowImpl::chooseVisualInfo,
        &sad::os::WindowImpl::createWindow
    };

#endif

    bool result = true;
    for(int i = 0; i < creationstepscount; i++)
    {
        CreationStep step = creationsteps[i];
        result = result && (this->*step)(result);
    }

    if (result == false)
    {
        SL_COND_LOCAL_FATAL("Failed to create window", this->renderer());
        this->destroy();
    }


    return result;
}

#ifdef WIN32

// External loop handler
LRESULT CALLBACK sad_renderer_window_proc (
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

bool sad::os::WindowImpl::registerWindowClass(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::registerWindowClass()", this->renderer());

    if (!lastresult)
    {
        return false;
    }

    m_handles.ProcessInstance = GetModuleHandle(nullptr);

    WNDCLASSA    wc;
    wc.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) sad_renderer_window_proc;
    wc.cbClsExtra  =  0;
    wc.cbWndExtra  =  0;
    wc.hInstance   =  m_handles.ProcessInstance;
    wc.hIcon       =  LoadIcon(nullptr,IDI_WINLOGO);
    wc.hCursor     =  LoadCursor(nullptr,IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName  = nullptr;

    m_handles.Class = "sad::os::WindowImpl at ";
    char pointernamebuffer[20];
    sprintf(pointernamebuffer, "%p", this);
    m_handles.Class += sad::String(pointernamebuffer);

    wc.lpszClassName = m_handles.Class.data();

    ATOM registerresult = RegisterClassA(&wc);
    bool result = (registerresult != 0);
    if (result == false)
    {
        SL_COND_LOCAL_FATAL(
            fmt::Format("Failed to register class \"{0}\"") << m_handles.Class,
            this->renderer()
        );

        m_handles.ProcessInstance = nullptr;
        m_handles.Class.clear();
    }
    return result;

}

void sad::os::WindowImpl::unregisterWindowClass()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::unregisterWindowClass()", this->renderer());

    if (m_handles.Class.length() != 0)
    {
        UnregisterClassA(m_handles.Class.data(), m_handles.ProcessInstance);

        m_handles.Class.clear();
        m_handles.ProcessInstance = nullptr;
    }
}

bool sad::os::WindowImpl::adjustWindowRect(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::adjustWindowRect()", this->renderer());

    if (!lastresult)
    {
        return false;
    }
    // Compute window rectangle size, adjusting it to needed rectangle
    DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW;

    m_handles.AdjustedWindowRect.left  =(long)0;
    m_handles.AdjustedWindowRect.right =(long)m_creation_size.Width;
    m_handles.AdjustedWindowRect.top   =(long)0;
    m_handles.AdjustedWindowRect.bottom=(long)m_creation_size.Height;

    SL_COND_LOCAL_INTERNAL(
        fmt::Format("Requested client size is [left: {0}, right: {1}, top: {2}, bottom: {3}]")
        << m_handles.AdjustedWindowRect.left
        << m_handles.AdjustedWindowRect.right
        << m_handles.AdjustedWindowRect.top
        << m_handles.AdjustedWindowRect.bottom
        ,
        this->renderer()
    );


    AdjustWindowRectEx(&(m_handles.AdjustedWindowRect),style,FALSE,ex_style);


    SL_COND_LOCAL_INTERNAL(
        fmt::Format("Adjusted window size is [left: {0}, right: {1}, top: {2}, bottom: {3}]")
        << m_handles.AdjustedWindowRect.left
        << m_handles.AdjustedWindowRect.right
        << m_handles.AdjustedWindowRect.top
        << m_handles.AdjustedWindowRect.bottom
        , this->renderer()
    );

    return true;
}

bool sad::os::WindowImpl::makeWindowAndObtainDeviceContext(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::makeWindowAndObtainDeviceContext()", this->renderer());

    if (!lastresult)
    {
        return false;
    }
    m_handles.WND = CreateWindowExA(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        m_handles.Class.data(),
        m_window_title.data(),
        WS_OVERLAPPEDWINDOW,
        m_handles.AdjustedWindowRect.left,
        m_handles.AdjustedWindowRect.top,
        m_handles.AdjustedWindowRect.right,
        m_handles.AdjustedWindowRect.bottom,
        nullptr,
        nullptr,
        m_handles.ProcessInstance,
        nullptr
    );

    bool result = (m_handles.WND != nullptr);
    if (m_handles.WND == nullptr)
    {
        sad::String msg = str(
                fmt::Format("CreateWindowExA() failed with code {0}")
                << GetLastError()
        );
        SL_COND_LOCAL_INTERNAL(msg, this->renderer());
    }
    else
    {
        m_handles.DC = GetDC(m_handles.WND);
        if (m_handles.DC == nullptr)
        {
            result = false;
            SL_COND_LOCAL_INTERNAL("GetDC() failed", this->renderer());
        }
    }

    if (result)
    {
        SetWindowPos(
            m_handles.WND,
            HWND_NOTOPMOST,
            0,
            0,
            m_handles.AdjustedWindowRect.right - m_handles.AdjustedWindowRect.left,
            m_handles.AdjustedWindowRect.bottom - m_handles.AdjustedWindowRect.top,
            SWP_NOSENDCHANGING
        );

        RECT windowrect;
        GetWindowRect(m_handles.WND, &windowrect);
        SL_COND_LOCAL_INTERNAL(
            fmt::Format("Created window rectangle with [left: {0}, top: {1}, right: {2}, bottom: {3}]")
            << windowrect.left
            << windowrect.top
            << windowrect.right
            << windowrect.bottom,
            this->renderer()
        );

        RECT clientrect;
        GetClientRect(m_handles.WND, &clientrect);
        SL_COND_LOCAL_INTERNAL(
            fmt::Format("Created client rectangle with [left: {0}, top: {1}, right: {2}, bottom: {3}]")
            << clientrect.left
            << clientrect.top
            << clientrect.right
            << clientrect.bottom,
            this->renderer()
        );

        ShowWindow(m_handles.WND, SW_HIDE);

        m_style = GetWindowLongPtr(m_handles.WND, GWL_STYLE);
    }
    return result;
}

void sad::os::WindowImpl::releaseContextAndDestroyWindow()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::releaseContextAndDestroyWindow()", this->renderer());

    if (m_handles.DC && !ReleaseDC(m_handles.WND,m_handles.DC))
    {
        SL_COND_LOCAL_INTERNAL("ReleaseDC() failed", this->renderer());
        m_handles.DC = nullptr;
    }

    if (m_handles.WND && !DestroyWindow(m_handles.WND))
    {
        SL_COND_LOCAL_INTERNAL("DestroyWindow() failed", this->renderer());
        m_handles.WND = nullptr;
    }
#ifdef X11
    XFree(m_handles.VisualInfo);
    m_handles.VisualInfo = nullptr;
#endif
}

bool sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor()", this->renderer());

    if (!lastresult)
    {
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
        PFD_TYPE_RGBA,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;

    int      pixelformat=0; //Pixel format
    pixelformat = ChoosePixelFormat(m_handles.DC, &pfd);

    bool result = true;
    if (!pixelformat)
    {
        result = false;
        SL_COND_LOCAL_INTERNAL("ChoosePixelFormat() failed", this->renderer());
    }
    else
    {
        if (!SetPixelFormat(m_handles.DC, pixelformat, &pfd))
        {
            result = false;
            SL_COND_LOCAL_INTERNAL("SetPixelFormat() failed", this->renderer());
        }
    }
    return true;
}

#endif

#ifdef X11

bool sad::os::WindowImpl::openConnectionAndScreen(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::openConnectionAndScreen()", this->renderer());
    if (!lastresult)
    {
        return false;
    }
    XSupportsLocale();
    XSetLocaleModifiers("");
    m_handles.Dpy = XOpenDisplay(0);
    unsigned long dpy = reinterpret_cast<unsigned long>(m_handles.Dpy);
    if (m_handles.Dpy == nullptr)
    {
        m_handles.Screen = 0;
        SL_COND_LOCAL_INTERNAL("XOpenDisplay(0) failed", this->renderer());
        return false;
    }

    sad::String conmesg = str(fmt::Format("Connected to display 0 on handle {0}") << dpy);
    SL_COND_LOCAL_INTERNAL(conmesg, this->renderer());

    m_handles.Screen = DefaultScreen(m_handles.Dpy);
    sad::String defscreenmesg = str(fmt::Format("Default screen is {0}") << m_handles.Screen);
    SL_COND_LOCAL_INTERNAL(defscreenmesg, this->renderer());

    //  Try to check, whether window can be created as OpenGL3 compatible
    //  and set FBConfig
    //  implementation taken from http://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
    m_gl3compatible = false;
    int visualattribs[] =
    {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , True,
        None
    };

    int glxmajor = 0, glxminor = 0;

    // FBConfigs were added in GLX version 1.3.
     if (glXQueryVersion( m_handles.Dpy, &glxmajor, &glxminor) == True)
     {
        if (glxmajor > 1 || ((glxmajor == 1) && (glxminor >= 3)))
        {
            int fbcount = 0;
            GLXFBConfig * configs = glXChooseFBConfig(
                m_handles.Dpy,
                m_handles.Screen,
                visualattribs,
                &fbcount
            );
            sad::String message = str(fmt::Format("Found {0} matching FB configs") << fbcount);
            SL_COND_LOCAL_INTERNAL(message, this->renderer());

            int bestfbindex = pickBestFBConfig(fbcount, configs);
            m_handles.FBC = configs[ bestfbindex ];
            XFree( configs );
            m_gl3compatible = true;
        }
     }

    return true;
}

int sad::os::WindowImpl::pickBestFBConfig(int fbcount, GLXFBConfig * configs)
{
    int bestfbc = -1,
        bestnumsamp = -1;

    for (int i = 0; i < fbcount; i++ )
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig( m_handles.Dpy, configs[i] );
        if ( vi )
        {
            int sampbuf = 0, samples = 0;
            glXGetFBConfigAttrib(
                m_handles.Dpy,
                configs[i],
                GLX_SAMPLE_BUFFERS,
                &sampbuf
            );
            glXGetFBConfigAttrib(
                m_handles.Dpy,
                configs[i],
                GLX_SAMPLES,
                &samples
            );

            if ( bestfbc < 0 || sampbuf && samples > bestnumsamp )
            {
                bestfbc = i;
                bestnumsamp = samples;
            }
        }
        XFree( vi );
    }
    return bestfbc;
}

void sad::os::WindowImpl::closeConnection()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::closeConnection()", this->renderer());
    if (m_handles.Win != 0)
    {
        XDestroyWindow(m_handles.Dpy, m_handles.Win);
    }
    XFreeColormap(m_handles.Dpy, m_handles.ColorMap);
    if (m_handles.Dpy != nullptr)
    {
        XCloseDisplay(m_handles.Dpy);
    }

    m_gl3compatible = false;
}

bool sad::os::WindowImpl::chooseVisualInfo(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::chooseVisualInfo()", this->renderer());
    if (!lastresult)
    {
        return false;
    }
    bool result = true;
    // If we are compatible with GL3  - pick visual info from FBConfig
    if (m_gl3compatible)
    {
        SL_COND_LOCAL_INTERNAL(
            "Getting XVisualInfo from GLXFBConfig...",
            this->renderer()
        );
        m_handles.VisualInfo = glXGetVisualFromFBConfig(
            m_handles.Dpy,
            m_handles.FBC
        );
        sad::String visualidmesg = str(fmt::Format("Chosen visualID - {0}")  << m_handles.VisualInfo->visualid);
        SL_COND_LOCAL_INTERNAL(
            visualidmesg,
            this->renderer()
        );
        return true;
    }

    int attrlistsinglebuffered[] = {
        GLX_RGBA,
        GLX_RED_SIZE,   1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE,  1,
        GLX_DEPTH_SIZE, 4,
        None
    };

    int attrlistdoublebuffered[] = {
        GLX_RGBA, GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_DEPTH_SIZE, 4,
        None
    };


    this->m_handles.VisualInfo = glXChooseVisual(
        m_handles.Dpy,
        m_handles.Screen,
        attrlistdoublebuffered
    );

    if (m_handles.VisualInfo == nullptr)
    {
        m_handles.VisualInfo = glXChooseVisual(
            m_handles.Dpy,
            m_handles.Screen,
            attrlistsinglebuffered
        );
        SL_COND_LOCAL_INTERNAL(
            "glXChooseVisual() failed, defaulting to singlebuffering",
            this->renderer()
        );

    }
    if (m_handles.VisualInfo == nullptr)
    {
        SL_COND_LOCAL_INTERNAL(
            "glXChooseVisual() failed for singlebuffering",
            this->renderer()
        );
        result = false;
    }
    return result;
}

bool sad::os::WindowImpl::createWindow(bool lastresult)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::createWindow()", this->renderer());
    if (!lastresult)
    {
        return false;
    }

    Atom wmDelete;
    ::Window winDummy;

    m_handles.ColorMap = XCreateColormap(
         m_handles.Dpy,
         RootWindow(m_handles.Dpy, m_handles.VisualInfo->screen),
         m_handles.VisualInfo->visual,
         AllocNone
    );

    XSetWindowAttributes attr;
    attr.colormap = m_handles.ColorMap;
    attr.border_pixel = 0;
    attr.background_pixmap = None ;
    attr.event_mask = ExposureMask
                    | KeyPressMask
                    | FocusChangeMask
                    | ButtonPressMask
                    | StructureNotifyMask
                    | PointerMotionMask
                    | ButtonReleaseMask
                    | KeyReleaseMask
                    | EnterWindowMask
                    | LeaveWindowMask
                    | VisibilityChangeMask
                    | SubstructureNotifyMask;

    m_handles.Win = XCreateWindow(
        m_handles.Dpy,
        RootWindow(m_handles.Dpy, m_handles.VisualInfo->screen),
        0,
        0,
        m_creation_size.Width,
        m_creation_size.Height,
        0,
        m_handles.VisualInfo->depth,
        InputOutput,
        m_handles.VisualInfo->visual,
        CWBorderPixel | CWColormap | CWEventMask,
        &attr
    );

    // Stop here, because we cannot create window
    if (m_handles.Win == None)
    {
        SL_COND_LOCAL_INTERNAL("XCreateWindow() failed", this->renderer());
        return false;
    }
    wmDelete = XInternAtom(m_handles.Dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(m_handles.Dpy,  m_handles.Win, &wmDelete, 1);
    XSetStandardProperties(
        m_handles.Dpy,
        m_handles.Win,
        m_window_title.data(),
        m_window_title.data(),
        None,
        nullptr,
        0,
        nullptr
    );

    // Do input query
    m_handles.IM = XOpenIM(  m_handles.Dpy, nullptr, nullptr, nullptr);
    if (m_handles.IM == nullptr)
    {
      SL_COND_LOCAL_INTERNAL("XOpenIM() failed", this->renderer());
      return false;
    }

    char* failed_arg = XGetIMValues(m_handles.IM, XNQueryInputStyle, &(m_handles.Styles), nullptr);

    if (failed_arg != nullptr)
    {
      SL_COND_LOCAL_INTERNAL("Can\'t get styles", this->renderer());
      return false;
    }

    m_handles.IC = XCreateIC(m_handles.IM, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, m_handles.Win, nullptr);
    if (m_handles.IC == nullptr) {
        SL_COND_LOCAL_INTERNAL("XCreateIC() failed", this->renderer());
        return false;
    }

    XSetICFocus(m_handles.IC);


    // Commented: unless explicity told to, we should not expose window
    // XMapRaised(m_handles.Dpy,  m_handles.Win);


    return true;
}

#endif

void sad::os::WindowImpl::close()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::close()", this->renderer());

    if (!valid())
        return;

#ifdef WIN32
    PostMessage(m_handles.WND, WM_QUIT, 0, 0);
#endif

#ifdef X11
    // Taken from
    // http://john.nachtimwald.com/2009/11/08/sending-wm_delete_window-client-messages/
    XEvent ev;

    memset(&ev, 0, sizeof (ev));

    ev.xclient.type = ClientMessage;
    ev.xclient.window = m_handles.Win;
    ev.xclient.message_type = XInternAtom(m_handles.Dpy, "WM_PROTOCOLS", true);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = XInternAtom(m_handles.Dpy, "WM_DELETE_WINDOW", false);
    ev.xclient.data.l[1] = CurrentTime;
    XSendEvent(m_handles.Dpy, m_handles.Win, False, NoEventMask, &ev);
#endif
}

void sad::os::WindowImpl::destroy()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::destroy()", this->renderer());

    typedef void (sad::os::WindowImpl::*CleanupStep)();

#ifdef WIN32
    const int cleanupstepcount = 2;

    CleanupStep cleanupsteps[cleanupstepcount] = {
        &sad::os::WindowImpl::releaseContextAndDestroyWindow,
        &sad::os::WindowImpl::unregisterWindowClass
    };
#endif

#ifdef X11
    const int cleanupstepcount = 1;

    CleanupStep cleanupsteps[cleanupstepcount] = {
        &sad::os::WindowImpl::closeConnection
    };
#endif

    for(int i =  0; i < cleanupstepcount; i++ )
    {
        CleanupStep  step = cleanupsteps[i];
        (this->*step)();
    }

    m_handles.cleanup();
}

bool sad::os::WindowImpl::valid() const
{
#ifdef WIN32
    return m_handles.WND != nullptr;
#endif

#ifdef X11
    return m_handles.Win != None;
#endif

}


bool sad::os::WindowImpl::fixed() const
{
    return m_fixed;
}

void sad::os::WindowImpl::makeFixedSize()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::makeFixedSize()", this->renderer());

    m_fixed = true;

    if (!valid())
        return;

#ifdef WIN32
    LONG style = GetWindowLongA(m_handles.WND, GWL_STYLE);
    style &=  ~WS_MAXIMIZEBOX;
    LONG result = SetWindowLongA(m_handles.WND, GWL_STYLE, style);
    m_style = GetWindowLongPtr(m_handles.WND, GWL_STYLE);
#endif

#ifdef X11
    sad::Rect2I rect = this->rect();
    XSizeHints * sizehints = nullptr;
    sizehints = XAllocSizeHints();
    sizehints->flags = PMinSize | PMaxSize;
    sizehints->min_width = rect.width();
    sizehints->max_width = rect.width();
    sizehints->min_height = rect.height();
    sizehints->max_height = rect.height();
    XSetWMNormalHints(m_handles.Dpy,m_handles.Win,sizehints);
    XFree(sizehints);
#endif
}

void sad::os::WindowImpl::makeResizeable()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::makeResizeable()", this->renderer());

    m_fixed = false;

    if (!valid())
        return;

#ifdef WIN32
    LONG style=GetWindowLongA(m_handles.WND, GWL_STYLE);
    style |= WS_MAXIMIZEBOX;
    SetWindowLongA(m_handles.WND, GWL_STYLE, style);
    m_style = GetWindowLongPtr(m_handles.WND, GWL_STYLE);
#endif

#ifdef X11
    XSizeHints * sizehints = nullptr;
    sizehints = XAllocSizeHints();
    sizehints->flags = PMinSize | PMaxSize;
    sizehints->min_width = 1;
    sizehints->max_width = 40000;
    sizehints->min_height = 1;
    sizehints->max_height = 40000;
    XSetWMNormalHints(m_handles.Dpy,m_handles.Win,sizehints);
    XFree(sizehints);
#endif
}


bool sad::os::WindowImpl::fullscreen() const
{
    return m_fullscreen;
}

void sad::os::WindowImpl::enterFullscreen()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::enterFullscreen()", this->renderer());

    m_fullscreen = true;

    if (!valid())
        return ;


#ifdef WIN32
    this->m_window_rect_stack << rect();
    LONG_PTR style = WS_SYSMENU
                   | WS_POPUP
                   | WS_CLIPCHILDREN
                   | WS_CLIPSIBLINGS
                   | WS_VISIBLE;
    SetWindowLongPtr(m_handles.WND,  GWL_STYLE,  style);


    const long screenwidth = GetSystemMetrics(SM_CXSCREEN);
    const long screenheight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(
        m_handles.WND,
        HWND_TOPMOST,
        0,
        0,
        screenwidth,
        screenheight,
        SWP_SHOWWINDOW | SWP_NOSENDCHANGING
    );
#endif

#ifdef X11
    sendNetWMFullscreenEvent(true);
#endif
}

void sad::os::WindowImpl::leaveFullscreen()
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::leaveFullscreen()", this->renderer());
    m_fullscreen = false;

    if (!valid())
        return ;

#ifdef WIN32
    SetWindowLongPtr(
        m_handles.WND,
        GWL_STYLE,
        m_style
    );
    popRect();
#endif

#ifdef X11
    sendNetWMFullscreenEvent(false);
#endif
}


#ifdef X11

void sad::os::WindowImpl::sendNetWMFullscreenEvent(bool fullscreen)
{
    SL_COND_INTERNAL_SCOPE("sad::os::WindowImpl::sendNetWMFullscreenEvent()", this->renderer());

    if (!valid())
        return;

    // Possibly taken from
    // http://boards.openpandora.org/topic/12280-x11-fullscreen-howto/#entry229890
    XEvent xev;
    Atom wmstateatom = XInternAtom(m_handles.Dpy, "_NET_WM_STATE", False);
    Atom fullscreenatom = XInternAtom(m_handles.Dpy, "_NET_WM_STATE_FULLSCREEN", False);

    memset(&xev, 0, sizeof(XEvent));

    XWindowAttributes xwa;
    XGetWindowAttributes(
        m_handles.Dpy,
        m_handles.Win,
        &xwa
    );

    xev.type = ClientMessage;
    xev.xclient.display = m_handles.Dpy;
    xev.xclient.window = m_handles.Win;
    xev.xclient.message_type = wmstateatom;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = (fullscreen) ? 1 : 0;
    xev.xclient.data.l[1] = fullscreenatom;
    xev.xclient.data.l[2] = 0;

    XSendEvent(
        m_handles.Dpy,
        xwa.root,
        False,
        SubstructureNotifyMask | SubstructureRedirectMask,
        &xev
    );

    XFlush(m_handles.Dpy);
}

#endif

bool sad::os::WindowImpl::hidden() const
{
    return m_hidden;
}


void sad::os::WindowImpl::show()
{
    m_hidden = false;
    if (valid())
    {
#ifdef WIN32
        ShowWindow(m_handles.WND, SW_SHOW);
        m_style = GetWindowLongPtr(m_handles.WND, GWL_STYLE);
#endif

#ifdef X11
        XMapWindow(m_handles.Dpy, m_handles.Win);
        XMapRaised(m_handles.Dpy, m_handles.Win);
        XFlush(m_handles.Dpy);
#endif
    }
}


void sad::os::WindowImpl::hide()
{
    m_hidden = true;
    if (valid())
    {
#ifdef WIN32
        ShowWindow(m_handles.WND, SW_HIDE);
        m_style = GetWindowLongPtr(m_handles.WND, GWL_STYLE);
#endif

#ifdef X11
        XUnmapWindow(m_handles.Dpy, m_handles.Win);
#endif
    }
}

void sad::os::WindowImpl::setRect(const sad::Rect2I& rect)
{
    if (!valid())
        return;

#ifdef WIN32
    SetWindowPos(
        m_handles.WND,
        HWND_NOTOPMOST,
        (int)(rect[0].x()),
        (int)(rect[0].y()),
        (int)(rect.width()),
        (int)(rect.height()),
        SWP_FRAMECHANGED
    );
#endif

#ifdef X11
    XMoveResizeWindow(
        m_handles.Dpy,
        m_handles.Win,
        rect[0].x(),
        rect[0].y(),
        rect.width(),
        rect.height()
    );
#endif
}

void sad::os::WindowImpl::pushRect(const sad::Rect2I& rect)
{
    if (!valid())
    {
        return ;
    }

    m_window_rect_stack << this->rect();
    setRect(rect);
}
void sad::os::WindowImpl::popRect()
{
    if (!valid() || m_window_rect_stack.size() == 0)
    {
        return ;
    }

    sad::Rect2I rect = m_window_rect_stack[m_window_rect_stack.size() - 1];
    m_window_rect_stack.removeAt(m_window_rect_stack.size() - 1);
    setRect(rect);
}

sad::Rect2I sad::os::WindowImpl::rect() const
{
    if (!valid())
        return sad::Rect2I();

#ifdef WIN32
    RECT rect;
    GetWindowRect(m_handles.WND, &rect);
    sad::Rect2I r(rect.left, rect.top, rect.right, rect.bottom);
    return r;
#endif

#ifdef X11
    int x = 0, y = 0;
    unsigned int width = 0, height = 0, depth = 0, border = 0;
    ::Window root = None;
    XGetGeometry(
        m_handles.Dpy,
        m_handles.Win,
        &root,
        &x,
        &y,
        &width,
        &height,
        &border,
        &depth
    );
    sad::Rect2I r(x, y, x + width + border, y + height + border);
    return r;
#endif
}

sad::Point2D sad::os::WindowImpl::toClient(const sad::Point2D & p)
{
    if (!valid())
        return p;

    sad::Point2D result =  p;

#ifdef WIN32
    RECT r;
    GetClientRect(this->m_handles.WND, &r);
    result.setY(r.bottom  - result.y());
#endif

    return result;
}

sad::os::WindowHandles * sad::os::WindowImpl::handles()
{
    return &m_handles;
}

const sad::String & sad::os::WindowImpl::title() const
{
    if (!valid())
        return m_window_title;

    sad::os::WindowImpl * impl = const_cast<sad::os::WindowImpl *>(this);

#ifdef WIN32
    int length = GetWindowTextLengthA(m_handles.WND);
    impl->m_window_title.resize((sad::String::size_type)(length + 1));
    GetWindowTextA(m_handles.WND, const_cast<char*>(impl->m_window_title.data()), length + 1);
#endif

#ifdef X11
    char * title = nullptr;
    XFetchName(m_handles.Dpy, m_handles.Win, &title);
    impl->m_window_title = title;
    XFree(title);
#endif

    return m_window_title;
}

void sad::os::WindowImpl::setTitle(const sad::String & s)
{
    m_window_title = s;

    if (!valid())
        return;

#ifdef WIN32
    SetWindowTextA(m_handles.WND, m_window_title.data());
#endif

#ifdef X11
    XStoreName(m_handles.Dpy, m_handles.Win, m_window_title.data());
    XFlush(m_handles.Dpy);
#endif

}


bool sad::os::WindowImpl::active() const
{
    return m_active && !hidden();
}

void sad::os::WindowImpl::setActive(bool active)
{
    m_active = active;
}

void sad::os::WindowImpl::setHidden(bool hidden)
{
    m_hidden = hidden;
}

bool sad::os::WindowImpl::minimized() const
{
     return m_minimized;
}

void sad::os::WindowImpl::setMinimized(bool minimized)
{
    m_minimized = minimized;
}


bool sad::os::WindowImpl::isGL3compatible() const
{
#ifdef WIN32
    return true;
#endif

#ifdef X11
    return m_gl3compatible;
#endif
}
