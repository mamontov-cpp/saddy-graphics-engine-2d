#include "../include/sprite.h"
#include <math.h>


#ifndef LINUX

#include <windows.h>
#include <gl/gl.h>														
#include <gl/glu.h>	
#else

#include <GL/gl.h>														
#include <GL/glu.h>	

#endif

												

Sprite::~Sprite()
{

}

//Default texture coordinates
static float default_tex_coord[8]={0,0,0,1,1,1,1,0};
Sprite::Sprite(
			   sad::Texture * tex,
			   s3d::point rect[4], 
			   float * tex_coord
			  )
{
	m_type=Sprite::Type;
	m_tex=tex;
	memcpy(m_rect,rect,4*sizeof(float));
    if (tex_coord)  memcpy(m_tex_coord,tex_coord,8*sizeof(float));
	else   		    memcpy(m_tex_coord,default_tex_coord,8*sizeof(float));
}


Sprite::Sprite(
			   sad::Texture * tex,
			   s3d::point rect[4], 
			   int * tex_coord
			  )
{
    m_type=Sprite::Type;
	m_tex=tex;
	memcpy(m_rect,rect,4*sizeof(float));
    if (tex_coord)  
	{
	 for (int i=0;i<8;i++)
	   if (i % 2 ==0)
	        m_tex_coord[i]=((float)tex_coord[i])/(float)m_tex->width();
	   else
	        m_tex_coord[i]=((float)tex_coord[i])/(float)m_tex->height();
	}
	else   		    memcpy(m_tex_coord,default_tex_coord,8*sizeof(float));
}

int Sprite::Type=1;

void Sprite::render()
{
	m_tex->enable();
    
	glBegin(GL_QUADS);
	for (int i=0;i<4;i++)
	{
	  int p=i<<1;
	  glTexCoord2f(m_tex_coord[ p],m_tex_coord[ p | 1 ]);
	  glVertex3f(m_rect[i].x(),m_rect[i].y(),m_rect[i].z());
	}  
	glEnd();
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

s3d::point & Sprite::point(int n)
{
 return m_rect[n];
}

s3d::point Sprite::middle() const
{
  return (m_rect[0]+m_rect[1]+m_rect[2]+m_rect[3])/4;
}

void Sprite::moveBy(const s3d::point & p)
{
  for (int i=0;i<4;i++)
       m_rect[i]+=p;
}

void Sprite::moveTo(const s3d::point & p)
{
  s3d::point  vector=p-middle();
  moveBy(vector);
}

void Sprite::rotate(
		            s3d::point & p, 
					const s3d::point & pivot, 
					double alpha, 
					double theta 
				   )
{
  s3d::point dist=p-pivot;
  s3d::point result=dist;
  
  result.setX( dist.x()*cos(alpha)-dist.y()*sin(alpha)   );
  result.setY( dist.x()*sin(alpha)*cos(theta)+dist.y()*cos(alpha)*cos(theta)-dist.z()*sin(theta) );
  result.setZ( dist.x()*sin(alpha)*sin(theta)+dist.y()*cos(alpha)*sin(theta)+dist.z()*cos(theta) );
    
  p=result+pivot;
}

void Sprite::rotate(const s3d::point & pivot, double alpha, double theta)
{
  for (int i=0;i<4;i++)
     rotate(m_rect[i],pivot,alpha,theta);
}

void Sprite::rotate(double alpha, double theta)
{
  rotate(middle(),alpha,theta);
}
