#include "fontmanager.h"

using namespace sad;

TMFont::TMFont()
{

}

TMFont::~TMFont()
{

}

bool TMFont::load(
			           const hst::string & tex, 
					   const hst::string & cfg, 
					   const hst::color & bk,
					   bool  fontdetermine
		         )
{
	m_tex=new sad::Texture;
	sad::TextureManager::instance()->load(tex,m_tex);
	if (!m_tex->load(tex))
	{
		sad::TextureManager::instance()->unload(tex);
		return false;
	}
	if (fontdetermine)
		alphaDetermine();
	//m_tex->setAlpha(255,bk,80);
    //Loading mapping file
	FILE * fl=fopen(cfg.data(),"rt");

	if (fl)
	{
		int count=0;
		fscanf(fl,"%d\n",&count);
		if (ferror(fl)) 
		{
			sad::TextureManager::instance()->unload(tex);
			return false;
		}
		for (int i=0;i<count;i++)
		{
			char c=0; float curx=0.0f; float cury=0.0f; float endx=0.0f; float endy=0.0f;
			fscanf(fl,"%c %f %f %f %f\n",&c,&curx,&cury,&endx,&endy);
			if (ferror(fl)) return false;
			m_ul[c].setX(curx);
			m_ul[c].setY(cury);
			m_lr[c].setX(endx);
			m_lr[c].setY(endy);
		}
	}
	return true;
}
hRectF TMFont::size(const hst::string & str)
{
  float lenx=0.0f;
  float leny=0.0f;
  //Calculate total length
  for (int i=0;i<str.length();i++)
  {
	  lenx+=m_lr[str[i]].x()-m_ul[str[i]].x();
  }
  if (str.length()!=0)
	leny=m_ul[str[0]].y()-m_lr[str[0]].y();
  return hRectF(hPointF(0,0),hPointF(lenx,leny));
}
void TMFont::render(const hst::string & str,const hRectF & rect,float z)
{
  float lenx=0.0f;
  //Calculate total length
  for (int i=0;i<str.length();i++)
  {
	  lenx+=m_lr[str[i]].x()-m_ul[str[i]].x();
  }
  float aspratio=rect.width()/lenx;
  m_tex->enable();
  float curx=rect.p().x();
  float cury=rect.p().y();
  float endy=rect.p().y()+rect.height()*aspratio;
  
  if (str.length()!=0) 
  {
     float test=m_lr[str[0]].y()-m_ul[str[0]].y();
	 endy=cury+(float)(test)*aspratio;
  }
  glBegin(GL_QUADS);
  for (int i=0;i<str.length();i++)
  {
    char c=str[i];
	float len=(m_lr[c].x()-m_ul[c].x())*aspratio;
    glTexCoord2f(m_ul[c].x(),m_lr[c].y()); 
		glVertex3f(curx,cury,z);
	glTexCoord2f(m_ul[c].x(),m_ul[c].y());
		glVertex3f(curx,endy,z);
	glTexCoord2f(m_lr[c].x(),m_ul[c].y()); 
		glVertex3f(curx+len,endy,z);
	glTexCoord2f(m_lr[c].x(),m_lr[c].y());
		glVertex3f(curx+len,cury,z);
	curx+=len;
  }
  glEnd();
  
}



class Luv
{
 private:
	      float m_L;
		  float m_u;
		  float m_v;
 public:
	      Luv()               {m_L=0.0f; m_u=0.0f; m_v=0.0f;}
		  Luv(const Luv & o ) {m_L=o.m_L; m_u=o.m_u; m_v=o.m_v;}
		  /*! Converts color to Luv model
			  \return (r,g,b)->(L,u,v)
		  */
		  Luv(const hst::color & cl);
		  /*! Calculates distance
		      \param[in] lu second Luv
		  */
		  float distance(const Luv & o);
};

/*! Converts color to Luv model
	\return (r,g,b)->(L,u,v)
*/
Luv::Luv(const hst::color & cl)
{
	Uint8 r=cl.r();Uint8 g=cl.g(); Uint8 b=cl.b();
	float rs=(float)r/255.0f,gs=(float)g/255.0f,bs=(float)b/255.0f;
	float X=0.412454*rs+0.35758*gs+0.180423*bs;
	float Y=0.212671*rs+0.71516*gs+0.072169*bs;
	float Z=0.019334*rs+0.119193*gs+0.950227*bs;

	#define Yn 1.000356f
	float YY=Y/Yn;
	if (YY>0.00856f) m_L=(116*powf(YY,0.33333333f)-16);
	else             m_L=(903.3*YY);
	
	if (X<0.0001 && Y<0.0001 && Z<0.0001)
	{
		m_u=0.0f;
		m_v=0.0f;
	}
	else
	{
	 m_u=(13.0f*m_L*( (4.0f*X)/(X+15.0f*Y+3.0f*Z) )-4.0f*m_L );
	 m_v=(13.0f*m_L*( (9.0f*Y)/(X+15.0f*Y+3.0f*Z) )-9.0f*m_L );
	}
	
}
/*! Calculates color distance
	\param[in] o other Luv
	\return  distance
*/
float Luv::distance(const Luv & o)
{
  float dL=m_L-o.m_L;
  float du=m_u-o.m_u;
  float dv=m_v-o.m_v;
  return sqrtf(dL*dL+du*du+dv*dv);
}


/*! Stores a color and sorts it descendly
*/
class ColorStorage
{
 private:
	     hst::vector<hst::color>    m_x;  //!< Colors
		 hst::vector<unsigned int>  m_c;  //!< Counters
		 hst::color                 m_lasttaken; //Last taken color
 public:
	     ColorStorage();
		 void push(Uint8 r, Uint8 g, Uint8 b); //!< Adds new color and sorts
		 hst::color   take();
		 ~ColorStorage();
};

hst::color ColorStorage::take()
{
	if (m_x.count()!=0) { hst::color tmp=m_x[0]; m_x.removeAt(0); m_lasttaken=tmp; }
	return m_lasttaken;
}
void ColorStorage::push(Uint8 r, Uint8 g, Uint8 b)
{
	hst::color cl(r,g,b);
	int i=0;
	int found=false;
	for (i=0;(i<m_x.count()) && (!found);i++)
	{
		if (m_x[i]==cl)
		{
			found=true;
			++(m_c[i]);
			//Find an object to swap with
			int swapind=-1;
			for (int index=i-1;index>-1;index--)
			{
				if (m_c[i]>m_c[index]) swapind=index;
			}
			//Swap
			if (swapind!=-1) 
			{
				hst::color tmp=m_x[swapind];m_x[swapind]=m_x[i];m_x[i]=tmp;
				unsigned int t=m_c[swapind];m_c[swapind]=m_c[i];m_c[i]=t;
			}
		}
	}
	//Add if not found
	if (!found)  {m_x<<cl;m_c<<1;}
}
ColorStorage::ColorStorage() {}
ColorStorage::~ColorStorage() {}

void sad::TMFont::alphaDetermine()
{
	unsigned int cnt=m_tex->vdata().count();
	Uint8 * p=&(m_tex->vdata()[0]);
	
	Luv bk, fnt; //!< Background and font
	{
		ColorStorage cst; //Current storage
		for (int i=0;i<cnt;i+=4)  {cst.push(p[i],p[i+1],p[i+2]);}
		bk=Luv(cst.take());
		fnt=Luv(cst.take());
	}
	float a=0.0f,b=0.0f;
	float c=bk.distance(fnt);
	if (c<0.001f) c=0.001f;
    for (int i=0;i<cnt;i+=4)
	{
		Luv rb(hst::color(p[i],p[i+1],p[i+2]));
		int ax=22;
		if (p[i]==0)
		    ax=1;
		a=fnt.distance(rb);
		b=bk.distance(rb);
		float D1=(a*a+c*c-b*b)/c/c/2;
		Uint8 bb=255-(Uint8)(fabs(D1)*255);
		p[i+3]=bb;
	}
}

