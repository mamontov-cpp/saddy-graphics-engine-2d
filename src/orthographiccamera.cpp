#include "orthocamera.h"
#include <GL/gl.h>

OrthoCamera::OrthoCamera(bool fix, sad::Renderer * r)
{
	m_renderer =  r;
	m_first_apply=fix;
	m_width= r->settings().width();
	m_height= r->settings().height();
	m_set_renderfix=fix;
}

OrthoCamera::OrthoCamera(int width, int height, sad::Renderer * r)
{
	m_renderer = r;
	m_first_apply=false;
	m_set_renderfix=false;
	m_width=width;
	m_height=height;
}

OrthoCamera::~OrthoCamera()
{
}


void OrthoCamera::apply()
{
	glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,m_width,0,m_height);
	glPopAttrib();

	this->Camera::apply();
}
