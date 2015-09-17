#include "size2i.h"

#include <QScriptEngine>

scripting::Size2I::Size2I()
{
    
}

scripting::Size2I::Size2I(const sad::Size2I& p) : m_size(p)
{
    
}

scripting::Size2I::~Size2I()
{
    
}

const sad::Size2I& scripting::Size2I::toSize() const
{
    return m_size;
}

unsigned int scripting::Size2I::width() const
{
    return m_size.Width;
}

unsigned int scripting::Size2I::height() const
{
    return m_size.Height;
}

void scripting::Size2I::setWidth(unsigned int width)
{
    m_size.Width = width;
}

void scripting::Size2I::setHeight(unsigned int height)
{
    m_size.Height = height;
}

QString scripting::Size2I::toString() const
{
    QString result("sad::Size2I(%1, %2)");
    return result.arg(m_size.Width).arg(m_size.Height);
}

