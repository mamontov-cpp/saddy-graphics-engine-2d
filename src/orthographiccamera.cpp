#include "orthographiccamera.h"
#include "renderer.h"
#include "log/log.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>														
#include <GL/glu.h>

sad::OrthographicCamera::OrthographicCamera(sad::Renderer * r)
{
	if (r == NULL)
	{
		r = sad::Renderer::ref();
	}
	m_width= r->settings().width();
	m_height= r->settings().height();
}

sad::OrthographicCamera::OrthographicCamera(int width, int height)
: m_width(width), m_height(height)
{
}



void sad::OrthographicCamera::apply()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,m_width,0,m_height);
	glPopAttrib();

	this->sad::Camera::apply();
}

sad::OrthographicCamera::~OrthographicCamera()
{
}
