#include "../include/sprite.h"
#include <math.h>


#ifndef LINUX
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
	#include <windows.h>
	#include <gl/gl.h>														
	#include <gl/glu.h>	
#else

	#include <GL/gl.h>														
	#include <GL/glu.h>	

#endif


DECLARE_SOBJ_INHERITANCE(Sprite,sad::BasicNode)

#define DEFAULT_COLOR_INITIALIZATION  m_color = sad::AColor(255,255,255,0);

Sprite::~Sprite()
{

}

//Default texture coordinates
static float default_tex_coord[8]={0,0,0,1,1,1,1,0};
Sprite::Sprite(
			   sad::Texture * tex,
			   sad::Point3D rect[4], 
			   float * tex_coord
			  )
{
	m_tex=tex;
	DEFAULT_COLOR_INITIALIZATION;
	memcpy(m_rect,rect,4*sizeof(sad::Point3D));
    if (tex_coord)
	{
	 for (int i=0;i<4;i++)
	 {
      m_tex_coord[(3-i)<<1]=tex_coord[i<<1];
	  m_tex_coord[(3-i)<<1 | 1]=tex_coord[i<<1 | 1];
	 }
	}
	else   		    memcpy(m_tex_coord,default_tex_coord,8*sizeof(float));
}


Sprite::Sprite(
			   sad::Texture * tex,
			   sad::Point3D rect[4], 
			   int * tex_coord
			  )
{
	m_tex=tex;
	memcpy(m_rect,rect,4*sizeof(sad::Point3D));
	DEFAULT_COLOR_INITIALIZATION;
    if (tex_coord)  
	{
	 for (int i=0;i<4;i++)
	 {
      m_tex_coord[(3-i)<<1]=(float)tex_coord[i<<1]/m_tex->width();
	  m_tex_coord[(3-i)<<1 | 1]=(float)tex_coord[i<<1 | 1]/m_tex->height();
	 }
	}
	else   		    memcpy(m_tex_coord,default_tex_coord,8*sizeof(float));
}

Sprite::Sprite(
			   sad::Texture * tex, 
			   const sad::Rect<  ::sad::Point3D> & rect,
			   const sad::Rect2D  & texrect
			  )
{
	m_tex=tex;
	DEFAULT_COLOR_INITIALIZATION;
	if (m_tex != NULL)
	{
	for (int i=0;i<4;i++)
	{
	 m_rect[i]=rect[i];
	 m_tex_coord[(3-i)<<1]=(float)(texrect[i].x()/m_tex->width());
	 m_tex_coord[(3-i)<<1 | 1]=(float)(texrect[i].y()/m_tex->height());
	}
	}
}

Sprite::Sprite(const Sprite & sprite)
{
 this->m_tex=sprite.m_tex;
 DEFAULT_COLOR_INITIALIZATION;
 memcpy(&(this->m_rect),&(sprite.m_rect),4*sizeof(sad::Point3D));
 memcpy(&(this->m_tex_coord),&(sprite.m_tex_coord),8*sizeof(float));
}

void Sprite::render()
{
   GLint   clr[4]={};
   glGetIntegerv(GL_CURRENT_COLOR,clr);
   glColor4ub(m_color.r(),m_color.g(),m_color.b(),255-m_color.a());
   
   m_tex->bind();
    
   glBegin(GL_QUADS);
   for (int i=0;i<4;i++)
   {
	  int p=i<<1;
	  glTexCoord2f(m_tex_coord[ p],m_tex_coord[ p | 1 ]);
	  glVertex3f(m_rect[i].x(),m_rect[i].y(),m_rect[i].z());
   }  
   glEnd();

	glColor4iv(clr);
}

void Sprite::setTexCoords(int ind, int x ,int y)
{
  m_tex_coord[ind << 1]=(float)x/(float)m_tex->width();
  m_tex_coord[ind << 1 | 1]=(float)y/(float)m_tex->height();
}

void Sprite::setTexCoords(int ind, float x ,float y)
{
  m_tex_coord[ind << 1]=x;
  m_tex_coord[ind << 1 | 1]=y;
}

const float * Sprite::texCoords() const
{
  return m_tex_coord;
}

sad::Point3D & Sprite::point(int n)
{
 return m_rect[n];
}

sad::Point3D Sprite::middle() const
{
  return (m_rect[0]+m_rect[1]+m_rect[2]+m_rect[3])/4;
}


void Sprite::setBBox(const sad::Rect< ::sad::Point3D> & rect)
{
	for(int i = 0; i < 4; i++)
	{
		m_rect[i] = rect[i];
	}
}

void Sprite::moveBy(const sad::Point3D & p)
{
  for (int i=0;i<4;i++)
       m_rect[i]+=p;
}

void Sprite::moveTo(const sad::Point3D & p)
{
  sad::Point3D  vector=p-middle();
  moveBy(vector);
}

void Sprite::rotate(
		            sad::Point3D & p, 
					const sad::Point3D & pivot, 
					double alpha, 
					double theta 
				   )
{
  sad::Point3D dist=p-pivot;
  sad::Point3D result=dist;
  
  result.setX( (float)(dist.x()*cos(alpha)-dist.y()*sin(alpha))   );
  result.setY( (float)(dist.x()*sin(alpha)*cos(theta)+dist.y()*cos(alpha)*cos(theta)-dist.z()*sin(theta)) );
  result.setZ( (float)(dist.x()*sin(alpha)*sin(theta)+dist.y()*cos(alpha)*sin(theta)+dist.z()*cos(theta)) );
    
  p=result+pivot;
}

void Sprite::rotate(const sad::Point3D & pivot, double alpha, double theta)
{
  for (int i=0;i<4;i++)
     rotate(m_rect[i],pivot,alpha,theta);
}

void Sprite::rotate(double alpha, double theta)
{
  rotate(middle(),alpha,theta);
}


sad::Rect< ::sad::Point3D> Sprite::bbox() const
{
	sad::Rect< ::sad::Point3D> rect;
	for(int i = 0; i < 4; i++)
	{
		rect[i] = m_rect[i];
	}
	return rect;
}


void Sprite::setColor(const sad::AColor & clr) 
{
	m_color = clr;
}

const sad::AColor & Sprite::color() const
{
	return m_color;
}
