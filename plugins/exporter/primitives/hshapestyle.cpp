#include "stdafx.h"
#include "hshapestyle.h"

using namespace hst;

shapestyle::shapestyle(shapetype shape, const hst::linestyle & lstyle, const acolor & fill,carvstyle carved)
{
	m_shape=shape;
	m_lstyle=lstyle;
	m_fill=fill;
	m_carved=carved;
}
shapestyle::shapestyle()
{
	m_carved=None;
	m_shape=Rect;
	m_fill=acolor(0,0,0,255);
}
shapestyle::~shapestyle()
{
}
shapestyle::shapestyle(const shapestyle & o)
{
 m_fill=o.m_fill;
 m_lstyle=o.m_lstyle;
 m_shape=o.m_shape;
 m_carved=o.m_carved;
}
shapestyle & shapestyle::operator=(const shapestyle & o)
{
  m_fill=o.m_fill;
  m_lstyle=o.m_lstyle;
  m_shape=o.m_shape;
  m_carved=o.m_carved;  
  return *this;
}

