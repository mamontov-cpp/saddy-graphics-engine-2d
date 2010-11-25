#include "stdafx.h"
#include "hshape.h"

using namespace hst;

shape::~shape()
{
}
shape::shape()
{
}
shape::shape(const shape & o)
{
	m_bbox=o.m_bbox;
	m_style=o.m_style;
}
shape::shape(const xyrect & rect,const shapestyle & style)
{
	m_bbox=rect;
	m_style=style;
}
shape & shape::operator=(const shape & o)
{
    m_bbox=o.m_bbox;
	m_style=o.m_style;
	return *this;
}

