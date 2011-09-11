#include "stdafx.h"
#include "hline.h"


#ifdef _USE_STL
	  #include <algorithm>
      #define MIN(X,Y) std::min(X,Y)
	  #define MAX(X,Y)  std::max(X,Y)
#else
      #define MIN(X,Y) (X<Y)?X:Y
	  #define MAX(X,Y) (X>Y)?X:Y
#endif
 
h1DLine::h1DLine()                              {x1=0;       x2=0;}
h1DLine::h1DLine(float x1,float x2)             {this->x1=MIN(x1,x2);this->x2=MAX(x1,x2);}
h1DLine::h1DLine(const h1DLine & o)             {x1=o.x1;x2=o.x2;}
h1DLine & h1DLine::operator=(const h1DLine & o) {x1=o.x1;x2=o.x2;return *this;}
h1DLine::~h1DLine() {} 

void h1DLine::addLine(const h1DLine & o)
{
	float tx1=x1,tx2=x2;
	x1=MIN((MIN(tx1,tx2)),(MIN(o.x1,o.x2)));
	x2=MAX((MAX(tx1,tx2)),(MAX(o.x1,o.x2)));
}
hLine::hLine()                                      {}
hLine::hLine(const hPointF & p1,const hPointF & p2) {this->p1=p1;this->p2=p2;}
hLine::hLine(const hLine & o)                       {p1=o.p1;p2=o.p2;}
hLine & hLine::operator=(const hLine & o)           {p1=o.p1;p2=o.p2;return *this;}
hLine::~hLine() {}
struct hsline hLine::analyze() const
{
	hsline res;
	if (fabs(p2.x()-p1.x())>=0.001)
	{
       res.k1=(float)((p2.y()-p1.y())/(p2.x()-p1.x()));
	   res.k2=-1;
	}
	else
	{
	   res.k1=1;
	   res.k2=0;
	}
	res.b=(float)(res.k1*p1.x()+res.k2*p1.y());
	return res;
}
axle_t hLine::getAxle() const
{
	return atan2((float)(p2.y()-p1.y()),(float)(p2.x()-p1.x()));
}
axle_t hLine::getPerAxle() const
{
   axle_t result=getAxle();
   if (result>0) 
	   result-=(float)M_PI_2;
   else
	   result+=(float)M_PI_2;
   return result;
}
h1DLine hLine::proect(axle_t axle) const
{
   float rp1=(float)(p1.x())*cos(axle)+(float)(p1.y())*sin(axle);
   float rp2=(float)(p2.x())*cos(axle)+(float)(p2.y())*sin(axle);
   return h1DLine(rp1,rp2);
}
class h1DLine  proect(axle_t axle,const hXYRect & rect)
{
   h1DLine result=hLine(rect[0],rect[1]).proect(axle);
   result.addLine(hLine(rect[1],rect[2]).proect(axle));
   result.addLine(hLine(rect[2],rect[3]).proect(axle));
   result.addLine(hLine(rect[3],rect[0]).proect(axle));
   return result;
}
