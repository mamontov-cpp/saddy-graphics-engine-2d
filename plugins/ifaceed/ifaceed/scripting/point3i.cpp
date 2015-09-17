#include "point3i.h"

scripting::Point3I::Point3I()
{
    
}

scripting::Point3I::Point3I(const sad::Point3I& p) : m_point(p)
{
    
}

scripting::Point3I::~Point3I()
{
    
}

const sad::Point3I& scripting::Point3I::toPoint() const
{
    return m_point;
}

int scripting::Point3I::x() const
{
    return m_point.x();
}

int scripting::Point3I::y() const
{
    return m_point.y();
}

int scripting::Point3I::z() const
{
    return m_point.z();
}

void scripting::Point3I::setX(int x)
{
    m_point.setX(x);
}

void scripting::Point3I::setY(int y)
{
    m_point.setY(y);
}

void scripting::Point3I::setZ(int z)
{
    m_point.setZ(z);
}

QString scripting::Point3I::toString() const
{
    QString result("sad::Point3I(%1, %2, %3)");
    return result.arg(m_point.x()).arg(m_point.y()).arg(m_point.z());
}
