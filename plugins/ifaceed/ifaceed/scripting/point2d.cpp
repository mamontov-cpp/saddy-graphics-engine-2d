#include "point2d.h"

#include <QScriptEngine>

scripting::Point2D::Point2D()
{
	
}

scripting::Point2D::Point2D(const sad::Point2D& p) : m_point(p)
{
	
}

scripting::Point2D::~Point2D()
{
	
}

const sad::Point2D& scripting::Point2D::toPoint() const
{
	return m_point;
}

double scripting::Point2D::x() const
{
	return m_point.x();
}

double scripting::Point2D::y() const
{
	return m_point.y();
}

void scripting::Point2D::setX(double x)
{
	m_point.setX(x);
}

void scripting::Point2D::setY(double y)
{
	m_point.setY(y);
}

QString scripting::Point2D::toString() const
{
	QString result("sad::Point2D(%1, %2)");
	return result.arg(m_point.x()).arg(m_point.y());
}
