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
    #include <X11/extensions/xf86vmode.h>
    #include <X11/keysym.h>
#endif
	
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "habstract.hpp"
#include "hdeque.hpp"
#include "hhash.hpp"
#include "hlist.hpp"
#include "hlvector.hpp"
#include "hpair.hpp"
#include "hptrie.hpp"
#include "hstring.h"
#include "hstringlist.h"
#include "hwstring.h"


#include "log.h"
#include "hcolor.h"
#include "hpoint.h"
#include "hrect.h"

#ifdef  LINUX

typedef Window  X11Window;

#endif

namespace sad
{
  /*! Chunk, for various loading operations
  */
  typedef unsigned long Chunk;

	
   /*! Contains a window info
   */
   struct Window
   {
#ifdef WIN32
	        HWND				hWND;						//!< Handle for a window
		HDC					hDC;					//!< Devide context
		HGLRC				hRC;						//!< OpenGL context
		HINSTANCE           hInstance;                 			        //!< Instance of module
		DEVMODEA            scr_settings;                                              //!<  Screen settings
#else
                Display         *     dpy;           //!< Display
                int                       screen;     //!<  Screen
                GLXContext ctx;                  //!< Graphic context
                XSetWindowAttributes attr;  //!< Window attributes    
                int x, y;                                //!<  X and Y coordinates of window
                unsigned int width, height;   //!< Width and height of window
                unsigned int depth;              //!< Depth    
#endif
		bool                active;                     //!< Is window active
		bool                fullscreen;                 //!< Full screen
	};	

}
