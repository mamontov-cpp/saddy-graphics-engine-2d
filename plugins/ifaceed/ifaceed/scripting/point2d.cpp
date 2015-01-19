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
	return result.arg(m_point.x(), m_point.y());
}

QScriptValue scripting::Point2D::qscript_call()
{
	scripting::Point2D* p = new scripting::Point2D();
	p->m_point.setX(0);
	p->m_point.setY(0);
	return engine()->newQObject(p, QScriptEngine::AutoOwnership); 
}

QScriptValue scripting::Point2D::qscript_call(double x, double y)
{
	scripting::Point2D* p = new scripting::Point2D();
	p->m_point.setX(x);
	p->m_point.setY(y);
	return engine()->newQObject(p, QScriptEngine::AutoOwnership); 
}

