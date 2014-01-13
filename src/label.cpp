#include "label.h"
#include "geometry2d.h"
#include "renderer.h"
#include "fontmanager.h"

#include <cassert>

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>														
#include <GL/glu.h>

DECLARE_SOBJ_INHERITANCE(sad::Label,sad::SceneNode)


sad::Label::Label(
	sad::Font *  font,
	const sad::Point2D  & point,
	const sad::String & string
) : 
m_font(font), m_point(point), 
m_string(string), m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0)
{
	recomputeRenderingPoint();
}



void sad::Label::render()
{
	m_font->setSize(m_size);
	m_font->setColor(m_color);
	m_font->setLineSpacingRatio(m_linespacing_ratio);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_center.x(), m_center.y(), 0.0f);
	glRotatef(m_angle / M_PI*180.0f, 0.0f, 0.0f, 1.0f);

	
	if (m_font)
		m_font->render(m_string, m_halfpadding);

	glPopMatrix();
}

sad::Rect2D sad::Label::region() const
{
	if (!m_font)
		return sad::Rect2D();

	m_font->setSize(m_size);
	m_font->setLineSpacingRatio(m_linespacing_ratio);

	sad::Size2D  size = m_font->size(m_string);
	sad::Rect2D  result(m_point.x(), 
						m_point.y(), 
						m_point.x() + size.Width,
						m_point.y() - size.Height);
	sad::rotate(m_angle, result);
	return result;
}



sad::Label::~Label()
{
}


void sad::Label::setPoint(const sad::Point2D & point)
{
	m_point = point;
	recomputeRenderingPoint();
}

void sad::Label::setFont(sad::Font * font)
{
	m_font = font;
	recomputeRenderingPoint();
}

void sad::Label::setFont(const sad::String & name, sad::Renderer * r)
{
	if (!r)
		r = sad::Renderer::ref();
	setFont(r->fonts()->get(name));
}

void sad::Label::setString(const sad::String & string)
{
	m_string = string;
	recomputeRenderingPoint();
}

void sad::Label::setSize(unsigned int size)
{
	m_size = size;
	recomputeRenderingPoint();
}

float sad::Label::builtinLineSpacing() const
{
	// 0.1f is placed to avoid division by zero
	if (!m_font)
		return 0.1f;
	m_font->setSize(m_size);
	return m_font->builtinLineSpacing();
}

void sad::Label::setLineSpacing(float spacing)
{
	setLineSpacingRatio(spacing / this->builtinLineSpacing());
}

void sad::Label::setLineSpacingRatio(float ratio)
{
	m_linespacing_ratio = ratio;
	recomputeRenderingPoint();
}

void sad::Label::recomputeRenderingPoint()
{
	if (!m_font)
		return;

	sad::Size2D size = m_font->size(m_string);
	m_center.setX(m_point.x() + size.Width / 2);
	m_center.setY(m_point.y() - size.Height / 2);
	m_halfpadding.setX(size.Width / -2.0);
	m_halfpadding.setY(size.Height / 2.0);
}
