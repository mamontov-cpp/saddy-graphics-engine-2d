#include "stdafx.h"
#include "hpolyline.h"

using namespace hst;
polyline::polyline()
{
}
polyline::~polyline()
{
}
polyline::polyline(const polyline & o)
{
	m_points=o.m_points;
	m_style=o.m_style;
}
polyline & polyline::operator=(const polyline & o)
{
	m_points=o.m_points;
	m_style=o.m_style;
	return *this;
}
polyline::polyline(const hdeque<pointf> & list, const linestyle & style)
{
	m_points=list;
	m_style=style;
}

