#include "texture.h"

#ifdef LINUX
#include <renderer.h>
#endif

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
#include <stdio.h>
#include <string.h>

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

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	// Устанавливаем параметры границ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, what);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, what);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
       
        printf("Building texture (%d,%d,%d):%lu\n",m_width,m_height,m_bpp,m_data.count());
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, m_width, m_height, type, GL_UNSIGNED_BYTE, m_data.data());
	printf("gluBuild2DMipmaps ended \n");
	
}

void Texture::setAlpha(Uint8 a)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
		m_data[i]=255-a;
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
		m_data[i]=255-a;
	}
}
void Texture::setMode(Texture::Mode mode)
{
	m_mode=mode;
}

bool Texture::load(const hst::string & filename)
{
	hst::string ff(filename.getExtension());
	char * f=const_cast<char *>(ff.data());
	while(*f) { *f=toupper(*f); ++f; }

	if (ff=="BMP") return loadBMP(filename);
	if (ff=="TGA") return loadTGA(filename);
	if (ff=="PNG") return loadPNG(filename);
	return false;
}
bool Texture::load(const hst::wstring & filename)
{
	char * tmp=new char[2*filename.length()+2];
	wcstombs(tmp,filename.data(),2*filename.length()+2);
	hst::string tt(tmp);
	delete tmp;
	return load(tt);
}

void Texture::save(const char * method, const char * file)
{
	FILE * f=fopen(file,"wt");
	if (f)
	{
		fputs("#include \"texture.h\"\n\nstatic const unsigned char texdata[",f);
		fprintf(f,"%lu]=\n{\n",m_data.count());
		if (m_data.count()!=0)
			fprintf(f,"%d",m_data[0]);
		for (unsigned long i=1;i<m_data.count();i++)
               fprintf(f,", %d",m_data[i]);
		fputs("\n};\n\n",f);
		
		fprintf(f,"static const unsigned int cnt=%lu;\n\n",m_data.count());
		fprintf(f,"\nvoid sad::Texture::%s()\n{\n m_bpp=%d;m_width=%d;m_height=%d;\n for (unsigned int i=0;i<cnt;i++) m_data<<texdata[i]; \n}\n\n\n",method,m_bpp,m_width,m_height);
		fclose(f);
	}
	
}
