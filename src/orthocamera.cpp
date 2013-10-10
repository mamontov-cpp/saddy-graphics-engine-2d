#include "orthocamera.h"

OrthoCamera::OrthoCamera(bool fix, sad::Renderer * r)
{
	m_renderer =  r;
	m_first_apply=true;
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

void OrthoCamera::renderFix()
{
    sad::Settings settings=m_renderer->settings();
	glViewport (0, 0, (GLsizei)(settings.width()), (GLsizei)(settings.height()));				
	glMatrixMode (GL_PROJECTION);										
	glLoadIdentity ();													
	gluPerspective (  settings.fov(), 
			          (GLfloat)(settings.width())/(GLfloat)(settings.height()), //15 - реальный размер отрисовочной части
			          settings.znear(), 
		              settings.zfar());		
    glMatrixMode (GL_MODELVIEW);										
    glLoadIdentity ();
	m_first_apply=false;
	m_set_renderfix=false;
}
void OrthoCamera::apply()
{
	if (m_first_apply && m_set_renderfix)
		renderFix();

	glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,m_width,0,m_height);
	glPopAttrib();

	this->Camera::apply();
}
