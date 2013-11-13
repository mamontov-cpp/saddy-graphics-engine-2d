#include "os/glcontextimpl.h"
#include <GL/gl.h>
#include <GL/glu.h>

sad::os::GLContextImpl::GLContextImpl()
{

}

sad::os::GLContextImpl::~GLContextImpl()
{
	if (valid())
	{
		destroy();
	}
}


bool sad::os::GLContextImpl::createFor(sad::Window * win)
{
	if (valid())
		return true;

	m_win = win;

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
