#include "renderer.h"
#include "texturemanager.h"
#include "../../include/log/log.h"
#include "../../include/3rdparty/format/format.h"


#ifdef WIN32

#define UNIQUE_CLASS_NAME  "SADDY_RENDERER_WINDOW_INSTANCE_V_2"


hst::point<hst::D3,float> sad::Renderer::mousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->m_window.hWND,&p);
	p = sad::Renderer::_toClient(p);
	float px=0.0f, py=0.0f,pz=0.0f; 
	mapToOGL(p.x,p.y,px,py,pz);
	return hst::point<hst::D3,float>(px,py,pz);
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
	m_window.window_class += hst::string::number(reinterpret_cast<unsigned long>(this));
	wc.lpszClassName = m_window.window_class.data();

	if (!RegisterClass(&wc)) { SL_LOCAL_FATAL("Failed to register class", *this); return false;}
	
	//Setup fullscreen or windowed mode
	DWORD       ex_style=0;
	DWORD       style=0;
	this->adjustVideoMode(style,ex_style);
	AdjustWindowRectEx(&rect,style,FALSE,ex_style);

	//Create error
	m_window.hWND=CreateWindowExA(ex_style,m_window.window_class.data(),m_windowtitle.data(),style,
		                         0,0,rect.right-rect.left,rect.bottom-rect.top,
								 NULL,NULL,m_window.hInstance,NULL);

	if (m_window.hWND)
	{
	  SetWindowPos(m_window.hWND, NULL, 0, 0, rect.right-rect.left,rect.bottom-rect.top, SWP_NOSENDCHANGING);  
	  RECT r;
	  GetWindowRect(m_window.hWND, &r);
	  //SL_WARNING(fmt::Format("Created window with {0} {1} {2} {3}") << r.left << r.top << r.right << r.bottom);
	}
	// Compute how screen should be mapped if CreateWindowExA creates smaller window
	// than requested
	m_window.clientwidth = m_glsettings.width();
	m_window.clientheight = m_glsettings.height();

	//long dx = (rect.right - rect.left) - (r.right - r.left);
	//long dy = (rect.bottom - rect.top) - (r.bottom - r.top);
	
	//m_window.clientwidth -= dx;
	//m_window.clientheight -= dy;

	
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
	reshape(m_window.clientwidth,m_window.clientheight);

	if (!initGLRendering())
	{
		this->releaseWindow();
		return false;
	}

	return true;
}

void sad::Renderer::adjustVideoMode(unsigned long & style, unsigned long & ex_style)
{
	SL_LOCAL_SCOPE("sad::Renderer::adjustVideoMode()", *this);
	DEVMODEA & scr_settings=m_window.scr_settings;
	memset(&scr_settings,0,sizeof(scr_settings));
    scr_settings.dmSize       = sizeof(scr_settings);
    scr_settings.dmPelsWidth  = m_glsettings.width();
    scr_settings.dmPelsHeight = m_glsettings.height();
    scr_settings.dmBitsPerPel = 32;
    scr_settings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (m_window.fullscreen)
	{
		LONG result=ChangeDisplaySettings(&scr_settings,CDS_FULLSCREEN);
		if (result!=DISP_CHANGE_SUCCESSFUL)
		{
			SL_LOCAL_CRITICAL(hst::string("Changing to fullscreen failed, switching to window . Failed with code") + hst::string::number(result), *this);
			m_window.fullscreen=false;
		}
	}
	//If successful change
	if (m_window.fullscreen)
	{
		ex_style=WS_EX_APPWINDOW;
		style=WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		ex_style=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        style=WS_OVERLAPPEDWINDOW;
	}
}
bool sad::Renderer::setupPFD()
{
	SL_LOCAL_SCOPE("sad::Renderer::setupPFD()", *this);
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		16,
		0,0,0,0,0,0,
		0,              //No alpha buffer
		0,
		0,
		0,0,0,0,
		16,
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
 if (m_setimmediately || m_timer.elapsed() > 600.0)
 {
	 setFPS( 1000.0 * m_frames / m_timer.elapsed() );
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

void sad::Renderer::setWindowTitle(const hst::string & s)
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

void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  if (m_running)
  {
   this->m_window.fullscreen=!this->m_window.fullscreen;
   LONG result;
   static RECT rct={0,0,0,0};
   if (this->m_window.fullscreen)
   {
		GetWindowRect(this->m_window.hWND,&rct);
		MoveWindow(m_window.hWND,0,0,rct.right-rct.left,rct.bottom-rct.top,TRUE);
		LONG style=GetWindowLong(m_window.hWND,GWL_STYLE);
		style &=~(WS_BORDER|WS_CAPTION|WS_THICKFRAME);
		SetWindowLong(m_window.hWND,GWL_STYLE,style);
		BOOL dp=SetWindowPos(this->m_window.hWND,NULL,rct.left,rct.top,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED);
	    if (!dp)
		{
			std::string c = str(fmt::Format("Can\'t position window, failed with {0}") << GetLastError());
			SL_LOCAL_CRITICAL(c,*this);
		}
		result=ChangeDisplaySettings(&(this->m_window.scr_settings),CDS_FULLSCREEN);	
   }
   else
   {
	    MoveWindow(m_window.hWND,rct.left,rct.top,rct.right-rct.left,rct.bottom-rct.top,TRUE);
	    result=ChangeDisplaySettings(NULL,0);
		LONG style=GetWindowLong(m_window.hWND,GWL_STYLE);
		style |=(WS_BORDER|WS_CAPTION|WS_THICKFRAME);
		SetWindowLong(m_window.hWND,GWL_STYLE,style);
		SetWindowPos(this->m_window.hWND,0,rct.left,rct.top,rct.right-rct.left,rct.bottom-rct.top,SWP_FRAMECHANGED);
   }
   if (result!=DISP_CHANGE_SUCCESSFUL)
   {
	   SL_LOCAL_CRITICAL("Can\'t changed mode",*this);
   }
  }
  else
  {
	  this->m_window.fullscreen=!this->m_window.fullscreen;
  }
}


void sad::Renderer::toggleFixedOn()
{
	LONG style=GetWindowLongA(m_window.hWND,GWL_STYLE);
	style |= WS_OVERLAPPED;
    style &=  ~WS_THICKFRAME;
	style &=  ~WS_MAXIMIZEBOX;
	LONG result=SetWindowLongA(m_window.hWND,GWL_STYLE,style);
}
void sad::Renderer::toggleFixedOff()
{
	LONG style=GetWindowLongA(m_window.hWND,GWL_STYLE);
	style |=WS_OVERLAPPED;
	style |= WS_THICKFRAME;
	style |= WS_MAXIMIZEBOX;
	SetWindowLongA(m_window.hWND,GWL_STYLE,style);
}
#endif
