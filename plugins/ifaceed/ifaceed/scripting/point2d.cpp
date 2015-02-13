#include "point2d.h"

#include <QScriptEngine>

#include <geometry2d.h>

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

QScriptValue scripting::Point2D::toValue(QScriptEngine* engine, scripting::Point2D* const &in)
{
	return engine->newQObject(in);
}

void scripting::Point2D::fromValue(const QScriptValue& object, scripting::Point2D*& out)
{
	out = qobject_cast<scripting::Point2D*>(object.toQObject());
}

scripting::Point2D*  scripting::Point2D::sum(scripting::Point2D* p)
{
	return new scripting::Point2D(this->toPoint() + p->toPoint());
}

scripting::Point2D*  scripting::Point2D::sub(scripting::Point2D* p)
{
	return new scripting::Point2D(this->toPoint() - p->toPoint());
}

scripting::Point2D* scripting::Point2D::mid(scripting::Point2D* p)
{
	return new scripting::Point2D((this->toPoint() + p->toPoint()) / 2.0);	
}

scripting::Point2D*  scripting::Point2D::mul(double d)
{
	return new scripting::Point2D(this->toPoint() * d);
}

scripting::Point2D*  scripting::Point2D::div(double d)
{
	if (sad::is_fuzzy_zero(d))
	{
		this->engine()->currentContext()->throwError("scripting::Point2D::div : number is zero");
		return new scripting::Point2D(this->toPoint());
	}
	return new scripting::Point2D(this->toPoint() / d);
}
