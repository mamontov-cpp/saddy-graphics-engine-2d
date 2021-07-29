/*! \file windowhandles.h


    Defines window OS-dependent handles, which should be used for identifying window
    and calling OS-dependent functions on them
 */
#pragma once
#include "../sadstring.h"

#ifdef WIN32
// We redefine  them to have access to specific macro, for system-dependent
// parameters
#undef   _WIN32_WINNT
#define _WIN32_WINNT  0x0501
#undef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
// Remove min max, as obsolete and leading to mistakes
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#endif

// Linux implementation, uses GLX for interactions between GL and window systems
#ifdef LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#endif

// You should not include it to system-wide headers, since XLib pollutes namespaces
// with it's definitions HEAVILY
#ifdef X11
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xlocale.h>
#include <GL/glx.h>
#include "../3rdparty/glext/glxext.h"
#endif


namespace sad
{

namespace os
{

/*! This is	OS-dependent handles for window. Note that, GL context handles are stored
    separately.
 */
class WindowHandles
{
public:

#ifdef WIN32
    /*! Instance of current process, as module
     */
    HINSTANCE    ProcessInstance;
    /*! Handle for a window
     */
    HWND	     WND;
    /*! Device context
     */
    HDC		     DC;
    /*! Adjusted window rectangle for window creation
     */
    RECT         AdjustedWindowRect;
    /*! A window class
     */
    sad::String  Class;
#endif

#ifdef X11
    /*! Display, where window is displayed
     */
    Display*     Dpy;
    /*! A screen, where window is displayed
     */
    int          Screen;
    /*! A window
     */
    ::Window     Win;
    /*! A visual information
     */
    XVisualInfo * VisualInfo;
    /*! A color map for a window
     */
    Colormap      ColorMap;
    /*! A frame buffer config for window creation
     */
    GLXFBConfig   FBC;
    /* Input method
     */
    XIM IM;
    /*! Input context
     */
    XIC IC;
    /*! Styles for input
     */
    XIMStyles *Styles;
#endif
    /*! Creates new zero-filled handles
     */
    inline WindowHandles()  //-V730
    {
        cleanup();
    }

    inline void cleanup()
    {
#ifdef WIN32
        ProcessInstance = nullptr;
        WND = nullptr;
        DC = nullptr;
        Class.clear();
#endif

#ifdef X11
        Dpy = nullptr;
        Screen = 0;
        Win = None;
        VisualInfo = 0;
#endif
    }
};

}

}
