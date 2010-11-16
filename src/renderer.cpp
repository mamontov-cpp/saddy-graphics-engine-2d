/*! \file   renderer.cpp
\author FreakyBlast

\brief  Definition of Renderer.

In this file OpenGL function has been used obviously.
*/


#include "renderer.h"
#include "testnode.h"
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

namespace sad{

	sad::Renderer::Renderer(void)
	{
		m_currentwindow.title="SadExample";


	}

	sad::Renderer::~Renderer(void)
	{


	}





	bool sad::Renderer::init(const sad::Settings& _settings)
	{
		m_glsettings.setWidthScreen(_settings.screenWidth());
		m_glsettings.setHeightScreen(_settings.screenHeight());
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
		gluPerspective (m_glsettings.fov()*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
			m_glsettings.znear(), m_glsettings.zfar());		
		glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
		glLoadIdentity ();													// Сбрасываем её на единичную


		glEnable(GL_DEPTH_TEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Действительно красивое вычисление перспективы
	}


	sad::Renderer& sad::Renderer::instance()
	{
		static Renderer aloneRenderer;
		return aloneRenderer;
	}


	void sad::Renderer::mainLoop(){

		int frames=0;
		bool isMessagePumpActive;
		LPMSG msg;

		isProgramLooping = true;											// Program Looping Is Set To TRUE


		Renderer::setTimer();

		while (isProgramLooping)											// Loop Until WM_QUIT Is Received
		{
			bool someflag=true;
			// Create A Window
			if (createWindow())							// Was Window Creation Successful?
			{
				// At This Point We Should Have A Window That Is Setup To Render OpenGL
				if (!createGLContext())					// Call User Intialization
				{
					// Failure
					quit();							// Close Window, This Will Handle The Shutdown
				}
				else														// Otherwise (Start The Message Pump)
				{	// Initialize was a success						// Set isMessagePumpActive To TRUE


					Renderer::instance().setTimer();

					while (isProgramLooping)						// While The Message Pump Is Active
					{
						// Success Creating Window.  Check For Window Messages
						if (PeekMessage (msg, m_currentwindow.hWnd, 0, 0, PM_REMOVE) != 0)
						{
							// Check For WM_QUIT Message
							if (msg->message != WM_QUIT)						// Is The Message A WM_QUIT Message?
							{
								DispatchMessage(msg);						// If Not, Dispatch The Message
							}
							else											// Otherwise (If Message Is WM_QUIT)
							{
								isProgramLooping = false;				// Terminate The Message Pump
							}
						}
						else												// If There Are No Messages
						{
							if (!m_currentwindow.isVisible)					// If Window Is Not Visible
							{
								WaitMessage();								// Application Is Minimized Wait For A Message
							}
							else											// If Window Is Visible
							{

								// Process Application Loop
								frames++;
								if (Renderer::instance().elapsedInMSeconds() >= 1000){// Counting fps
									m_fps = frames;
									frames=0;
									Renderer::instance().setTimer();
									//if (someflag){
									//	Sleep(sad::Renderer::instance().fps()*1000);
									//someflag=false;
									//}
								}
								// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
								update();
								//Sleep(50);



								SwapBuffers (m_currentwindow.hDC);					// Swap Buffers (Double Buffering)

							}
						}
					}														// Loop While isMessagePumpActive == TRUE
				}															// If (Initialize (...

				// Application Is Finished

				destroyWindowGL();									// Destroy The Active Window
			}
			else															// If Window Creation Failed
			{
				// Error Creating Window
				MessageBox (HWND_DESKTOP, LPCSTR("Error Creating OpenGL Window"), LPCSTR("Error"), MB_OK | MB_ICONEXCLAMATION);
				isProgramLooping = false;									// Terminate The Loop
				// to log
			}
		}// While (isProgramLooping)

	}




	void sad::Renderer::run(){

		mainLoop();

	}	

	LRESULT CALLBACK sad::Renderer::WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Get The Window Context
		//sad::Renderer* render = (sad::Renderer*)(GetWindowLong (hWnd, GWL_USERDATA));
		sad::sadWindow newSadW;
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

		case WM_CLOSE:													// Closing The Window
			sad::Renderer::instance().quit();								// Terminate The Application
			return 0;														// Return

		case WM_SIZE:													// Size Action Has Taken Place
			switch (wParam)												// Evaluate Size Action
			{
			case SIZE_MINIMIZED: // Was Window Minimized?

				newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				newSadW.isVisible=false;
				newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				sad::Renderer::instance().setCurrentWindow(&newSadW);
				return 0;												// Return

			case SIZE_MAXIMIZED:									// Was Window Maximized?
				newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				newSadW.isVisible=true;
				newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				sad::Renderer::instance().setCurrentWindow(&newSadW);
				sad::Renderer::instance().reshape(LOWORD (lParam), HIWORD (lParam));		// Reshape Window - LoWord=Width, HiWord=Height
				return 0;												// Return

			case SIZE_RESTORED:										// Was Window Restored?
				newSadW.app=sad::Renderer::instance().getCurrentWindow()->app;
				newSadW.hDC=sad::Renderer::instance().getCurrentWindow()->hDC;
				newSadW.hRC=sad::Renderer::instance().getCurrentWindow()->hRC;
				newSadW.hWnd=sad::Renderer::instance().getCurrentWindow()->hWnd;
				newSadW.isVisible=true;
				newSadW.title=sad::Renderer::instance().getCurrentWindow()->title;					// Set isVisible To False
				sad::Renderer::instance().setCurrentWindow(&newSadW);
				sad::Renderer::instance().reshape(LOWORD (lParam), HIWORD (lParam));
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
			//	/*g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			//	PostMessage (hWnd, WM_QUIT, 0, 0);*/
			//break;															// Break
		}

		return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
	}


	bool sad::Renderer::RegisterWindowClass (Application& application)						// Register A Window Class For This Application.
	{																		// TRUE If Successful
		// Register A Window Class
		WNDCLASSEX windowClass;												// Window Class
		ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
		windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
		windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
		windowClass.lpfnWndProc		= (WNDPROC)(sad::Renderer::WindowProc);				// WindowProc Handles Messages
		windowClass.hInstance		= application.hInstance;				// Set The Instance
		windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
		windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		windowClass.lpszClassName	= (LPCSTR)application.className.data();				// Sets The Applications Classname
		if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
		{
			// NOTE: Failure, Should Never Happen
			MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return false;													// Return False (Failure)
		}
		return true;														// Return True (Success)
	}





	bool sad::Renderer::createWindow(){

		sad::Application tempApp;
		tempApp.className="classSad";
		tempApp.hInstance = GetModuleHandle(NULL);
		RegisterWindowClass(tempApp);

		m_currentwindow.app.hInstance = tempApp.hInstance;
		m_currentwindow.app.className = "classSad";


		m_currentwindow.hWnd = CreateWindow(/*WS_EX_APPWINDOW,*/
			(LPCSTR)m_currentwindow.app.className.data(),	// Class Name
			(LPCSTR)m_currentwindow.title.data(),			// Window Title
			WS_VISIBLE,						    // Window Style
			0, 0,								// Window X,Y Position
			m_glsettings.screenWidth(),	        // Window Width
			m_glsettings.screenHeight(),	    // Window Height
			HWND_DESKTOP,						// Desktop Is Window's Parent
			0,									// No Menu
			tempApp.hInstance,          // Pass The Window Instance
			0
			);

		if (!m_currentwindow.hWnd)												// Was Window Creation A Success?
		{
			return false;													// If Not Return False
		}

		m_currentwindow.hDC = GetDC(m_currentwindow.hWnd);		// Grab A Device Context For This Window
		if (!m_currentwindow.hDC)						
		{// Did We Get A Device Context?
			// Failed
			DestroyWindow (getCurrentWindow()->hWnd);	
			m_currentwindow.hWnd = 0;										
			return false;													
		}
		m_currentwindow.isVisible = true;         
		return true;
	}

	bool sad::Renderer::createGLContext(){

		hst::log* curLog;
		unsigned long windowStyle;
		unsigned long windowExtendedStyle;

		RECT windowRect = {0, 0, m_glsettings.screenWidth(), m_glsettings.screenHeight()};	// Define Our Window Coordinates


		if (m_glsettings.isFullscreen())								// Fullscreen Requested, Try Changing Video Modes
		{
			if (!changeScreenResolution (m_glsettings.screenWidth(), m_glsettings.screenHeight(), m_glsettings.bpp()))
			{
				// Fullscreen Mode Failed.  Run In Windowed Mode Instead
				MessageBox (HWND_DESKTOP, LPCSTR("Mode Switch Failed.\nRunning In Windowed Mode."), LPCSTR("Error"), MB_OK | MB_ICONEXCLAMATION);
				m_glsettings.setIsFullscreen(false);							// Set isFullscreen To False (Windowed Mode)
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
			AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, 0, WS_EX_APPWINDOW);
		}



		PIXELFORMATDESCRIPTOR pfd =											// pfd указывает Windows как устанавливать OpenGL
		{
			sizeof (PIXELFORMATDESCRIPTOR),									// Размер этого Pixel Format Descriptor'а
			1,																// Номер версии
			PFD_DRAW_TO_WINDOW |											// Формат должен поддерживать отрисовку в окно
			PFD_SUPPORT_OPENGL |											// Формат должен поддерживать OpenGL
			PFD_DOUBLEBUFFER,												// Формат должен поддерживать Double Buffering
			PFD_TYPE_RGBA,													// Формат должен поддерживать RGBA режим
			m_glsettings.bpp(),								        		// Выбираем глубину цвета
			0, 0, 0, 0, 0, 0,												// Игнорируем биты цвета
			0,																// Нет альфа буфера (пока нет)
			0,																// Игнорируем бит сдвига
			0,																// Нету Accumulation Buffer'а
			0, 0, 0, 0,														// Accumulation биты игнорируем
			m_glsettings.depth(),												// Глубина Z-Buffer'а (Depth Buffer)  
			0,																// Нет Stencil Buffer'а (пока нету)
			0,																// Нет Auxiliary Buffer'а
			PFD_MAIN_PLANE,													// Главный слой для отрисовки
			0,																// Зарезирвировано
			0, 0, 0															// Игнорируем маску слоёв
		};

		GLuint PixelFormat;													

		PixelFormat = ChoosePixelFormat(m_currentwindow.hDC, &pfd);			
		if (!PixelFormat)												
		{
			// Неудача
			curLog = hst::log::inst();
			curLog->write(hString("PixelFormat() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)m_currentwindow.hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)m_currentwindow.hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (m_currentwindow.hWnd, m_currentwindow.hDC);							
			m_currentwindow.hDC = 0;												
			DestroyWindow (m_currentwindow.hWnd);									
			m_currentwindow.hWnd = 0;												
			return false;													
		}

		if (!SetPixelFormat (m_currentwindow.hDC, PixelFormat, &pfd))		// Пытаемся установить этот формат
		{
			// Неудачно
			hst::log* curLog = hst::log::inst();
			curLog->write(hString("SetPixelFormat() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)m_currentwindow.hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)m_currentwindow.hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (m_currentwindow.hWnd, m_currentwindow.hDC);							// Освобождаем контекст устройсва
			m_currentwindow.hDC = 0;												// Обнуляем его
			DestroyWindow (m_currentwindow.hWnd);									// Убиваем окно
			m_currentwindow.hWnd = 0;												// Обнуляем хэндл окна
			curLog = hst::log::inst();
			curLog->write(hString("PixelFormat() failure!"));
			curLog->save("log.txt");
			return false;													// Возвращаем False (Ложь)
		}

		m_currentwindow.hRC = wglCreateContext(m_currentwindow.hDC);						// Пробуем получить контекст рендера

		if (!m_currentwindow.hRC)												// Мы его получили?
		{
			// Неудачно
			curLog = hst::log::inst();
			curLog->write(hString("wglCreateContext() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)m_currentwindow.hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)m_currentwindow.hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (m_currentwindow.hWnd, m_currentwindow.hDC);							// Освобождаем контекст устройсва
			m_currentwindow.hDC = 0;												// Обнуляем его
			DestroyWindow (m_currentwindow.hWnd);									// Убиваем окно
			m_currentwindow.hWnd = 0;												// Обнуляем хэндл окна
			return false;													// Возвращаем False (Ложь)
		}

		// Делаем этот контект рендера текущим
		if (!wglMakeCurrent(m_currentwindow.hDC, m_currentwindow.hRC))
		{
			// Неудачно
			curLog = hst::log::inst();
			curLog->write(hString("wglMakeCurrent() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)m_currentwindow.hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)m_currentwindow.hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			wglDeleteContext (m_currentwindow.hRC);									// Удаляем контекст рендера
			m_currentwindow.hRC = 0;												// Обнуляем его
			ReleaseDC (m_currentwindow.hWnd, m_currentwindow.hDC);							// Освобождаем контекст устройсва
			m_currentwindow.hDC = 0;												// Обнуляем его
			DestroyWindow (m_currentwindow.hWnd);									// Убиваем окно
			m_currentwindow.hWnd = 0;												// Обнуляем хэндл окна
			return false;													// Возвращаем False (Ложь)
		}



		ShowWindow (m_currentwindow.hWnd, SW_NORMAL);								// Make The Window Visible

		reshape(m_glsettings.screenWidth(),m_glsettings.screenHeight());      // Reshape Our GL Window

		return true;														// Window Creating Was A Success
	}

	void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
	{

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

	bool Renderer::destroyWindowGL()						          		// Destroy The OpenGL Window & Release Resources
	{
		if (m_currentwindow.hWnd != 0)												// Does The Window Have A Handle?
		{	
			if (m_currentwindow.hDC != 0)											// Does The Window Have A Device Context?
			{
				wglMakeCurrent(m_currentwindow.hDC, 0);							// Set The Current Active Rendering Context To Zero
				if (m_currentwindow.hRC != 0)										// Does The Window Have A Rendering Context?
				{
					wglDeleteContext(m_currentwindow.hRC);							// Release The Rendering Context
					m_currentwindow.hRC = 0;										// Zero The Rendering Context

				}
				ReleaseDC(m_currentwindow.hWnd, m_currentwindow.hDC);						// Release The Device Context
				m_currentwindow.hDC = 0;											// Zero The Device Context
			}
			DestroyWindow(m_currentwindow.hWnd);									// Destroy The Window
			m_currentwindow.hWnd = 0;												// Zero The Window Handle
		}

		if (m_glsettings.isFullscreen())										// Is Window In Fullscreen Mode
		{
			ChangeDisplaySettings(NULL,0);									// Switch Back To Desktop Resolution
			ShowCursor(true);												// Show The Cursor
		}	
		return true;														// Return True
	}
	void sad::Renderer::update(){

		getCurrentScene()->render();

	}

	void sad::Renderer::quit(){
	    PostMessage(m_currentwindow.hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
		isProgramLooping=false;
	}

}


