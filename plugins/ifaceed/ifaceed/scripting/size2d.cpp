#include "size2d.h"

#include <QScriptEngine>

scripting::Size2D::Size2D()
{
	
}

scripting::Size2D::Size2D(const sad::Size2D& p) : m_size(p)
{
	
}

scripting::Size2D::~Size2D()
{
	
}

const sad::Size2D& scripting::Size2D::toSize() const
{
	return m_size;
}

double scripting::Size2D::width() const
{
	return m_size.Width;
}

double scripting::Size2D::height() const
{
	return m_size.Height;
}

void scripting::Size2D::setWidth(double width)
{
	m_size.Width = width;
}

void scripting::Size2D::setHeight(double height)
{
	m_size.Height = height;
}

QString scripting::Size2D::toString() const
{
	QString result("sad::Size2D(%1, %2)");
	return result.arg(m_size.Width).arg(m_size.Height);
}
