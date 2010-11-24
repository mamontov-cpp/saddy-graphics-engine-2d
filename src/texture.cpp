#include "texture.h"
using namespace sad;

Texture::Texture()
{
	m_mode = BORDER_CLAMP;	/*!< Set default mode of texture.		*/
	m_filter = 10;			/*!< Set default filter of texture.		*/
}

Texture::~Texture()
{
	m_data.clear();			/*!< Cleaning the container of pixels.  */
}
void Texture::disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::enable()
{
	glBindTexture(GL_TEXTURE_2D,m_id);
}
void Texture::buildMipMaps()
{
	// Строим текстуру
	GLuint type,components;
	switch (m_bpp)
	{
		case 24: // 24 бита
			type=GL_RGB;components=3; break;
		case 32: // 32 бита
			type=GL_RGBA;components=4; break;
	};
    
	glGenTextures(1, (GLuint *)&m_id);	
	glBindTexture(GL_TEXTURE_2D, m_id);

	GLuint what;
	switch (m_mode)
	{
		case BORDER_CLAMP:	what=GL_CLAMP;break;
		case BORDER_REPEAT: what=GL_REPEAT;break;
	}
    
	// Устанавливаем параметры границ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, what);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, what);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, m_width, m_height, type, GL_UNSIGNED_BYTE, m_data.data());
	
}

void Texture::setAlpha(Uint8 a)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
		m_data[i]=a;
	}
}
void Texture::setAlpha(Uint8 a, const hst::color & clr, Uint8 prec)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
	  bool rck=abs(m_data[i-3]-clr.r())<=prec;
	  bool gck=abs(m_data[i-2]-clr.g())<=prec;
	  bool bck=abs(m_data[i-1]-clr.b())<=prec;
	  if (rck && gck && bck)
		m_data[i]=a;
	}
}
void Texture::setMode(Texture::Mode mode)
{
	m_mode=mode;
}