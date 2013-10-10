#include "renderer.h"
#include "texturemanager.h"
#include "../../include/log/log.h"
#include "../../include/3rdparty/format/format.h"


#ifdef WIN32

#define UNIQUE_CLASS_NAME  "SADDY_RENDERER_WINDOW_INSTANCE_V_2"


sad::Point3D sad::Renderer::mousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->m_window.hWND,&p);
	p = sad::Renderer::_toClient(p);
	float px=0.0f, py=0.0f,pz=0.0f; 
	mapToOGL(p.x,p.y,px,py,pz);
	return sad::Point3D(px,py,pz);
}
void sad::Renderer::releaseWindow()
{
  SL_LOCAL_SCOPE("sad::Renderer::releaseWindow()", *this);
  if (m_window.fullscreen) //If fullscreen
  {
	  ChangeDisplaySettings(NULL,0);
	  ShowCursor(TRUE);
  }
  
  // Unload all textures, because their freeing will cause segfault on some machines
  this->m_texture_manager->unload();
  // Release rendering context
  if (m_window.hRC)
  {
	  if (!wglMakeCurrent(NULL,NULL)) 
		    SL_LOCAL_CRITICAL("Failed to release OpenGL context", *this);
	  if (!wglDeleteContext(m_window.hRC))     
		    SL_LOCAL_CRITICAL("Failed to release rendering context", *this);
	  m_window.hRC=NULL;
  }

  //Release device context
  if (m_window.hDC && !ReleaseDC(m_window.hWND,m_window.hDC))
  {
	   SL_LOCAL_CRITICAL("Failed to release hDC", *this);
       m_window.hDC=NULL;
  }
  //If we can destroy the window
  if (m_window.hWND && !DestroyWindow(m_window.hWND))
  {
	  SL_LOCAL_CRITICAL("Failed to release hWND", *this);
      m_window.hWND=NULL;
  }
  
  //Unregister Class
  if (!UnregisterClass(m_window.window_class.data(),m_window.hInstance))
  {
	  SL_LOCAL_CRITICAL("Failed to unregister class", *this);
	  m_window.hInstance=NULL;
  }

}

bool sad::Renderer::createWindow()
{
    SL_LOCAL_SCOPE("sad::Renderer::createWindow()", *this);
	//Window parameters
	WNDCLASS    wc;
    //Rectangle
	RECT        rect;
    rect.left  =(long)0;
	rect.right =(long)this->m_glsettings.width();
	rect.top   =(long)0;
	rect.bottom=(long)this->m_glsettings.height();


	//Init window
	m_window.hInstance = GetModuleHandle(NULL); 
    wc.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) sad::Renderer::WindowProc;
	wc.cbClsExtra  =  0;
	wc.cbWndExtra  =  0;
	wc.hInstance   =  m_window.hInstance;
	wc.hIcon       =  LoadIcon(NULL,IDI_WINLOGO);             // /TODO: Must be replaced in new releases (default icon)
    wc.hCursor     =  LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	m_window.window_class = UNIQUE_CLASS_NAME;
	m_window.window_class += sad::String::number(reinterpret_cast<unsigned long>(this));
	wc.lpszClassName = m_window.window_class.data();

	if (!RegisterClass(&wc)) { SL_LOCAL_FATAL("Failed to register class", *this); return false;}
	
	//Setup indowed mode
	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect,style,FALSE,ex_style);

	//Create new window
	m_window.hWND=CreateWindowExA(ex_style,m_window.window_class.data(),m_windowtitle.data(),style,
		                         rect.left,rect.top,rect.right,rect.bottom,
								 NULL,NULL,m_window.hInstance,NULL);

	// Compute how screen should be mapped if CreateWindowExA creates smaller window
	// than requested
	m_window.clientwidth = m_glsettings.width();
	m_window.clientheight = m_glsettings.height();
	
	//Handle error
	if (!m_window.hWND)
	{
		releaseWindow();
		SL_LOCAL_FATAL("Failed to create window", *this);
		return false;
	}
	//Getting a device context
	this->m_window.hDC=GetDC(m_window.hWND);
	if (!m_window.hDC)
	{
		SL_LOCAL_FATAL("Failed to create a device context, destroying window...", *this);
		releaseWindow();
		return false;
	}
	if (!setupPFD())
	{
	    SL_LOCAL_FATAL("Failed to find setup pixel format descripted...", *this);
	    this->releaseWindow();
		return false;
	}
	//Creating context
	if (!(m_window.hRC=wglCreateContext(m_window.hDC)))
	{
		SL_LOCAL_FATAL("Can't create a rendering context...", *this);
		this->releaseWindow();
		return false;
	}
	//Trying to activate a context
	if (!wglMakeCurrent(m_window.hDC,m_window.hRC))
	{
		SL_LOCAL_FATAL("Can't activate a rendering context...", *this);
		this->releaseWindow();
		return false;
	}

	ShowWindow(m_window.hWND,SW_SHOW);
	SetForegroundWindow(m_window.hWND);
	SetFocus(m_window.hWND);

	if (!initGLRendering())
	{
		this->releaseWindow();
		return false;
	}

	SetWindowPos(m_window.hWND, HWND_NOTOPMOST, 0, 0, rect.right-rect.left,rect.bottom-rect.top, SWP_NOSENDCHANGING | SWP_SHOWWINDOW);  
	RECT windowrect;
	RECT clientrect;
	GetWindowRect(m_window.hWND, &windowrect);
	SL_DEBUG(fmt::Format("Window rect is set to with {0} {1} {2} {3}") << windowrect.left << windowrect.top << windowrect.right << windowrect.bottom);
	GetClientRect(m_window.hWND, &clientrect);
	SL_DEBUG(fmt::Format("Client rect is set to with {0} {1} {2} {3}") << clientrect.left << clientrect.top << clientrect.right << clientrect.bottom);
	
	reshape(m_window.width, m_window.height);
		
	if (m_window.fullscreen)
	{
		enterFullScreen();
		reshape(m_window.width, m_window.height);
	}

	return true;
}

bool sad::Renderer::setupPFD()
{
	SL_LOCAL_SCOPE("sad::Renderer::setupPFD()", *this);
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		PFD_TYPE_RGBA,
		0,0,0,0,0,0,
		0,              //No alpha buffer
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	GLuint      pixel_format=0; //Pixel format
	
	if (!(pixel_format=ChoosePixelFormat(m_window.hDC,&pfd)))
	{
		SL_LOCAL_FATAL("Can't find a suitable pixel format descriptor", *this);
		return false;
	}
	if (!SetPixelFormat(m_window.hDC,pixel_format,&pfd))
	{
		SL_LOCAL_FATAL("Can't setup a suitable pixel format descriptor", *this);
		return false;
	}

	return true;
}

void sad::Renderer::update()
{
 if (m_setimmediately || m_reset)
 {
	 m_timer.start();
	 m_reset = false;
 }
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 getCurrentScene()->render();
 SwapBuffers(m_window.hDC);
 ++m_frames;
 m_timer.stop();
 double elapsed = m_timer.elapsed();
 if (m_setimmediately || elapsed > 500.0)
 {
	 double newfps = 1000.0 * m_frames / elapsed; 
	 setFPS( newfps );
	 m_frames = 0;
	 m_reset = true;
	 m_setimmediately = false;
 }
}

void sad::Renderer::initWindowParameters(void)
{
  this->m_window.hRC=NULL;
  this->m_window.hDC=NULL;
  this->m_window.hWND=NULL;
  this->m_window.hInstance=NULL;
}

void sad::Renderer::setWindowTitle(const sad::String & s)
{
	m_windowtitle=s;
	if (m_window.hWND)
         SetWindowTextA(m_window.hWND,s.data()); 
}
void sad::Renderer::quit()
{
	    PostMessage(m_window.hWND, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
		m_running=false;
		m_created=false;
}

void sad::Renderer::enterFullScreen()
{
	RECT rct={0,0,0,0};

	// Save old position
	GetWindowRect(m_window.hWND, &rct);
	m_window.previousx = rct.left;
	m_window.previousy = rct.top;
	m_window.previouswidth = rct.right - rct.left;
	m_window.previousheight = rct.bottom - rct.top;
	m_window.previousstyle = GetWindowLongA(m_window.hWND, GWL_STYLE);
		
	const long screenwidth = ::GetSystemMetrics(SM_CXSCREEN);
	const long screenheight = ::GetSystemMetrics(SM_CYSCREEN);
	SetWindowLongPtr(m_window.hWND, GWL_STYLE, 
		WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	SetWindowPos(m_window.hWND, HWND_TOPMOST, 0, 0, screenwidth, screenheight, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);	

	m_window.width = screenwidth;
	m_window.height = screenheight;
}

void sad::Renderer::leaveFullScreen()
{
	SL_DEBUG("sad::Renderer::leaveFullScreen");
	SetWindowLongPtr(m_window.hWND, GWL_STYLE, m_window.previousstyle);
	SetWindowPos(m_window.hWND, 
				 HWND_NOTOPMOST, 
				 m_window.previousx, 
				 m_window.previousy, 
				 m_window.previouswidth, 
				 m_window.previousheight, 
				 SWP_SHOWWINDOW | SWP_NOSENDCHANGING  
				);	
}

void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  m_window.fullscreen=!m_window.fullscreen;   
  if (m_running)
  {
   // DON'T CHANGE DISPLAY SETTINGS
   // We can broke everything, using it 
   // just because driver flushes context-associated memory
   if (m_window.fullscreen)
   {
		enterFullScreen();
   }
   else
   {
		leaveFullScreen();
   }
  }
}


void sad::Renderer::toggleFixedOn()
{
	SL_SCOPE("sad::Renderer::toggleFixedOn()");

	LONG style=GetWindowLongA(m_window.hWND,GWL_STYLE);
	style |= WS_OVERLAPPED;
	style &=  ~WS_THICKFRAME;
	style &=  ~WS_MAXIMIZEBOX;
	LONG result=SetWindowLongA(m_window.hWND,GWL_STYLE,style);

	// Force updating window rectangle, because some OS fails to redraw it
	RECT r;
	GetWindowRect(m_window.hWND, &r);
	SetWindowPos(m_window.hWND, HWND_NOTOPMOST, r.left, r.top, r.right,r.bottom, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME );
	reshape(r.right - r.left, r.bottom - r.top);
	
}
void sad::Renderer::toggleFixedOff()
{
	SL_SCOPE("sad::Renderer::toggleFixedOff()");

	LONG style=GetWindowLongA(m_window.hWND,GWL_STYLE);
	style |=WS_OVERLAPPED;
	style |= WS_THICKFRAME;
	style &= ~WS_SYSMENU;
	style |= WS_MAXIMIZEBOX;
	SetWindowLongA(m_window.hWND,GWL_STYLE,style);

	// Force updating window rectangle, because some OS fails to redraw it
	RECT r;
	GetWindowRect(m_window.hWND, &r);
	SetWindowPos(m_window.hWND, HWND_NOTOPMOST, r.left, r.top, r.right,r.bottom, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME );
	reshape(r.right - r.left, r.bottom - r.top);
}
#endif
