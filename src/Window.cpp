
#include "..\include\window.h"													
#include "..\include\renderer.h"													


#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

namespace Sad{
	Sad::Window::Window(void)
	{
	}

	Sad::Window::~Window(void)
	{
	}

#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling

	extern bool isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
	extern bool createFullScreen;											// If TRUE, Then Create Fullscreen

	void Sad::Window::terminateApplication (sadWindow* window)							// Terminate The Application
	{
		PostMessage(window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
		isProgramLooping = false;											// Stop Looping Of The Program
	}

	void Sad::Window::toggleFullscreen (sadWindow* window)								// Toggle Fullscreen/Windowed
	{
		PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
	}

	void Sad::Window::reshapeGL (int width, int height, Sad::Settings* _sets)									// Reshape The Window When It's Moved Or Resized
	{
		Renderer::instance().reshape(width, height, _sets);
	}

	bool Sad::Window::changeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
	{
		DEVMODE dmScreenSettings;											// Device Mode
		ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
		dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
		dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
		dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
		dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			return false;													// Display Change Failed, Return False
		}
		return true;														// Display Change Was Successful, Return True
	}

	bool Sad::Window::createWindow(sadWindow* wndparams){
		// Create The OpenGL Window
		unsigned long windowStyle=0; // <-- Please, fill this field
		unsigned long windowExtendedStyle=0; //<-- Please, fill this field

		/* ATTENTION PLEASE 
		we need to fill this variable,
		but from where we have got hInstance?

		Application appToFill;
		appToFill.hInstance = ???? ;
		appToFill.className = "SadEngine Demo";

		wndparams.app = &appToFill;
		*/

		wndparams->hWnd = CreateWindowEx(windowExtendedStyle,					// Extended Style
			(LPCSTR)wndparams->app->className,	// Class Name
			(LPCSTR)wndparams->title,					// Window Title
			windowStyle,							// Window Style
			0, 0,								// Window X,Y Position
			800,	// Window Width
			600,	// Window Height
			HWND_DESKTOP,						// Desktop Is Window's Parent
			0,									// No Menu
			0,/*wndparams->hInstance*/ // Pass The Window Instance
			0
			);

		if (!wndparams->hWnd)												// Was Window Creation A Success?
		{
			return false;													// If Not Return False
		}

		wndparams->hDC = GetDC (wndparams->hWnd);									// Grab A Device Context For This Window
		if (!wndparams->hDC)												// Did We Get A Device Context?
		{
			// Failed
			DestroyWindow (wndparams->hWnd);									// Destroy The Window
			wndparams->hWnd = 0;												// Zero The Window Handle
			return false;													// Return False
		}
		wndparams->isVisible = true;											// Set isVisible To True

		return true;
	}

	bool Sad::Window::createGLContext(Sad::Settings* _settings,sadWindow* wndparams)									// This Code Creates Our OpenGL Window
	{
		unsigned long windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
		unsigned long windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style


		RECT windowRect = {0, 0, _settings->screenWidth(), _settings->screenHeight()};	// Define Our Window Coordinates


		if (_settings->isFullscreen())								// Fullscreen Requested, Try Changing Video Modes
		{
			if (!changeScreenResolution (_settings->screenWidth(), _settings->screenHeight(), _settings->bpp()))
			{
				// Fullscreen Mode Failed.  Run In Windowed Mode Instead
				MessageBox (HWND_DESKTOP, LPCSTR("Mode Switch Failed.\nRunning In Windowed Mode."), LPCSTR("Error"), MB_OK | MB_ICONEXCLAMATION);
				_settings->setIsFullscreen(false);							// Set isFullscreen To False (Windowed Mode)
			}
			else															// Otherwise (If Fullscreen Mode Was Successful)
			{
				ShowCursor (FALSE);											// Turn Off The Cursor
				windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
				windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
			}																// (Top Window Covering Everything Else)
		}
		else																// If Fullscreen Was Not Selected
		{
			// Adjust Window, Account For Window Borders
			AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		}


		if (!Renderer::instance().init(_settings,wndparams))
			return false;


		ShowWindow (wndparams->hWnd, SW_NORMAL);								// Make The Window Visible
		Window::reshapeGL(_settings->screenWidth(), _settings->screenHeight(),_settings);				// Reshape Our GL Window

		return true;														// Window Creating Was A Success
		// Initialization Will Be Done In WM_CREATE
	}


	bool Sad::Window::destroyWindowGL (Sad::sadWindow* window, Sad::Settings* settings)								// Destroy The OpenGL Window & Release Resources
	{

		if (window->hWnd != 0)												// Does The Window Have A Handle?
		{	
			if (window->hDC != 0)											// Does The Window Have A Device Context?
			{
				wglMakeCurrent (window->hDC, 0);							// Set The Current Active Rendering Context To Zero
				if (window->hRC != 0)										// Does The Window Have A Rendering Context?
				{
					wglDeleteContext (window->hRC);							// Release The Rendering Context
					window->hRC = 0;										// Zero The Rendering Context

				}
				ReleaseDC (window->hWnd, window->hDC);						// Release The Device Context
				window->hDC = 0;											// Zero The Device Context
			}
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
		}

		if (settings->isFullscreen())										// Is Window In Fullscreen Mode
		{
			ChangeDisplaySettings (NULL,0);									// Switch Back To Desktop Resolution
			ShowCursor (true);												// Show The Cursor
		}	
		return true;														// Return True
	}

	void Sad::Window::update(long long deltaTime){

	}

	void Sad::Window::initSettings(Sad::Settings* _GLSettings){

	}

}

