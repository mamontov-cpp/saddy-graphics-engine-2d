#include "point2i.h"

#include <QScriptEngine>

scripting::Point2I::Point2I()
{
    
}

scripting::Point2I::Point2I(const sad::Point2I& p) : m_point(p)
{
    
}

scripting::Point2I::~Point2I()
{
    
}

const sad::Point2I& scripting::Point2I::toPoint() const
{
    return m_point;
}

int scripting::Point2I::x() const
{
    return m_point.x();
}

int scripting::Point2I::y() const
{
    return m_point.y();
}

void scripting::Point2I::setX(int x)
{
    m_point.setX(x);
}

void scripting::Point2I::setY(int y)
{
    m_point.setY(y);
}

QString scripting::Point2I::toString() const
{
    QString result("sad::Point2I(%1, %2)");
    return result.arg(m_point.x()).arg(m_point.y());
}
