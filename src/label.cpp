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


sad::Label::Label() :
m_point(0, 0), 
m_string(""), m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0)
{
	
}

sad::Label::Label(
	sad::Font *  font,
	const sad::Point2D  & point,
	const sad::String & string
) : 
m_point(point), 
m_string(string), m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0)
{
	m_font.attach(font);
	recomputeRenderingPoint();
}

sad::Label::Label(
	const sad::String &  font,
	const sad::Point2D  & point,
	const sad::String & string,
	const sad::String & tree
)
m_point(point), 
m_string(string), m_angle(0), 
m_size(20), m_linespacing_ratio(1.0),
m_color(0, 0, 0, 0)
{
	m_font.setTree(NULL, tree);
	m_font.setPath(font);
	recomputeRenderingPoint();
}



void sad::Label::render()
{
	sad::Font * font = m_font->get();
	if (!font)
		return;

	font->setSize(m_size);
	font->setColor(m_color);
	font->setLineSpacingRatio(m_linespacing_ratio);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((GLfloat)m_center.x(), (GLfloat)m_center.y(), 0.0f);
	glRotatef((GLfloat)(m_angle / M_PI*180.0f), 0.0f, 0.0f, 1.0f);

	
	if (font)
		font->render(m_string, m_halfpadding);

	glPopMatrix();
}

sad::Rect2D sad::Label::region() const
{
	sad::Font * font = m_font->get();
	if (!font)
		return sad::Rect2D();

	font->setSize(m_size);
	font->setLineSpacingRatio(m_linespacing_ratio);

	sad::Size2D  size = font->size(m_string);
	sad::Rect2D  result(m_point.x(), 
						m_point.y(), 
						m_point.x() + size.Width,
						m_point.y() - size.Height);
	sad::rotate(result, m_angle);
	return result;
}



sad::Label::~Label()
{

}

void sad::Label::setScene(sad::Scene * scene)
{
	this->sad::SceneNode::setScene(scene);
	if (m_font.dependsOnRenderer() != 0 && scene)
	{
		m_font.setRenderer(scene->renderer());
		reloadFont();
	}
}

void sad::Label::setPoint(const sad::Point2D & point)
{
	m_point = point;
	recomputeRenderingPoint();
}

void sad::Label::setFontName(const sad::String & name)
{
	m_font.setPath(name);
	reloadFont();
}

void sad::Label::setFont(sad::Font * font)
{
	m_font.attach(font);
	recomputeRenderingPoint();
}

void sad::Label::setFont(const sad::String & name, sad::Renderer * r)
{
	if (!r)
		r = sad::Renderer::ref();
	m_font.setPath(name);
	m_font.setRenderer(r);
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
	sad::Font * font = m_font.get();
	if (!font)
		return 0.1f;
	font->setSize(m_size);
	return font->builtinLineSpacing();
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

void sad::Label::reloadFont()
{
	sad::Font * font = font->get();
	if (font)
	{
		recomputeRenderingPoint();
	}
}


void sad::Label::recomputeRenderingPoint()
{
	sad::Font * font = font->get();
	if (!font)
		return;

	sad::Size2D size = font->size(m_string);
	m_center.setX(m_point.x() + size.Width / 2);
	m_center.setY(m_point.y() - size.Height / 2);
	m_halfpadding.setX(size.Width / -2.0);
	m_halfpadding.setY(size.Height / 2.0);
}
