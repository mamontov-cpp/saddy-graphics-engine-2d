/*! \file   renderer.cpp
\author FreakyBlast

\brief  Definition of Renderer.

In this file OpenGL function has been used obviously.
*/


#include "renderer.h"
#ifdef WIN32
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif


sad::Renderer::~Renderer(void)
{
	
}

bool sad::Renderer::init(const sad::Settings& _settings)
{
 m_glsettings.setWidthScreen(_settings.width());
 m_glsettings.setHeightScreen(_settings.height());
 m_glsettings.setIsFullscreen(_settings.isFullscreen());
 m_glsettings.setBPP(_settings.bpp());
 m_glsettings.setDepth(_settings.depth());
 m_glsettings.setFoV(_settings.fov());
 m_glsettings.setZNear(_settings.znear());
 m_glsettings.setZFar(_settings.zfar());
 
 return true;
}


void sad::Renderer::reshape(int width, int height)
{
 glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Переустанавливаем ViewPort (область видимости)
 glMatrixMode (GL_PROJECTION);										// Выбираем матрицу проекции
 glLoadIdentity ();													// Сбрасываем её на единичную
 gluPerspective (m_glsettings.fov(), 
			    (GLfloat)(width)/(GLfloat)(height),		      	// Calculate The Aspect Ratio Of The Window
			     m_glsettings.znear(), 
				 m_glsettings.zfar());		
 glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
 glLoadIdentity ();													// Сбрасываем её на единичную
}


sad::Renderer& sad::Renderer::instance()
{
 static Renderer aloneRenderer;
 return aloneRenderer;
}


void sad::Renderer::mainLoop()
{
 int frames=0;
 bool isMessagePumpActive;
 MSG msg;

 m_running = true;											// Program Looping Is Set To TRUE
 Renderer::setTimer();


 while (m_running)											// Loop Until WM_QUIT Is Received
 {
  Renderer::instance().setTimer();					
  // Check For Window Messages
  if (PeekMessage (&msg, m_window.hWND, 0, 0, PM_REMOVE) != 0)
  {
	 // Check For WM_QUIT Message
	 if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
	 {
	  TranslateMessage(&msg);				// Translate The Message
	  DispatchMessage(&msg);						// If Not, Dispatch The Message
	 }
	 else											// Otherwise (If Message Is WM_QUIT)
	 {
		m_running = false;				// Terminate The Message Loop
	 }
   }
   else												// If There Are No Messages
   {
	  // Process Application Loop
	  frames++;
	  if (Renderer::instance().elapsedInMSeconds() >= 1000)
	  {m_fps = frames;frames=0;Renderer::instance().setTimer();}
	   update();
   }
  }														
 this->releaseWindow();
}



void sad::Renderer::run()
{
 if (createWindow())
 {
		hst::log::inst()->owrite(hst::string("Renderer: started rendering cycles\n"));
	    mainLoop();
 }
 else
	 hst::log::inst()->owrite(hst::string("Can't create window\n"));
 
 hst::log::inst()->save("log.txt");
}	

LRESULT CALLBACK sad::Renderer::WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_CLOSE)
	{
     	    sad::Renderer::instance().m_running=false;					
			return 0;													
	}
	if (uMsg==WM_KEYDOWN)
	{
		if (wParam==VK_ESCAPE)
		{
			instance().quit();
		}
		char af[1000];
		GetKeyNameText(lParam,af,100);
		if (af[0]=='F')
		{
			instance().toggleFullscreen();
		}
		hst::log::inst()->owrite(hst::string(af));
		return 0;
	}
	/*	
	   // Get The Window Context
		switch (uMsg)														// Evaluate Window Message
		{
		case WM_SYSCOMMAND:												// Intercept System Commands
			{
				switch (wParam)												// Check System Calls
				{
				case SC_SCREENSAVE:										// Screensaver Trying To Start?
				case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
					return 0;												// Prevent From Happening
				}
				break;														// Exit
			}
			return 0;														// Return

		case WM_CREATE:													// Window Creation
			{
				//CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
				//render = (sad::Renderer*)(creation->lpCreateParams);
				//SetWindowLong (hWnd, GWL_USERDATA, (LONG)(render));
			}
			return 0;														// Return


		case WM_SIZE:													// Size Action Has Taken Place
			switch (wParam)												// Evaluate Size Action
			{
			case SIZE_MINIMIZED: // Was Window Minimized?

				//newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				//newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				//newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				//newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				//newSadW.isVisible=false;
				//newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				//sad::Renderer::instance().setCurrentWindow(&newSadW);
				//return 0;												// Return

			case SIZE_MAXIMIZED:									// Was Window Maximized?
				//newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				//newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				//newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				//newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				//newSadW.isVisible=true;
				//newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				//sad::Renderer::instance().setCurrentWindow(&newSadW);
				//sad::Renderer::instance().reshape(LOWORD (lParam), HIWORD (lParam));		// Reshape Window - LoWord=Width, HiWord=Height
				return 0;												// Return

			case SIZE_RESTORED:										// Was Window Restored?
				//newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				//newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				//newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				//newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				//newSadW.isVisible=true;
				//newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				//sad::Renderer::instance().setCurrentWindow(&newSadW);
				//sad::Renderer::instance().reshape(LOWORD (lParam), HIWORD (lParam));
				return 0;												// Return
			}
			break;															// Break

		case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed

			break;															// Break

		case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
			
			break;															// Break

		case VK_ESCAPE:
			break;

			//case WM_TOGGLEFULLSCREEN:										// Toggle FullScreen Mode On/Off
			//	g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			//	PostMessage (hWnd, WM_QUIT, 0, 0);
			//break;															// Break
		}

		*/
		return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}




void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  this->releaseWindow();
  this->m_window.fullscreen=!this->m_window.fullscreen;
  this->createWindow();
}

bool sad::Renderer::changeScreenResolution(int width, int height, int bitsPerPixel)	// Change The Screen Resolution
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






//Getting a black background with all params
bool sad::Renderer::initGLRendering()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f); //Fill a black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return true;
}
