/*! \file   types.h
\author FreakyBlast

\brief  Declaration of types for engine.

*/

#ifndef Types_h__
#define Types_h__

#include <windows.h>
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

namespace Sad{
	typedef HANDLE SADHANDLE;

	static bool isProgramLooping;
	static bool createFullScreen;



	typedef struct{
		HINSTANCE		    hInstance;						//!< Application Instance
		const char*		    className;						//!< Application ClassName
	} Application;

	/*!\typedef Contains information about window in operating system*/
	typedef struct {
		char*				title;
		Application*        app;
		HWND				hWnd;						//!< Handle for a window
		HDC					hDC;						//!< Devide context
		HGLRC				hRC;						//!< OpenGL context
		Uint8				isVisible;					//!< Visibility of window
	} sadWindow;										// GL_Window

}
#endif // Types_h__
