/*! \file   types.h
    \author FreakyBlast, HiddenSeeker

    \brief  Declaration of types for engine.
*/
#pragma once

#ifdef   _WIN32
    #undef   _WIN32_WINNT
    #define _WIN32_WINNT  0x0501
    #undef _WIN32_WINDOWS
    #define _WIN32_WINDOWS 0x0501
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include <windows.h>
    #include <gl/gl.h>														
    #include <gl/glu.h>
#else
    #define XK_MISCELLANY 1
    #define XK_LATIN1           1
    #define XK_CYRILLIC        1

    #include <GL/glx.h>
    #include <GL/gl.h>														
    #include <GL/glu.h>
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
#ifndef DO_NOT_CLEANUP_X11_NAMESPACE
    #undef CurrentTime
	#undef Status
	#undef Bool
	#undef None
	#undef CursorShape
	#undef GrayScale
	#undef Color
	#undef KeyPress
	#undef KeyRelease
	#undef FocusIn
	#undef FocusOut
	#undef FontChange
	#undef Unsorted
#endif
#endif
	
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "templates/habstract.hpp"
#include "templates/hdeque.hpp"
#include "templates/hhash.hpp"
#include "templates/hlist.hpp"
#include "templates/svector.h"
#include "templates/hpair.hpp"
#include "templates/hptrie.hpp"
#include "templates/hstring.h"
#include "templates/hwstring.h"


#include "primitives/hcolor.h"
#include "primitives/hpoint.h"
#include "primitives/hrect.h"
#pragma once

#ifdef  LINUX

typedef Window  X11Window;

#endif

#ifndef UNUSED
#ifdef GCC
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

namespace sad
{
  /*! Chunk, for various loading operations
  */
  typedef unsigned long Chunk;

	
   /*! Contains a window info
   */
   class Window
   {
    public:
#ifdef WIN32
	    HWND			     hWND;				    //!< Handle for a window
		HDC					 hDC;					//!< Devide context
		HGLRC				 hRC;				    //!< OpenGL context
		HINSTANCE            hInstance;             //!< Instance of module
		DEVMODEA             scr_settings;          //!< Screen settings
		hst::string          window_class;          //!< Class of window       
#else
        Display*             dpy;                   //!< Display
        int                  screen;                //!<  Screen
        GLXContext           ctx;                   //!< Graphic context
        XSetWindowAttributes attr;                  //!< Window attributes 
		X11Window            win;                   //!< X11 window   
        int                  x, y;                  //!<  X and Y coordinates of window
        unsigned int         depth;                 //!< Depth    
		XVisualInfo*         vi;                    //!< Visual info
#endif
		bool                 active;                //!< Is window active
		bool                 fullscreen;            //!< Full screen
		unsigned int         width;                 //!< Width of window
		unsigned int         height;                //!< Height of window
		unsigned int         clientwidth;           //!< A client width of window
		unsigned int         clientheight;          //!< A client height of window
		inline Window() {}
	};	

}
