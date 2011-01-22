#include "background.h"

#ifndef LINUX

#include <windows.h>
#include <gl/gl.h>														
#include <gl/glu.h>	
#else

#include <GL/gl.h>														
#include <GL/glu.h>	

#endif


int Background::Type=2;

Background::Background(const hst::string &name)
{
	m_tex=sad::TextureManager::instance()->get(name);
	m_type=Background::Type;
}

Background::~Background()
{

}

void Background::render()
{

#define Z_AXVAL 0.5
	m_tex->enable();
    glBegin(GL_QUADS);

	
	glTexCoord2f(0.0f,0.0f);	
		glVertex3f(-0.290,-0.260,Z_AXVAL );
	glTexCoord2f(0.0f,1.0f);
		glVertex3f(-0.290,0.250,Z_AXVAL );
	glTexCoord2f(1.0f,1.0f);
		glVertex3f(0.275,0.250,Z_AXVAL );
	glTexCoord2f(1.0f,0.0f);
		glVertex3f(0.275,-0.260,Z_AXVAL );
    
	glEnd();

}