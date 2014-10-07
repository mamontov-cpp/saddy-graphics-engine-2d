#include "font.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>														
#include <GL/glu.h>

DECLARE_SOBJ_INHERITANCE(sad::Font, sad::resource::Resource)

sad::Font::Font() : m_size(14), m_linespacing_ratio(1.0)
{

}

void sad::Font::setSize(unsigned int size)
{
	m_size = size;
}

sad::Font::~Font()
{

}


void sad::Font::setColor(const sad::AColor & c)
{
	m_color = c;
}

sad::AColor sad::Font::color() const
{
	return m_color;
}


float sad::Font::builtinLineSpacing() const
{
	return 1;
}

void sad::Font::setCurrentColor()
{
	glGetIntegerv(GL_CURRENT_COLOR,this->m_current_color_buffer);
	glColor4ub(m_color.r(),m_color.g(),m_color.b(),255-m_color.a());
}

void sad::Font::restoreColor()
{
	//glPopAttrib();
	glColor4iv(m_current_color_buffer);
}