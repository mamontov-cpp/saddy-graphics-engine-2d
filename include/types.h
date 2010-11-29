/*! \file   types.h
\author FreakyBlast, HiddenSeeker

\brief  Declaration of types for engine.

*/

#ifndef types_h__
#define types_h__


#ifdef   _WIN32
    #define _WIN32_WINNT  0x0501
    #define _WIN32_WINDOWS 0x0501
	#include <windows.h>
    #include <gl/gl.h>														
    #include <gl/glu.h>
#else
    #include <GL/gl.h>														
    #include <GL/glu.h>
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
		HDC					hDC;						//!< Devide context
		HGLRC				hRC;						//!< OpenGL context
		HINSTANCE           hInstance;                  //!< Instance of module
#endif
		bool                active;                     //!< Is window active
		bool                fullscreen;                 //!< Full screen
	};	

}
#endif // types_h__
