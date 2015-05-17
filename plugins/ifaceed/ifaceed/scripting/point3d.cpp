#include "point3d.h"

#include <QScriptEngine>

scripting::Point3D::Point3D()
{
	
}

scripting::Point3D::Point3D(const sad::Point3D& p) : m_point(p)
{
	
}

scripting::Point3D::~Point3D()
{
	
}

const sad::Point3D& scripting::Point3D::toPoint() const
{
	return m_point;
}

double scripting::Point3D::x() const
{
	return m_point.x();
}

double scripting::Point3D::y() const
{
	return m_point.y();
}

double scripting::Point3D::z() const
{
	return m_point.z();
}

void scripting::Point3D::setX(double x)
{
	m_point.setX(x);
}

void scripting::Point3D::setY(double y)
{
	m_point.setY(y);
}

void scripting::Point3D::setZ(double z)
{
	m_point.setZ(z);
}

QString scripting::Point3D::toString() const
{
	QString result("sad::Point3D(%1, %2, %3)");
	return result.arg(m_point.x()).arg(m_point.y()).arg(m_point.z());
}
