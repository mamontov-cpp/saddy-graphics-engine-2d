#include "os/glcontextimpl.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "3rdparty/glext/glext.h"
#ifdef WIN32
#include "3rdparty/glext/wglext.h"
#endif



sad::os::GLContextImpl::GLContextImpl()
: m_win(NULL), m_handle(), m_isopengl3compatible(false)
{

}

sad::os::GLContextImpl::~GLContextImpl()
{
	if (valid())
	{
		destroy();
	}
}


#ifdef WIN32
typedef HGLRC (APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC,HGLRC, const int *);
#endif


bool sad::os::GLContextImpl::createFor(sad::Window * win)
{
	if (valid())
		return true;

	m_win = win;
	m_isopengl3compatible = false;

#ifdef WIN32
	bool result = false;
	m_handle.Context = wglCreateContext(win->handles()->DC); 
	if(m_handle.Context) 
	{
		wglMakeCurrent(win->handles()->DC, m_handle.Context);
		result = true;
		// Try to parse version from context
		int major = 0, minor = 0;
		const char *verstr = (const char *) glGetString(GL_VERSION);
		if ((verstr == NULL) || (sscanf(verstr,"%d.%d", &major, &minor) != 2))
		{
			major = minor = 0;
		}

		if (major * 10 + minor >= 30)
		{
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB != NULL)
			{
				int attributes[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB, 
					major,
					WGL_CONTEXT_MINOR_VERSION_ARB, 
					minor,
					WGL_CONTEXT_FLAGS_ARB,         
					WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					0
				};
				HGLRC newContext = wglCreateContextAttribsARB(
					win->handles()->DC, 
					0, 
					attributes
				);
				if (newContext != NULL)
				{
					m_isopengl3compatible = true;
					wglDeleteContext(m_handle.Context);
					m_handle.Context = newContext;
					wglMakeCurrent(win->handles()->DC, m_handle.Context);
				}
			}
		}
	} 
	
	return result;
#endif

	// Stub
	return false;
}

void sad::os::GLContextImpl::destroy()
{
	if (!valid())
		return;

#ifdef WIN32
	wglMakeCurrent(m_win->handles()->DC, 0); 
	wglDeleteContext(m_handle.Context); 
#endif

#ifdef X11
	glXMakeCurrent(m_win->handles()->Dpy, 0, 0);    
    glXDestroyContext(m_win->handles()->Dpy, m_handle.Context);
#endif

	m_isopengl3compatible = false;
	
	m_handle.cleanup();
}

bool sad::os::GLContextImpl::valid() const
{
	return m_handle.valid();
}

void sad::os::GLContextImpl::swapBuffers()
{
	if (m_win->valid() == false)
		return;

#ifdef WIN32
	SwapBuffers(m_win->handles()->DC);
#endif

#ifdef X11
	glXSwapBuffers(m_win->handles()->Dpy, m_win->handles()->Win);
#endif
}

sad::Point3D sad::os::GLContextImpl::mapToViewport(const sad::Point2D & p, bool ztest)
{
	if (m_win->valid() == false || !valid())
		return p;

	GLint     viewport[4];
	GLdouble  modelview[16];
	GLdouble  projection[16];

	GLfloat winx=0,winy=0,winz=0;
	GLdouble result[3];
	
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT,viewport);

	winx=(float)p.x();
#ifdef WIN32  // On win32 we explicitly handle coordinates
	winy=(float)(p.y());
#else
	winy=(float)(viewport[3] - p.y());
#endif
	if (ztest)
		glReadPixels(winx,(int)winy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winz);
	else
		winz = 0;


	gluUnProject(winx,winy,winz,modelview,projection,viewport,result,result+1,result+2);
	
	return sad::Point3D(result[0], result[1], result[2]);
}

sad::os::GLContextHandle * sad::os::GLContextImpl::handle() const
{
	return &(const_cast<sad::os::GLContextImpl *>(this)->m_handle);
}

sad::Window * sad::os::GLContextImpl::window() const
{
	return m_win;
}


bool sad::os::GLContextImpl::isOpenGL3compatible() const
{
	return m_isopengl3compatible;
}
