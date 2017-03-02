#include "sadqtglcontext.h"

#include <window.h>
#include <renderer.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>


sad::qt::GLContext::GLContext() : m_window(NULL), m_valid(false)
{
	
}

sad::qt::GLContext::~GLContext()
{

}


bool sad::qt::GLContext::valid() const
{
	return m_valid;
}


void sad::qt::GLContext::swapBuffers()
{
	
}

#ifndef DEFAULT_DEPTH_VALUE
#define DEFAULT_DEPTH_VALUE 0.8f //!< Value, which gives us a z=0.5 in mapping coordinates
#endif

sad::Point3D sad::qt::GLContext::mapToViewport(const sad::Point2D & p, bool ztest)
{
	if (!m_window)
		return p;
	if (m_window->valid() == false || !valid())
		return p;

	GLint     viewport[4];
	GLdouble  modelview[16];
	GLdouble  projection[16];

	GLfloat winx = 0, winy = 0, winz = 0;
	GLdouble result[3];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winx = (float)p.x();
#ifdef WIN32  // On win32 we explicitly handle coordinates
	winy = (float)(p.y());
#else
	winy = (float)(viewport[3] - p.y());
#endif
	if (ztest)
		glReadPixels((int)winx, (int)winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
	else
		winz = DEFAULT_DEPTH_VALUE;


	gluUnProject(winx, winy, winz, modelview, projection, viewport, result, result + 1, result + 2);

	result[0] -= this->renderer()->globalTranslationOffset().x();
	result[1] -= this->renderer()->globalTranslationOffset().y();

	return sad::Point3D(result[0], result[1], result[2]);
}


sad::os::GLContextHandle *  sad::qt::GLContext::handle() const
{
	return NULL;
}

sad::Window * sad::qt::GLContext::window() const
{
	return m_window;
}

bool sad::qt::GLContext::isOpenGL3compatible() const
{
	return true;
}

bool sad::qt::GLContext::createFor(sad::Window * win)
{
	m_window = win;
	m_valid = true;
	return true;
}

void sad::qt::GLContext::destroy()
{
	
}

void  sad::qt::GLContext::makeCurrent()
{
	
}

sad::Renderer* sad::qt::GLContext::renderer() const
{
	if (m_window)
	{
		return m_window->renderer();
	}
	return NULL;
}
