#include "os/windowimpl.h"
#include "renderer.h"



#define SL_WINDOW_INTERNAL_SCOPE(X)   \
if (this->renderer()) {				  \
	SL_INTERNAL_SCOPE(X, *(this->renderer()));  \
}

sad::os::WindowImpl::WindowImpl()
: 
m_handles(), 
m_fixed(false), 
m_fullscreen(false),
m_hidden(true),
m_creation_size(320, 240),
m_window_rect_stack(),
m_renderer(NULL),
m_window_title("Saddy Engine")
{

}

sad::os::WindowImpl::~WindowImpl()
{
	if (valid())
	{
		destroy();
	}
}

void sad::os::WindowImpl::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::os::WindowImpl::renderer() const
{
	return m_renderer;
}

void sad::os::WindowImpl::setCreationSize(const sad::Size2I& size)
{
	m_creation_size = size;
}


bool sad::os::WindowImpl::create()
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::create()");
	// Do not do anything, whether window implementation is valid
	if (valid())
		return true;

	typedef bool (sad::os::WindowImpl::*CreationStep)(bool);
	typedef void (sad::os::WindowImpl::*CleanupStep)();
#ifdef WIN32
	
	const int creationstepscount = 4;
	
	CreationStep creationsteps[creationstepscount] = {
		&sad::os::WindowImpl::registerWindowClass,
		&sad::os::WindowImpl::adjustWindowRect,
		&sad::os::WindowImpl::makeWindowAndObtainDeviceContext,
		&sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor
	};

	const int cleanupstepcount = 2;
	
	CleanupStep cleanupsteps[cleanupstepcount] = {
		&sad::os::WindowImpl::releaseContextAndDestroyWindow,
		&sad::os::WindowImpl::unregisterWindowClass		
	};
#endif

#ifdef X11
	const int creationstepscount = 3;
	
	CreationStep creationsteps[creationstepscount] = {
		&sad::os::WindowImpl::openConnectionAndScreen,
		&sad::os::WindowImpl::chooseVisualInfo,
		&sad::os::WindowImpl::createWindow
	};

	const int cleanupstepcount = 1;
	
	CleanupStep cleanupsteps[cleanupstepcount] = {
		&sad::os::WindowImpl::closeConnection
	};
#endif

	bool result = true;
	for(int i = 0; i < creationstepscount; i++)
	{
		CreationStep step = creationsteps[i];
		result = result && (this->*step)(result);
	}

	if (result == false)
	{
		if (this->renderer() != NULL)
		{
			SL_LOCAL_FATAL(
				fmt::Format("Failed to create window") << m_handles.Class,
				*(this->renderer())
			);
		}

		for(int i =  0; i < cleanupstepcount; i++ )
		{
			CleanupStep  step = cleanupsteps[i];
			(this->*step)();
		}
	}

	
	return result;
}

#ifdef WIN32

// External loop handler
LRESULT CALLBACK sad_renderer_window_proc (
	HWND hWnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam
);

bool sad::os::WindowImpl::registerWindowClass(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::registerWindowClass()");

	if (!lastresult) 
	{
		return false;
	}

	m_handles.ProcessInstance = GetModuleHandle(NULL); 

	WNDCLASS    wc;
	wc.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) sad_renderer_window_proc;
	wc.cbClsExtra  =  0;
	wc.cbWndExtra  =  0;
	wc.hInstance   =  m_handles.ProcessInstance;
	wc.hIcon       =  LoadIcon(NULL,IDI_WINLOGO);       
    wc.hCursor     =  LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	
	m_handles.Class = "sad::os::WindowImpl at ";
	m_handles.Class += sad::String::number(reinterpret_cast<unsigned long>(this));
	
	wc.lpszClassName = m_handles.Class.data();

	ATOM registerresult = RegisterClassA(&wc);
	bool result = (registerresult != 0);
    if (result == false && this->renderer() != NULL)
	{
		SL_LOCAL_FATAL(
			fmt::Format("Failed to register class \"{0}\"") << m_handles.Class,
			*(this->renderer())
		);

		m_handles.ProcessInstance = NULL;
		m_handles.Class.clear();
	}
	return result;
	
}

void sad::os::WindowImpl::unregisterWindowClass()
{
	if (m_handles.Class.length() != 0)
	{
		UnregisterClassA(m_handles.Class.data(), m_handles.ProcessInstance);

		m_handles.Class.clear();
		m_handles.ProcessInstance = NULL;
	}
}

bool sad::os::WindowImpl::adjustWindowRect(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::adjustWindowRect()");

	if (!lastresult) 
	{
		return false;
	}
	// Compute window rectangle size, adjusting it to needed rectangle
	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW;

	m_handles.AdjustedWindowRect.left  =(long)0;
	m_handles.AdjustedWindowRect.right =(long)m_creation_size.Width;
	m_handles.AdjustedWindowRect.top   =(long)0;
	m_handles.AdjustedWindowRect.bottom=(long)m_creation_size.Height;

	if (this->renderer() != NULL)
	{
		SL_LOCAL_INTERNAL(
			fmt::Format("Requested client size is [left: {0}, right: {1}, top: {2}, bottom: {3}]")
			<< m_handles.AdjustedWindowRect.left
			<< m_handles.AdjustedWindowRect.right
			<< m_handles.AdjustedWindowRect.top
			<< m_handles.AdjustedWindowRect.bottom			
			, *(this->renderer())
		);
	}

	AdjustWindowRectEx(&(m_handles.AdjustedWindowRect),style,FALSE,ex_style);
	
	if (this->renderer() != NULL)
	{
		SL_LOCAL_INTERNAL(
			fmt::Format("Adjusted window size is [left: {0}, right: {1}, top: {2}, bottom: {3}]")
			<< m_handles.AdjustedWindowRect.left
			<< m_handles.AdjustedWindowRect.right
			<< m_handles.AdjustedWindowRect.top
			<< m_handles.AdjustedWindowRect.bottom			
			, *(this->renderer())
		);
	}
	return true;
}

bool sad::os::WindowImpl::makeWindowAndObtainDeviceContext(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::makeWindowAndObtainDeviceContext()");

	if (!lastresult) 
	{
		return false;
	}
	m_handles.WND = CreateWindowExA(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		m_handles.Class.data(),
		m_window_title.data(),
		WS_OVERLAPPEDWINDOW,
		m_handles.AdjustedWindowRect.left,
		m_handles.AdjustedWindowRect.top,
		m_handles.AdjustedWindowRect.right,
		m_handles.AdjustedWindowRect.bottom,
		NULL,
		NULL,
		m_handles.ProcessInstance,
		NULL
	);

	bool result = (m_handles.WND != NULL);
	if (m_handles.WND == NULL)
	{
		if (this->renderer() != NULL)
		{
			SL_LOCAL_INTERNAL(
				"CreateWindowExA() failed", *(this->renderer())
			);
		}
	}
	else
	{
		m_handles.DC = GetDC(m_handles.WND);
		if (m_handles.DC == NULL)
		{
			result = false;
			if (this->renderer() != NULL)
			{
				SL_LOCAL_INTERNAL(
					"GetDC() failed", *(this->renderer())
				);
			}
		}
	}
	
	if (result && this->renderer() != NULL)
	{
		SetWindowPos(
			m_handles.WND, 
			HWND_NOTOPMOST, 
			0, 
			0, 
			m_handles.AdjustedWindowRect.right - m_handles.AdjustedWindowRect.left,
			m_handles.AdjustedWindowRect.bottom - m_handles.AdjustedWindowRect.top, 
			SWP_NOSENDCHANGING | SWP_SHOWWINDOW
		);  
		
		RECT windowrect;
		GetWindowRect(m_handles.WND, &windowrect);
		SL_LOCAL_INTERNAL(
			fmt::Format("Created window rectangle with [left: {0}, top: {1}, right: {2}, bottom: {3}]") 
			<< windowrect.left 
			<< windowrect.top 
			<< windowrect.right 
			<< windowrect.bottom,
			*(this->renderer())
	    );

		RECT clientrect;
		GetClientRect(m_handles.WND, &clientrect);
		SL_LOCAL_INTERNAL(
			fmt::Format("Created client rectangle with [left: {0}, top: {1}, right: {2}, bottom: {3}]") 
			<< clientrect.left 
			<< clientrect.top 
			<< clientrect.right 
			<< clientrect.bottom,
			*(this->renderer())
		);
	}
	return result;
}

void sad::os::WindowImpl::releaseContextAndDestroyWindow()
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::releaseContextAndDestroyWindow()");

	if (m_handles.DC && !ReleaseDC(m_handles.WND,m_handles.DC))
	{
		SL_LOCAL_INTERNAL("ReleaseDC() failed", *(this->renderer()));
		m_handles.DC = NULL;
	}
  
	if (m_handles.WND && !DestroyWindow(m_handles.WND))
	{
		SL_LOCAL_INTERNAL("DestroyWindow() failed", *(this->renderer()));
		m_handles.WND = NULL;
	}
}

bool sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::chooseAndSetPixelFormatDescriptor()");

	if (!lastresult)
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		PFD_TYPE_RGBA,
		0,0,0,0,0,0,
		0,              
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

	int      pixelformat=0; //Pixel format
	pixelformat = ChoosePixelFormat(m_handles.DC, &pfd);

	bool result = true;
	if (!pixelformat)
	{
		result = false;
		if (this->renderer() != NULL)
		{
			SL_LOCAL_INTERNAL("ChoosePixelFormat() failed", *(this->renderer()));
		}
	} 
	else
	{
		if (!SetPixelFormat(m_handles.DC, pixelformat, &pfd))
		{
			result = false;
			if (this->renderer() != NULL)
			{
				SL_LOCAL_INTERNAL("SetPixelFormat() failed", *(this->renderer()));
			}
		}
	}
	return true;
}

#endif

#ifdef X11

bool sad::os::WindowImpl::openConnectionAndScreen(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::openConnectionAndScreen()");
	if (!lastresult)
	{
		return false;
	}
	
	m_handles.Dpy = XOpenDisplay(0);
  	m_handles.Screen = DefaultScreen(m_handles.Dpy);
	
	return true;
}

void sad::os::WindowImpl::closeConnection()
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::closeConnection()");
	if (m_handles.Dpy != NULL)
	{
		XCloseDisplay(m_handles.Dpy);
	}
}

bool sad::os::WindowImpl::chooseVisualInfo(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::chooseVisualInfo()");
	if (!lastresult)
	{
		return false;
	}

	int attrlistsinglebuffered[] = {
		GLX_RGBA, 
		GLX_RED_SIZE,   1, 
		GLX_GREEN_SIZE, 1, 
		GLX_BLUE_SIZE,  1, 
		GLX_DEPTH_SIZE, 4,
		None
	};

	int attrlistdoublebuffered[] = { 
		GLX_RGBA, GLX_DOUBLEBUFFER, 
		GLX_RED_SIZE, 1, 
		GLX_GREEN_SIZE, 1, 
		GLX_BLUE_SIZE, 1, 
		GLX_DEPTH_SIZE, 4,
		None 
	};

	m_handles.VisualInfo = glXChooseVisual(
		m_handles.Dpy, 
		m_handles.Screen, 
		attrlistdoublebuffered
	);

  	if (m_handles.VisualInfo == NULL)
  	{
        m_handles.VisualInfo = glXChooseVisual(
			m_handles.Dpy, 
			m_handles.Screen, 
			attrlistsinglebuffered
		);
		if (this->renderer() != NULL)
		{
			SL_LOCAL_INTERNAL("glXChooseVisual() failed, defaulting to singlebuffering", *(this->renderer()));
		}
    }
	bool result =  true;
  	if (m_handles.VisualInfo == NULL)
  	{
  		if (this->renderer() != NULL)
		{
			SL_LOCAL_INTERNAL("glXChooseVisual() failed for singlebuffering", *(this->renderer()));
		}
        result = false;
  	}
	return result;
}

bool sad::os::WindowImpl::createWindow(bool lastresult)
{
	SL_WINDOW_INTERNAL_SCOPE("sad::os::WindowImpl::createWindow()");
	if (!lastresult)
	{
		return false;
	}

	Colormap cmap;
	Atom wmDelete;
	X11Window winDummy;

	 cmap = XCreateColormap(
		 m_handles.Dpy, 
		 RootWindow(m_handles.Dpy, m_handles.VisualInfo->screen),
		 m_handles.VisualInfo->visual, 
		 AllocNone
	);
 
	XSetWindowAttributes attr;
	attr.colormap = cmap;
	attr.border_pixel = 0;
	attr.event_mask = ExposureMask 
					| KeyPressMask 
					| ButtonPressMask 
					| StructureNotifyMask 
					| PointerMotionMask 
					| ButtonReleaseMask 
					| KeyReleaseMask;
 
	m_handles.Win = XCreateWindow(
		m_handles.Dpy, 
		RootWindow(m_handles.Dpy, m_handles.VisualInfo->screen), 
		0, 
		0, 
		m_creation_size.Width,
		m_creation_size.Height, 
		0, 
		m_handles.VisualInfo->depth, 
		InputOutput, 
		m_handles.VisualInfo->visual, 
		CWBorderPixel | CWColormap | CWEventMask, 
		&attr
	);

	wmDelete = XInternAtom(m_handles.Dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(m_handles.Dpy,  m_handles.Win, &wmDelete, 1);
	XSetStandardProperties(
		m_handles.Dpy, 
		m_handles.Win,
		m_window_title.data(), 
		m_window_title.data(), 
		None, 
		NULL, 
		0, 
		NULL
	);
	XMapRaised(m_handles.Dpy,  m_handles.Win);

	return true;
}

#endif
