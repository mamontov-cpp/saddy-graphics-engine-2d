/*! \file   types.h
    \author FreakyBlast

	\brief  Declaration of types for engine.

*/

#ifndef Types_h__
#define Types_h__

#include <windows.h>
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


#include "hcolor.h"
#include "hpoint.h"
#include "hrect.h"


typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef HANDLE SADHANDLE;


/*! \typedef Contains TRUE/FALSE for any key*/
typedef struct {									
	BOOL keyDown [256];					            //!< Array of keys			
} Keys;				

/*!\typedef Information for program*/
typedef struct {									
	HINSTANCE		hInstance;						//!< Application Instance
	const char*		className;						//!< Application ClassName
} Application;										// Application

/*!\typedef Information for creation of the window*/
typedef struct {									
	Application*		application;				//!< Structure application 
	char*				title;						//!< Title of window
	int					width;						//!< Width of window
	int					height;						//!< Height of window
	int					bitsPerPixel;				//!< Bit per pixel
	BOOL				isFullScreen;				//!< Is fullscreen mode
} GL_WindowInit;									// GL_WindowInit

/*!\typedef Contains information about OpenGL window*/
typedef struct {									
	Keys*				keys;						//!< Array for keys
	HWND				hWnd;						//!< Handle for a window
	HDC					hDC;						//!< Devide context
	HGLRC				hRC;						//!< OpenGL context
	GL_WindowInit		init;						//!< Initialization of window
	BOOL				isVisible;					//!< Visibility of window
	DWORD				lastTickCount;				//!< Tick counter
} GL_Window;										// GL_Window


#endif // Types_h__
