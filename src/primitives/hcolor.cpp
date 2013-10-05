#include "hcolor.h"

using namespace hst;

color::color()
{
	m_r=0;
	m_g=0;
	m_b=0;
}
color::color(Uint8 r, Uint8 g, Uint8 b)
{
	this->m_r=r;
	this->m_g=g;
	this->m_b=b;
}
color::color(const color & o)
{
    m_r=o.m_r;
    m_g=o.m_g;
    m_b=o.m_b;
}
color& color::operator=(const color & o)
{
    m_r=o.m_r;
    m_g=o.m_g;
    m_b=o.m_b;
	return *this;
}
color::~color()
{
}



acolor::acolor() :color()
{
	m_alpha=255;
}
acolor::acolor(Uint8 r,Uint8 g,Uint8 b,Uint8 a):color(r,g,b)
{
	m_alpha=a;
}

acolor::~acolor()
{

}
acolor::acolor(const acolor & o)
{
	setR(o.r());
	setG(o.g());
	setB(o.b());
	setA(o.a());
}
acolor & acolor::operator=(const acolor & o)
{
	setR(o.r());
	setG(o.g());
	setB(o.b());
	setA(o.a());
	return *this;
}
bool acolor::operator==(const acolor & o) const
{
	return (a()==o.a()) && (r()==o.r()) && (g()==o.g()) && (b()==o.b());
}
