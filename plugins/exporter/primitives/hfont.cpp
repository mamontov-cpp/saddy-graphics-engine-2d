#include "stdafx.h"
#include "hfont.h"

using namespace hst;

font::font()
{
	m_size=32;
	m_fontname="serif";
	m_style=Normal;
	m_color=hAColor(0,0,0,255);
}
font::~font()
{
}
font::font(int size,const hString & font,unsigned int style,const hAColor & color)
{
	m_size=size;
    m_fontname=font;
	m_style=style;
	m_color=color;
}
font::font(const font & o)
{
	m_size=o.m_size;
	m_fontname=o.m_fontname;
	m_style=o.m_style;
	m_color=o.m_color;
}
font & font::operator=(const font & o)
{
	m_size=o.m_size;
	m_fontname=o.m_fontname;
	m_style=o.m_style;
	m_color=o.m_color;
    return *this;
}



