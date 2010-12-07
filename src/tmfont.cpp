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
					   const hst::color & bk
		         )
{
	m_tex=new sad::Texture;
	sad::TextureManager::instance()->load(tex,m_tex);
	if (!m_tex->load(tex))
	{
		sad::TextureManager::instance()->unload(tex);
		return false;
	}
	m_tex->setAlpha(255,bk,80);
	m_tex->buildMipMaps();
	//Loading mapped file
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
  float endy=rect.p().y()+rect.height();
  
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
