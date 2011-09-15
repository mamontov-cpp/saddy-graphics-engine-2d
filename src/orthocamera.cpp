#include "orthocamera.h"

OrthoCamera::OrthoCamera(bool fix)
{
	m_first_apply=true;
	m_width=sad::Renderer::instance().settings().width();
	m_height=sad::Renderer::instance().settings().height();
	m_set_renderfix=fix;
}

OrthoCamera::OrthoCamera(int width, int height)
{
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
    sad::Settings settings=sad::Renderer::instance().settings();
	glViewport (0, 0, (GLsizei)(settings.width()), (GLsizei)(settings.height()-15));				
	glMatrixMode (GL_PROJECTION);										
	glLoadIdentity ();													
	gluPerspective (  settings.fov(), 
			          (GLfloat)(settings.width())/(GLfloat)(settings.height()-15), //15 - реальный размер отрисовочной части
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
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,m_width,0,m_height);
    glPopAttrib();

	this->Camera::apply();
}
