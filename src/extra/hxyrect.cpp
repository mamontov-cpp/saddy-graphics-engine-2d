#include "stdafx.h"
#include "hxyrect.h"

using namespace hst;

xyrect::xyrect()
{
	_p=hPointF(0,0);
	_width=0;
	_height=0;
}
xyrect::xyrect(const hPointF & p1, const hPointF & p2)
{
	double minx,maxx,miny,maxy;
	if (p1.x()>p2.x()) {minx=p2.x();maxx=p1.x();} else {minx=p1.x();maxx=p2.x();}
	if (p1.y()>p2.y()) {miny=p2.y();maxy=p1.y();} else {miny=p1.y();maxy=p2.y();}
	_p=hPointF(minx,miny);
	_width=(float)(maxx-minx);
	_height=(float)(maxy-miny);
}
xyrect::xyrect(const xyrect & o)
{
	_p=o._p;
	_width=o.width();
	_height=o.height();
}
xyrect & xyrect::operator=(const xyrect & o)
{
	_p=o._p;
	_width=o.width();
	_height=o.height();
	return *this;
}

xyrect::xyrect(const hPointF & p,  double width, double height)
{
	_p=p;
	_width=width;
	_height=height;
}




xyrect xyrect::enlarged(double _w,double _h) const
{
	xyrect res(*this);
    res.moveBy(hPointF(_w/-2,_h/-2));
	res.setWidth(res.width()+_w);
	res.setHeight(res.height()+_h);
	return res;
}
xyrect xyrect::enlarged(double _a) const
{
	return enlarged(_a,_a);
}
bool xyrect::operator==(const hst::xyrect & o)
{
	return (this->_p==o._p) && (_width==o._width) && (_height==o._height);
}
