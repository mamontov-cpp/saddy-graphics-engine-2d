#include "os/glcontextimpl.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "sadmutex.h"
#include "renderer.h"
#include "log/log.h"
#include "3rdparty/glext/glext.h"
#ifdef WIN32
#include "3rdparty/glext/wglext.h"
#endif


#ifdef X11

static sad::Mutex context_creation;

static bool context_error_occured = false;

static int context_error_handler( Display *dpy, XErrorEvent *ev )
{
    context_error_occured = true;
    return 0;
}

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
 
{
 
  const char *start;
  const char *where, *terminator;
 
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if ( where || *extension == '\0' )
    return false;
 
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for ( start = extList; ; ) {
    where = strstr( start, extension );
 
    if ( !where )
      break;
 
    terminator = where + strlen( extension );
 
    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;
 
    start = terminator;
  }
 
  return false;
}

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

#ifdef X11
	if (m_win->isGL3compatible())
	{
		bool result = false;
		// Nobody should enter here, only us
		context_creation.lock();
		// Get the default screen's GLX extension list
		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Before querying glXQueryExtensionsString()", r);
		}
		const char *glxExts = glXQueryExtensionsString( 
			m_win->handles()->Dpy,
			m_win->handles()->Screen
		);
		
		XSync( m_win->handles()->Dpy, False );
		
		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Supported extensions are ...", r);
			SL_LOCAL_INTERNAL(glxExts, r);			
		}
		
		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Getting glXCreateContextAttribsARB()", r);
		}
 
		// NOTE: It is not necessary to create or make current to a context before
		// calling glXGetProcAddressARB
		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
		glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
		
		XSync( m_win->handles()->Dpy, False );
		
		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Calling XSetErrorHandler()", r);
		}
		
		
		context_error_occured = false;
		int (*oldHandler)(Display*, XErrorEvent*) =
			XSetErrorHandler(&context_error_handler);
			

		XSync( m_win->handles()->Dpy, False );

		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Checking extension support", r);
		}
		
		if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) 
			||  glXCreateContextAttribsARB == NULL )
		{
			XSync( m_win->handles()->Dpy, False );
			
			if (m_win->renderer())
			{
				sad::Renderer & r = *(m_win->renderer());
				SL_LOCAL_INTERNAL("Trying to create context via glXCreateNewContext()", r);
			}
			m_handle.Context = glXCreateNewContext( 
				m_win->handles()->Dpy,
				m_win->handles()->FBC, 
				GLX_RGBA_TYPE, 
				0, 
				True 
			);
			result = m_handle.Context != NULL;
			glXMakeCurrent(m_win->handles()->Dpy, m_win->handles()->Win, m_handle.Context);  
		}
		else
		{
			if (m_win->renderer())
			{
				sad::Renderer & r = *(m_win->renderer());
				SL_LOCAL_INTERNAL("Started trying to obtain GL3 context", r);
			}
			result  = tryCreateOpenGL3Context(glXCreateContextAttribsARB);
		}

		XSync( m_win->handles()->Dpy, False );
 
		XSetErrorHandler( oldHandler );

		if (context_error_occured)
			result = false;

		context_creation.unlock();

		return result;
	}
	else 
	{
		if (m_win->renderer())
		{
			sad::Renderer & r = *(m_win->renderer());
			SL_LOCAL_INTERNAL("Trying to create simple context", r);
		}
		return this->makeDefaultContext();
	}
#endif

	
	return false;
}



#ifdef X11


bool sad::os::GLContextImpl::tryCreateOpenGL3Context(glXCreateContextAttribsARBProc cc)
{
	if (makeDefaultContext())
	{
		// Try to parse version from context
		int major = 0, minor = 0;
		const char *verstr = (const char *) glGetString(GL_VERSION);
		if ((verstr == NULL) || (sscanf(verstr,"%d.%d", &major, &minor) != 2))
		{
			major = minor = 0;
		}
		// If we could create more advanced context - create it
		if (major * 10 + minor >= 30)
		{
			if (m_win->renderer())
			{
				sad::Renderer & r = *(m_win->renderer());
				SL_LOCAL_INTERNAL("Trying to create GL3 context", r);
			}
			int contextattribs[] =
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB, major,
				GLX_CONTEXT_MINOR_VERSION_ARB, minor,
				GLX_CONTEXT_PROFILE_MASK_ARB , GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				None
			};
			GLXContext context = cc( 
				m_win->handles()->Dpy, 
				m_win->handles()->FBC, 
				0,
				True, 
				contextattribs 
			);
			if (context != NULL)
			{
				// We did it! Finally new context
				glXMakeCurrent(m_win->handles()->Dpy, 0, 0);    
				glXDestroyContext(m_win->handles()->Dpy, m_handle.Context);
				m_handle.Context = context;
				glXMakeCurrent(m_win->handles()->Dpy, m_win->handles()->Win, m_handle.Context);  
				m_isopengl3compatible = true;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool sad::os::GLContextImpl::makeDefaultContext()
{
	m_isopengl3compatible = false;
	m_handle.Context = glXCreateContext(
		m_win->handles()->Dpy,
		m_win->handles()->VisualInfo, 
		0, 
		GL_TRUE
	);
	if (m_handle.Context != NULL)
	{
		glXMakeCurrent(
			m_win->handles()->Dpy, 
			m_win->handles()->Win, 
			m_handle.Context
		);
		return true;
	}
	return false;
}

#endif

void sad::os::GLContextImpl::destroy()
{
	if (!valid())
		return;

#ifdef WIN32
	wglMakeCurrent(m_win->handles()->DC, 0); 
	wglDeleteContext(m_handle.Context); 
#endif

#ifdef X11
	glXMakeCurrent(m_win->handles()->Dpy, 0 , 0);    
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


#ifndef DEFAULT_DEPTH_VALUE
#define DEFAULT_DEPTH_VALUE 0.8f //!< Value, which gives us a z=0.5 in mapping coordinates
#endif

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
		winz = DEFAULT_DEPTH_VALUE;


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
