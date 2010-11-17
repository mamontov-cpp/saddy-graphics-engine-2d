/*! \file   types.h
\author FreakyBlast, HiddenSeeker

\brief  Declaration of types for engine.

*/

#ifndef Types_h__
#define Types_h__

#ifdef   WIN32
	#include <windows.h>
#else

#endif
#include <gl/gl.h>														
#include <gl/glu.h>	
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "habstract.hpp"
#include "hdeque.hpp"
#include "hhash.hpp"
#include "hlist.hpp"
#include "hlvector.hpp"
#include "hmatrix.hpp"
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
#endif // Types_h__
