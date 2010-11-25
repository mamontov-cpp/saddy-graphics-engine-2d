#include "stdafx.h"
#include "pngimage.h"
#include <algorithm>

#ifdef max
		#undef max
#endif
#ifdef min
		#undef min
#endif

#define HLSMAX 240
#define RGBMAX 255
#define UNDEFINED (HLSMAX/3*2)
using namespace hst;

//! Gaps for ellipses and other
//#define VGAP 4.0
//! Gaps for ellipses and other
//#define HGAP 6.0

int pngimage::width()  const {return m_width;}
int pngimage::height() const {return m_height;}

void pngimage::setFileName(const hString & fn) {m_filename=fn;}
const hString & pngimage::fileName() const {return m_filename;}
pngimage::pngimage(const hString & name,int szx,int szy)
{
	m_filename=name;
    m_surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,szx,szy);
	//surface=cairo_svg_surface_create("svg.svg",390,60);
	m_cr =cairo_create(m_surface);
	cairo_set_source_rgb(m_cr,1.0,1.0,1.0);
	cairo_rectangle(m_cr,0,0,szx,szy);       //Draw white rectangle
	cairo_fill(m_cr);
	this->m_width=szx;
	this->m_height=szy;
}
pngimage::~pngimage()
{
  if (m_cr!=NULL)      cairo_destroy(m_cr);
  if (m_surface!=NULL) cairo_surface_destroy(m_surface);
}

void pngimage::save()
{
  if (m_cr!=NULL && m_surface!=NULL)
		cairo_surface_write_to_png(m_surface,m_filename.data());
}
pngimage::pngimage(const hString & fn)
{
  m_surface=cairo_image_surface_create_from_png(fn.data());
  m_cr =cairo_create(m_surface);
  m_width=cairo_image_surface_get_width(m_surface);
  m_height=cairo_image_surface_get_height(m_surface);
  m_filename=fn;
}

void pngimage::bezier(const hPointF & p1,const hPointF & p2,const hPointF & p3,const hPointF & p4,hPointF & q1,hPointF & q2)
{
	hPointF a1=p2-p1*(8.0f/27.0f)-p4/27.0f;
	hPointF a2=p3-p4*(8.0f/27.0f)-p1/27.0f;
    q1=a1*3.0-a2*1.5;
	q2=a2*3.0-a1*1.5;
}

hString pngimage::convert(const hString & str, const font & font)
{
   //Avoid empty markup, when passsing empty fields
   if (str.empty()) return hString("");
   //Escape some pango markup, to avoid bad result
   char * temporarystring=g_markup_escape_text(str.data(),str.length());
   //char * temporarystring=const_cast<char*>(str.data());
   // To see pango marup documentation, refer to
   // http://library.gnome.org/devel/stable/PangoMarkupFormat.html
   
   //Style string
   hString style=(font.style() & Normal)?hString(" style=\"normal\" "):  \
	             ((font.style() & Italic)?hString(" style=\"italic\" "):  \
				 hString(" style=\"oblique\" "));
   //Weight string
   hString weight=(font.style() & Bold)?hString( "weight=\"bold\" "):hString( "weight=\"normal\" ");
   //Font description string
   hString tfont=hString(" font=\"")+font.fontFamily()+hString(" ")+hString::number(font.size()) +hString("\" ");
   //Color
   int clr=font.color().r()*256*256+font.color().g()*256+font.color().b();
   char tmpclr[16];
   sprintf(tmpclr,"%06X",clr);
   hString tcolor=hString(" color=\"#")+hString(tmpclr)+ hString("\" ");
   //Resulting string
   hString result=hString("<span ")+ tfont+style+weight+tcolor+hString(">") + \
	              hString(temporarystring)+hString("</span>");
   
   //Free some temporary string
   g_free(temporarystring);
   return result;
}
void pngimage::drawLayout(const pointf & p , const hString & result)
{
  PangoLayout * layout;
  PangoFontDescription * font_description;
	
  font_description=pango_font_description_new();
  pango_font_description_set_family(font_description,"serif");
  pango_font_description_set_weight(font_description,PANGO_WEIGHT_NORMAL);
  pango_font_description_set_absolute_size(font_description,32*PANGO_SCALE);

  layout=pango_cairo_create_layout(m_cr);
  pango_layout_set_font_description(layout,font_description);
  pango_layout_set_markup(layout,result.data(),-1);

  cairo_move_to(m_cr,p.x(),p.y());
  pango_cairo_show_layout(m_cr,layout);
  
  g_object_unref(layout);
  pango_font_description_free(font_description);
}
xyrect pngimage::sizeOf(const hString & result)
{
	PangoLayout * layout;
  PangoFontDescription * font_description;
	
  font_description=pango_font_description_new();
  pango_font_description_set_family(font_description,"serif");
  pango_font_description_set_weight(font_description,PANGO_WEIGHT_NORMAL);
  pango_font_description_set_absolute_size(font_description,32*PANGO_SCALE);

  layout=pango_cairo_create_layout(m_cr);
  pango_layout_set_font_description(layout,font_description);
  pango_layout_set_markup(layout,result.data(),-1);

  int w=0,h=0;
	
  pango_layout_get_size(layout,&w,&h);
  w/=PANGO_SCALE;
  h/=PANGO_SCALE;
  
  g_object_unref(layout);
  pango_font_description_free(font_description);
  
  return xyrect(pointf(0,0),w,h);
}
void pngimage::drawText(const pointf & p,const MarkedupText & text)
{
 if (m_cr!=NULL && m_surface!=NULL && !text.empty())
 {
  hString result=pngimage::convert(text[0].p2(),text[0].p1());
  for (int i=1;i<text.count();i++)
	  result<<pngimage::convert(text[i].p2(),text[i].p1());
  drawLayout(p,result);
 }
}

xyrect pngimage::sizeOf(const MarkedupText & text)
{
  hString result=pngimage::convert(text[0].p2(),text[0].p1());
  for (int i=1;i<text.count();i++)
	  result<<pngimage::convert(text[i].p2(),text[i].p1());
  pngimage tmp("i",1,1);
  return tmp.sizeOf(result);
}

#ifdef IMAGE_WITH_STYLES
void pngimage::drawText(const pointf & p,const MarkedText & text)
{
  if (m_cr!=NULL && m_surface!=NULL && !text.empty())
  {
   hString result=pngimage::convert(text[0].p2(),istylesheet::styles()[text[0].p1()]);
   for (int i=1;i<text.count();i++)
	  result<<pngimage::convert(text[i].p2(),istylesheet::styles()[text[i].p1()]);
   drawLayout(p,result);
  }
}

xyrect pngimage::sizeOf(const MarkedText & text)
{
  hString result=pngimage::convert(text[0].p2(),istylesheet::styles()[text[0].p1()]);
   for (int i=1;i<text.count();i++)
	  result<<pngimage::convert(text[i].p2(),istylesheet::styles()[text[i].p1()]);
  pngimage tmp("i",1,1);
  return tmp.sizeOf(result);
}
#endif


bool pngimage::load(const hString & fn)
{
  if (m_cr!=NULL)      cairo_destroy(m_cr);
  if (m_surface!=NULL) cairo_surface_destroy(m_surface);

  m_surface=cairo_image_surface_create_from_png(fn.data());
  if (m_surface)
  {
  m_cr =cairo_create(m_surface);
  m_width=cairo_image_surface_get_width(m_surface);
  m_height=cairo_image_surface_get_height(m_surface);
  m_filename=fn;
  }
  return m_surface!=NULL;
}

pngimage::pngimage(const hString & name,cairo_surface_t * surf,cairo_t * cr, int w ,int h)
{
	m_surface=surf;
	m_cr=cr;
	m_width=w;
	m_height=h;
	m_filename=name;
}
pngimage  pngimage::mergeHorizontal(const hString & to,pngimage & img1, pngimage & img2)
{
  int szx=img1.width()+img2.width();
  int szy=(img1.height()>img2.height())?img1.height():img2.height();
  cairo_surface_t * _surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,szx,szy);
  cairo_t         * _cr =cairo_create(_surface);
  cairo_set_source_rgba(_cr,1.0,1.0,1.0,1.0);
  cairo_rectangle(_cr,0,0,szx,szy);       //Draw white rectangle
  cairo_fill(_cr);
  
  //Draw first image
  cairo_set_source_surface(_cr,img1.m_surface,0,((double)szy-(double)img1.height())/2);
  cairo_rectangle(_cr,0,((double)szy-(double)img1.height())/2,img1.width(),img1.height());
  cairo_fill(_cr);
  
  //Draw second image
  cairo_set_source_surface(_cr,img2.m_surface,img1.width(),((double)szy-(double)img2.height())/2);
  cairo_rectangle(_cr,img1.width(),((double)szy-(double)img2.height())/2,img2.width(),img2.height());
  cairo_fill(_cr);
  
  //Create new image
  return pngimage(to,_surface,_cr,szx,szy);
}

pngimage  pngimage::mergeVertical(const hString & to,pngimage & img1, pngimage & img2)
{
  int szx=(img1.width()>img2.width())?img1.width():img2.width();
  int szy=img1.height()+img2.height();
  cairo_surface_t * _surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,szx,szy);
  cairo_t         * _cr =cairo_create(_surface);
  cairo_set_source_rgba(_cr,1.0,1.0,1.0,1.0);
  cairo_rectangle(_cr,0,0,szx,szy);       //Draw white rectangle
  cairo_fill(_cr);
  
  //Draw first image
  cairo_set_source_surface(_cr,img1.m_surface,((double)szx-(double)img1.width())/2,0);
  cairo_rectangle(_cr,((double)szx-(double)img1.width())/2,0,img1.width(),img1.height());
  cairo_fill(_cr);
  
  //Draw second image
  cairo_set_source_surface(_cr,img2.m_surface,((double)szx-(double)img2.width())/2,img1.height());
  cairo_rectangle(_cr,((double)szx-(double)img2.width())/2,img1.height(),img2.width(),img2.height());
  cairo_fill(_cr);
   
  //Create new image
  return pngimage(to,_surface,_cr,szx,szy);
}
void pngimage::setLineStyle(const linestyle & line)
{
   double dashes[2]={ 8.0,8.0 };
   if (line.type()==Dashed)
   {
   	   cairo_set_dash(m_cr,dashes,1,1.0);
   }
   else
   {
	   cairo_set_dash(m_cr,dashes,0,1.0);
   }
   cairo_set_line_width(m_cr,line.width());
   setColor(line.color());
}
void pngimage::setColor(const acolor & clr)
{
	cairo_set_source_rgba(m_cr,(double)clr.r()/255,(double)clr.g()/255,(double)clr.b()/255,1.0-(double)clr.a()/255);
}
void pngimage::stroke()
{
	cairo_stroke(m_cr);
}
void pngimage::drawArrowLines(const pointf & plast, const pointf & last, double arrow_len,double arrow_angle )
{
 double dx=last.x()-plast.x();
 double dy=last.y()-plast.y();
 double a0;                 //Angle of  xy axis
 if (abs(dx)<0.001)         //Safe atan
 {
  a0=(dy<0)?-1.57:1.57;
 }
 else
 {
   a0=atan(dy/dx);
   if (dx<0) a0+=3.14;
  }
  //Draw arrow 1st line
  cairo_move_to(m_cr,last.x(),last.y());
  cairo_line_to(m_cr,last.x()-arrow_len*cos(a0-arrow_angle),last.y()-arrow_len*sin(a0-arrow_angle));
  //Draw arrow 2nd line
  cairo_move_to(m_cr,last.x(),last.y());
  cairo_line_to(m_cr,last.x()-arrow_len*cos(a0+arrow_angle),last.y()-arrow_len*sin(a0+arrow_angle));
  
  cairo_stroke(m_cr);
}
void pngimage::drawLine(const polyline & line,double arrow_len,double arrow_angle)
{
#define LPOINTS line.points() 
  
  if (m_cr!=NULL && m_surface!=NULL && (line.points().count()>1) )
  {
	  setLineStyle(line.style());
	  

	  cairo_new_path(m_cr);

		  
	  //Drawing lines
	  cairo_move_to(m_cr,LPOINTS[0].x(),LPOINTS[0].y());
	  for (long i=1;i<LPOINTS.count();i++)
	  {
	    pointf tmp=LPOINTS[i];
		cairo_line_to(m_cr,tmp.x(),tmp.y());	
	  }
	  stroke();

	  //if (line.style().type()==Dashed)
	  //{
	  //	  drawArrowLines(LPOINTS[LPOINTS.count()-2],LPOINTS[LPOINTS.count()-1],arrow_len,arrow_angle);
	  //}
	  if (line.style().type()==Arrow ) 
	  {
		  drawArrowLines(LPOINTS[LPOINTS.count()-2],LPOINTS[LPOINTS.count()-1],arrow_len,arrow_angle);
	  }
	  if (line.style().type()==Anchor)
	  {
          shapestyle shpstyle(Ellipse,linestyle(Simple,1.0,line.style().color()),line.style().color(),None);
		  //Shift point
		  pointf shift=pointf(ANCHOR_RAD*line.style().width(),ANCHOR_RAD*line.style().width());
		  drawShape(shape(xyrect(
			                      pointf( LPOINTS[LPOINTS.count()-1]-shift) ,
								  pointf( LPOINTS[LPOINTS.count()-1]+shift) 
								 ),shpstyle));
		  drawShape(shape(xyrect(
			                      pointf( LPOINTS[0]-shift) ,
								  pointf( LPOINTS[0]+shift) 
								 ),shpstyle));
	  }
      cairo_close_path(m_cr);
#undef LPOINTS
  
  }
}

void pngimage::shift(xyrect & rect )
{
 if (rect.p().y()<0.1) rect.p().setY(rect.p().y()+1);
 if (rect.p().x()<0.1) rect.p().setX(rect.p().x()+1);
 
 double dwidth=(float)(rect.p().x())+rect.width()-m_width; 
 double dheight=(float)(rect.p().y())+rect.height()-m_height;
 //Decreasing sizes, if rect gets outta image, a little
 if (dwidth>-1 && dwidth<4) 
		  rect.setWidth(rect.width()-dwidth-1.0f); 
 if (dheight>-1 && dheight<4) 
		  rect.setHeight(rect.height()-dheight-1.0f); 
}
void pngimage::addArcToContext(const hst::xyrect & rect,double ba,double ea,bool move)
{
  if (move)
  {
   pointf center(rect.p().x()+rect.width()/2,rect.p().y()+rect.height()/2);
   center+=pointf(rect.width()/2*cos(ba),rect.height()/2*sin(ba));
   cairo_move_to(m_cr,center.x(),center.y());
  }
  cairo_save(m_cr);
  cairo_translate(m_cr,rect.p().x()+rect.width()/2.,rect.p().y()+rect.height()/2.);
  cairo_scale(m_cr,rect.width()/2.,rect.height()/2.);
  cairo_arc(m_cr,0.,0.,1.,ba,ea);
  cairo_restore(m_cr);
}
void pngimage::addNegativeArcToContext(const hst::xyrect & rect,double ba,double ea,bool move)
{
  if (move)
  {
   pointf center(rect.p().x()+rect.width()/2,rect.p().y()+rect.height()/2);
   center+=pointf(rect.width()/2*cos(ba),rect.height()/2*sin(ba));
   cairo_move_to(m_cr,center.x(),center.y());
  }
  cairo_save(m_cr);
  cairo_translate(m_cr,rect.p().x()+rect.width()/2.,rect.p().y()+rect.height()/2.);
  cairo_scale(m_cr,rect.width()/2.,rect.height()/2.);
  cairo_arc_negative(m_cr,0.,0.,1.,ba,ea);
  cairo_restore(m_cr);
}
double pngimage::calcFieldEllipse(const hst::xyrect & rect,double field)
{
  if (field>=rect.width()/2 || field<0) return 0.0;
  double x=rect.width()/2-field;
  double sina=sqrt(1- 4*x*x/rect.width()/rect.width());
  double y=rect.height()/2*sqrt(1- 4*x*x/rect.width()/rect.width());
  //rect.width()-field==rect.width*cos(alpha)
  double alpha=asin( sina ) ;                
  return alpha;
}
void pngimage::drawLeftEllipseField(const hst::xyrect & rect,double alpha)
{
  cairo_new_path(m_cr);
  //Left field
  addArcToContext(rect,M_PI-alpha,M_PI+alpha);
  
  cairo_close_path(m_cr);
  cairo_fill(m_cr);
}
void pngimage::drawRightEllipseField(const hst::xyrect & rect,double alpha)
{
  cairo_new_path(m_cr);
  //Right field
  addArcToContext(rect,-1*alpha,alpha);

  cairo_close_path(m_cr);
  cairo_fill(m_cr);
}
void pngimage::drawEllipseField(const xyrect & rect,double field)
{ 
 double alpha=calcFieldEllipse(rect,field);
 drawLeftEllipseField(rect,alpha);
 drawRightEllipseField(rect,alpha); 
}
void pngimage::drawEllipse(const xyrect & rect)
{
  addArcToContext(rect,0.0,2*M_PI);	 
}
acolor RGBtoHLS(const acolor & clr)
{
   acolor result;
   int cmax,cmin;
   int R=clr.r(),G=clr.g(),B=clr.b();
   int H,L,S;
   cmax=std::max( std::max(R,G),B);
   cmin=std::min( std::min(R,G),B);
   //Lightness
   L=((unsigned int)((cmax+cmin)*HLSMAX+RGBMAX)/(unsigned int)(2*RGBMAX)) ;
   if (cmax==cmin)  //Achromatic case
   {
     S=0;  //Saturation
	 H=(UNDEFINED); //hue
   }
   else
   {
	   int Rdelta,Gdelta,Bdelta; 
	   //Saturation
	   if (L<(HLSMAX/2))
		    S=( ((cmax-cmin)*HLSMAX)+((cmax+cmin)/2) )/(cmax+cmin);
	   else
		    S=( ((cmax-cmin)*HLSMAX)+((2*RGBMAX-cmax-cmin)/2))/(2*RGBMAX-cmax-cmin);
	   //Hue
	   Rdelta=( ((cmax-R)*(HLSMAX/6))+((cmax-cmin)/2))/(cmax-cmin);
	   Gdelta=( ((cmax-G)*(HLSMAX/6))+((cmax-cmin)/2))/(cmax-cmin);
	   Bdelta=( ((cmax-B)*(HLSMAX/6))+((cmax-cmin)/2))/(cmax-cmin);
	   if (R==cmax)
		   H=(Bdelta-Gdelta);
	   else
	   {
	     if (G==cmax)
		   H=(HLSMAX/3+Rdelta-Bdelta);
		 else
		   H=(2*HLSMAX/3+Rdelta-Bdelta);
	   }
	   if (H<0) H+=HLSMAX; if (H>HLSMAX) H-=HLSMAX;
       
   }
   result.setR((Uint8)H);
   result.setG((Uint8)L);
   result.setB((Uint8)S);
   return result;
}
int HueToRGB(int n1,int n2,int hue)
{
	if (hue<0) hue+=HLSMAX; if (hue>HLSMAX) hue-=HLSMAX;

	if (hue< (HLSMAX/6))
		return (n1+(((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)));
	if (hue< (HLSMAX/2))
		return n2;
	if (hue< ((HLSMAX*2)/3))
		return (n1+(((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6)));
	return n1;
}
acolor HLSToRGB(const acolor & hls)
{
	acolor result;
	int R,G,B;
	int hue=hls.r();int lum=hls.g();int sat=hls.b();
	int Magic1,Magic2;
	if (sat==0)
	{
		R=G=B=(lum*RGBMAX)/HLSMAX;
	}
	else
	{
		if (lum<=(HLSMAX/2))
			Magic2=(lum*(HLSMAX+sat)+(HLSMAX/2))/HLSMAX;
		else
			Magic2=lum+sat- ((lum*sat)+(HLSMAX/2))/HLSMAX;
		Magic1=2*lum-Magic2;

		R=(HueToRGB(Magic1,Magic2,hue+(HLSMAX/3))*RGBMAX+(HLSMAX/2))/HLSMAX;
		G=(HueToRGB(Magic1,Magic2,hue)*RGBMAX+(HLSMAX/2))/HLSMAX;
		B=(HueToRGB(Magic1,Magic2,hue-(HLSMAX/3))*RGBMAX+(HLSMAX/2))/HLSMAX;
	}

	result.setR((Uint8)R);
	result.setG((Uint8)G);
	result.setB((Uint8)B);
	return result;
}
acolor pngimage::increaseColor(const acolor & clr)
{
   //Used from http://support.microsoft.com/kb/29240
   acolor hls=RGBtoHLS(clr);
   hls.setG(hls.g()+25);
   acolor res=HLSToRGB(hls);
   res.setA(clr.a());
   return res;
}
acolor pngimage::decreaseColor(const acolor & clr)
{
   //Used from http://support.microsoft.com/kb/29240
   acolor hls=RGBtoHLS(clr);
   hls.setG(hls.g()-25);
   acolor res=HLSToRGB(hls);
   res.setA(clr.a());
   return res;
}
void pngimage::concave(const xyrect & rect, const acolor & clr)
{
  acolor light=increaseColor(clr);
  acolor dark=decreaseColor(clr);
  drawLine(polyline(hdeque<pointf>()<<pointf(rect[1]-pointf(2,0))<<rect.p()<<pointf(rect[3]-pointf(0,2)),
	       linestyle(Simple,1.0,light)));
  drawLine(polyline(hdeque<pointf>()<<pointf(rect[1]+pointf(0,2))<<rect[2]<<pointf(rect[3]+pointf(2,0)),
	       linestyle(Simple,1.0,dark)));
}
void pngimage::convex(const xyrect & rect, const acolor & clr)
{
  acolor light=increaseColor(clr);
  acolor dark=decreaseColor(clr);
  drawLine(polyline(hdeque<pointf>()<<pointf(rect[1]-pointf(2,0))<<rect.p()<<pointf(rect[3]-pointf(0,2)),
	       linestyle(Simple,1.0,dark)));
  drawLine(polyline(hdeque<pointf>()<<pointf(rect[1]+pointf(0,2))<<rect[2]<<pointf(rect[3]+pointf(2,0)),
	       linestyle(Simple,1.0,light)));
}
void pngimage::drawRect(const xyrect & rect)
{
 cairo_rectangle(m_cr,rect.p().x(),rect.p().y(),rect.width(),rect.height());
}
void drawRoundRectFields(cairo_t * m_cr,const xyrect & rect,double field)
{
 double radius=rect.height()/8.0/rect.width()*rect.height();
 double degrees= M_PI / 180.0;

 cairo_new_path(m_cr);

 cairo_arc(m_cr,rect.p().x()+rect.width()-radius,rect.p().y()+radius,radius,-90*degrees,0*degrees);
 cairo_arc(m_cr,rect.p().x()+rect.width()-radius,rect.p().y()+rect.height()-radius,radius,0*degrees,90*degrees);
 
 cairo_line_to(m_cr,rect.p().x()+rect.width()-field,rect.p().y()+rect.height());
 cairo_line_to(m_cr,rect.p().x()+rect.width()-field,rect.p().y());

 cairo_close_path(m_cr);
 cairo_fill(m_cr);

 cairo_new_path(m_cr);

 cairo_arc(m_cr,rect.p().x()+radius,rect.p().y()+rect.height()-radius,radius,90*degrees,180*degrees);
 cairo_arc(m_cr,rect.p().x()+radius,rect.p().y()+radius,radius,180*degrees,270*degrees);

 cairo_line_to(m_cr,rect.p().x()+field,rect.p().y());
 cairo_line_to(m_cr,rect.p().x()+field,rect.p().y()+rect.height());

 cairo_close_path(m_cr);
 cairo_fill(m_cr);

}
void pngimage::drawRoundRect(const xyrect & rect)
{
 cairo_new_path(m_cr);

 double radius=rect.height()/8.0/rect.width()*rect.height();
 double degrees= M_PI / 180.0;

 cairo_arc(m_cr,rect.p().x()+rect.width()-radius,rect.p().y()+radius,radius,-90*degrees,0*degrees);
 cairo_arc(m_cr,rect.p().x()+rect.width()-radius,rect.p().y()+rect.height()-radius,radius,0*degrees,90*degrees);
 cairo_arc(m_cr,rect.p().x()+radius,rect.p().y()+rect.height()-radius,radius,90*degrees,180*degrees);
 cairo_arc(m_cr,rect.p().x()+radius,rect.p().y()+radius,radius,180*degrees,270*degrees);

 cairo_close_path(m_cr);
}
void pngimage::drawBezier(const pointf & p1,const pointf & p2, const pointf & p3, const pointf & p4)
{
	pointf q1,q2;
    bezier( p1,p2,p3,p4,q1,q2);
	//cairo_move_to(m_cr,p1.x(),p1.y());
	cairo_curve_to(m_cr,q1.x(),q1.y(),q2.x(),q2.y(),p4.x(),p4.y());
}
void pngimage::drawBarrelFields(const xyrect & rect,double field)
{
  double len=rect.width()/11;
  hst::xyrect frright(pointf(rect.p().x()+rect.width()-2*len,rect.p().y()),2*len,rect.height());
  hst::xyrect frleft(pointf(rect.p().x(),rect.p().y()),2*len,rect.height());
  hst::xyrect frmain(rect.p()+pointf(len,0),rect.width()-2*len,rect.height());
  if (field>len)
  {
	  drawLeftEllipseField(frleft,M_PI/2);
	  drawRightEllipseField(frright,M_PI/2);
	  drawRectFields(frmain,field-len);
  }
  else
  {
	  double alpha=calcFieldEllipse(frright,field);
	  drawLeftEllipseField(frleft,alpha);
	  drawRightEllipseField(frright,alpha);
  }

  /*
  hPointF p=rect.p(),sz=pointf(rect.width(),rect.height());

  cairo_new_path(m_cr);
  cairo_move_to(m_cr,p.x()+sz.x()-len,p.y());
  drawBezier(
	         hPointF(p.x()+sz.x()-len,p.y()),
		     hPointF(p.x()+sz.x(),p.y()+sz.y()/4),
             hPointF(p.x()+sz.x(),p.y()+sz.y()/4*3),
		     hPointF(p.x()+sz.x()-len,p.y()+sz.y())
			);
  if (field>len)
  {
  cairo_line_to(m_cr,p.x()+sz.x()-field,p.y()+sz.y());
  cairo_line_to(m_cr,p.x()+sz.x()-field,p.y());
  cairo_line_to(m_cr,p.x()+sz.x()-len,p.y());
  } else cairo_line_to(m_cr,p.x()+sz.x()-len,p.y());
  cairo_close_path(m_cr);
  cairo_fill(m_cr);


  cairo_new_path(m_cr);
  cairo_move_to(m_cr,p.x()+len,p.y()+sz.y());
  drawBezier(
	           hPointF(p.x()+len,p.y()+sz.y()),
               hPointF(p.x(),p.y()+sz.y()/4*3),
	           hPointF(p.x(),p.y()+sz.y()/4),
		       hPointF(p.x()+len,p.y())
			 );
  if (field>len)
  {
  cairo_line_to(m_cr,p.x()+field,p.y());
  cairo_line_to(m_cr,p.x()+field,p.y()+sz.y());
  cairo_line_to(m_cr,p.x()+len,p.y()+sz.y());
  } else cairo_line_to(m_cr,p.x()+len,p.y());
   
  cairo_close_path(m_cr);
  cairo_fill(m_cr);
  */
}
void pngimage::drawBarrel(const xyrect & rect)
{
  double len=rect.width()/11;
  hPointF p=rect.p(),sz=pointf(rect.width(),rect.height());
 
  cairo_new_path(m_cr); 
  cairo_move_to(m_cr,p.x()+len,p.y());
  cairo_line_to(m_cr,p.x()+sz.x()-len,p.y());
  addArcToContext(hst::xyrect(pointf(p.x()+sz.x()-2*len,p.y()),2*len,sz.y()),-1*M_PI/2,M_PI/(2),false);
  cairo_line_to(m_cr,p.x()+len,p.y()+sz.y());
  
  addArcToContext(hst::xyrect(pointf(p.x(),p.y()),2*len,sz.y()),-3*M_PI/2,M_PI/(-2),false);
  cairo_close_path(m_cr);
}
void pngimage::drawRectFields(const hst::xyrect & rect,double field)
{
  cairo_move_to(m_cr,rect.p().x(),rect.p().y());
  cairo_line_to(m_cr,rect.p().x()+field,rect.p().y());
  cairo_line_to(m_cr,rect.p().x()+field,rect.p().y()+rect.height());
  cairo_line_to(m_cr,rect.p().x(),rect.p().y()+rect.height());
  cairo_line_to(m_cr,rect.p().x(),rect.p().y());
  cairo_fill(m_cr);

  cairo_move_to(m_cr,rect.p().x()+rect.width(),rect.p().y());
  cairo_line_to(m_cr,rect.p().x()+rect.width()-field,rect.p().y());
  cairo_line_to(m_cr,rect.p().x()+rect.width()-field,rect.p().y()+rect.height());
  cairo_line_to(m_cr,rect.p().x()+rect.width(),rect.p().y()+rect.height());
  cairo_line_to(m_cr,rect.p().x()+rect.width(),rect.p().y());
  cairo_fill(m_cr);
  
}
void pngimage::drawShape(const shape & shp,double field)
{
 if (m_cr==NULL || m_surface==NULL ) return;
  //Temporary shift rect
  xyrect tmprect=shp.box();
  shift(tmprect);

  //Set color style
  setColor(shp.style().fill());

  //Type dependend drawing action
  if (shp.style().type()==RoundRect) 
  {
	  drawRoundRect(tmprect);
	  //Apply changes to context
      cairo_fill_preserve(m_cr);
      setLineStyle(shp.style().linestyle());
      cairo_stroke(m_cr);
	  if (field>0.1)
		drawRoundRectFields(m_cr,tmprect,field);
  }
  if (shp.style().type()==Barrel)  
  {
	  drawBarrel(tmprect);
	  //Apply changes to context
      cairo_fill_preserve(m_cr);
      setLineStyle(shp.style().linestyle());
      cairo_stroke(m_cr);
	  if (field>0.01)
	  {
         drawBarrelFields(tmprect,field);
	  }
  }
  if (shp.style().type()==Rect)    
  {
	  drawRect(tmprect);
	  //Apply changes to context
      cairo_fill_preserve(m_cr);
      setLineStyle(shp.style().linestyle());
      cairo_stroke(m_cr);
	  if (field>0.1)
		  drawRectFields(tmprect,field);
  }
  if (shp.style().type()==Ellipse) 
  {
	  drawEllipse(tmprect);
	  //Apply changes to context
      cairo_fill_preserve(m_cr);
      setLineStyle(shp.style().linestyle());
      cairo_stroke(m_cr);
	  if (field>0.1)
	  	drawEllipseField(tmprect,field);
  }
  
  
  if (shp.style().type()==Rect)
   {
	   if (shp.style().carving()==Concave)
		 concave(tmprect,shp.style().fill());
       if (shp.style().carving()==Convex)
		 convex(tmprect,shp.style().fill());
   }
}
void pngimage::drawArc( const hst::xyrect & rect,double an1,double an2,const linestyle & lstyle)
{
  setLineStyle(lstyle); 
  addArcToContext(rect,an1,an2);
  cairo_stroke(m_cr);
  double b=rect.width()/2,a=rect.height()/2;
  pointf center(rect.p().x()+b,rect.p().y()+a);
  if (lstyle.type()==Arrow ) 
  {
	 pointf last=center+pointf(b*cos(an2),a*sin(an2));
	 pointf prev=center+pointf(b*cos(an2-0.01),a*sin(an2-0.01));
	 drawArrowLines(prev,last,ARROW_LEN,ARROW_ANGLE);
  }
  if (lstyle.type()==Anchor)
  {
    pointf last=center+pointf(b*cos(an2),a*sin(an2));
	pointf first=center+pointf(b*cos(an1),a*sin(an1));
	 
    shapestyle shpstyle(Ellipse,linestyle(Simple,1.0,lstyle.color()),lstyle.color(),None);
    //Shift point
	pointf shift=pointf(ANCHOR_RAD*lstyle.width(),ANCHOR_RAD*lstyle.width());
	drawShape(shape(xyrect(
			                      pointf( last-shift) ,
								  pointf( last+shift) 
						   ),shpstyle));
	drawShape(shape(xyrect(
			                      pointf( first-shift) ,
								  pointf( first+shift) 
								 ),shpstyle));
	                       }
}
