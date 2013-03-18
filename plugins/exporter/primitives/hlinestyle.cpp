#include "stdafx.h"
#include "hlinestyle.h"
#include <math.h>
using namespace hst;

linestyle::linestyle( hst::linetype type,float width, const acolor & color)
{
  m_type=type;
  m_width=width;
  m_color=color;
}
linestyle::linestyle() {}
linestyle::~linestyle() {}

linestyle::linestyle(const hst::linestyle &o)
{
	m_type=o.m_type;
	m_color=o.m_color;
	m_width=o.m_width;
}

linestyle & linestyle::operator=(const hst::linestyle &o)
{
	m_type=o.m_type;
	m_color=o.m_color;
	m_width=o.m_width;
	return *this;
}

bool linestyle::operator==(const linestyle & o) const
{
	bool flag= fabs(m_width-o.width())<0.1;
	flag=flag && m_color==o.color();
	flag=flag && m_type==o.type();
	return flag;
}
