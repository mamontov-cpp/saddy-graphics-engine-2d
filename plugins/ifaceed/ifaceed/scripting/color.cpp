#include "color.h"

#include <QScriptEngine>

scripting::Color::Color()
{
    
}

scripting::Color::Color(const sad::Color& p) : m_color(p)
{
    
}

scripting::Color::~Color()
{
    
}

const sad::Color& scripting::Color::toColor() const
{
    return m_color;
}

unsigned int scripting::Color::r() const
{
    return m_color.r();
}

unsigned int scripting::Color::g() const
{
    return m_color.g();
}

unsigned int scripting::Color::b() const
{
    return m_color.b();
}

void scripting::Color::setR(unsigned int r)
{
    if (r > 255)
    {
        r = 255;
    }
    m_color.setR(r);
}

void scripting::Color::setG(unsigned int g)
{
    if (g > 255)
    {
        g = 255;
    }
    m_color.setG(g);
}

void scripting::Color::setB(unsigned int b)
{
    if (b > 255)
    {
        b = 255;
    }
    m_color.setB(b);
}

QString scripting::Color::toString() const
{
    QString result("sad::Color(%1, %2, %3)");
    return result.arg(static_cast<int>(m_color.r())).arg(static_cast<int>(m_color.g())).arg(static_cast<int>(m_color.b()));
}
