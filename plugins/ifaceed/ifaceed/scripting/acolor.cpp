#include "acolor.h"

#include <QScriptEngine>

scripting::AColor::AColor()
{
	
}

scripting::AColor::AColor(const sad::AColor& p) : m_color(p)
{
	
}

scripting::AColor::~AColor()
{
	
}

const sad::AColor& scripting::AColor::toColor() const
{
	return m_color;
}

unsigned int scripting::AColor::r() const
{
	return m_color.r();
}

unsigned int scripting::AColor::g() const
{
	return m_color.g();
}

unsigned int scripting::AColor::b() const
{
	return m_color.b();
}

unsigned int scripting::AColor::a() const
{
	return m_color.a();
}

void scripting::AColor::setR(unsigned int r)
{
	if (r > 255)
	{
		r = 255;
	}
	m_color.setR(r);
}

void scripting::AColor::setG(unsigned int g)
{
	if (g > 255)
	{
		g = 255;
	}
	m_color.setG(g);
}

void scripting::AColor::setB(unsigned int b)
{
	if (b > 255)
	{
		b = 255;
	}
	m_color.setB(b);
}

void scripting::AColor::setA(unsigned int a)
{
	if (a > 255)
	{
		a = 255;
	}
	m_color.setA(a);
}

QString scripting::AColor::toString() const
{
	QString result("sad::AColor(%1, %2, %3, %4)");
	return result
			.arg(static_cast<int>(m_color.r()))
			.arg(static_cast<int>(m_color.g()))
			.arg(static_cast<int>(m_color.b()))
			.arg(static_cast<int>(m_color.a()));
}
