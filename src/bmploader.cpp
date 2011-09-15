#include "texture.h"
namespace BMP
{
	struct Header
	{
		unsigned short type;
		unsigned long  size;
		unsigned short reserv1;
		unsigned short reserv2;
		unsigned long  offsetbits;
	};

	struct Info
	{
		unsigned long size;
		unsigned long width;
		unsigned long height;
		unsigned short planes;
		unsigned short bitcount;
		unsigned long  compression;
		unsigned long  sizeimage;
		long           xpels;
		long           ypels;
		unsigned long  cused;
		unsigned long  cimportant;
	};
	struct Quad 
	{
	 char rgbBlue;
	 char rgbGreen;
	 char rgbRed;
	 char rgbReserved;
    };
}

#define READ(TYPE,WHAT) fread(& WHAT ,sizeof(TYPE),1,file);

static const short BITMAP_MAGIC_NUMBER=19778;

bool  sad::Texture::loadBMP(FILE * file)
{
    m_data.clear();
	BMP::Header head;
	fread(&(head.type),sizeof(head.type),1,file);
    fread(&(head.size),sizeof(head.size),1,file);
    fread(&(head.reserv1),sizeof(head.reserv1),1,file);
    fread(&(head.reserv2),sizeof(head.reserv2),1,file);
    fread(&(head.offsetbits),sizeof(head.offsetbits),1,file);

	if(head.type!=BITMAP_MAGIC_NUMBER) {m_data.clear();this->loadDefaultTGATexture();return false;}
	
	BMP::Info    info;
	
	READ(unsigned long, info.size);
	READ(unsigned long ,info.width);
	READ(unsigned long ,info.height);
	READ(unsigned short ,info.planes);
	READ(unsigned short ,info.bitcount);
	READ(unsigned long  ,info.compression);
	READ(unsigned long  ,info.sizeimage);
	READ(long           ,info.xpels);
	READ(long           ,info.ypels);
	READ(unsigned long ,info.cused);
	READ(unsigned long ,info.cimportant);
	
	if (ferror(file)) {m_data.clear();this->loadDefaultTGATexture();return false;}

	m_data.clear();
	m_width=info.width;
	m_height=info.height;
	m_bpp=info.bitcount;
	//TODO: Add support for 8-bit color
	if (m_bpp!=24 && m_bpp!=32) {m_data.clear();this->loadDefaultTGATexture(); return false;}

	unsigned long size=m_width*m_height;

	m_data.rescale(4*m_width*m_height);
	int x=0;
	int y=m_height-1;
	for (unsigned long i=0;i<size;i++)
	{
		unsigned char b=0,g=0,r=0,a=255;
		fread(&b,sizeof(char),1,file);
		fread(&g,sizeof(char),1,file);
		fread(&r,sizeof(char),1,file);
		if (m_bpp==32)
			fread(&a,sizeof(char),1,file);
		int coord=4*(y*m_width+x);
		m_data[coord]=r;
		m_data[coord+1]=g;
		m_data[coord+2]=b;
		m_data[coord+3]=a;
		++x;
		if (x==m_width)
		{
			--y;
			x=0;
		}
	}
	m_bpp=32;
	return true;
}
bool sad::Texture::loadBMP(const hst::string &filename)
{
	FILE * fl=fopen(filename.data(),"rb");
    if (fl)
	{
		bool result=loadBMP(fl);
		fclose(fl);
		return result;
	}
	m_data.clear();
	this->loadDefaultTGATexture();
	return false;
}
bool sad::Texture::loadBMP(const hst::wstring &filename)
{
	char * tmp=new char[2*filename.length()+2];
	wcstombs(tmp,filename.data(),2*filename.length()+2);
	FILE * fl=fopen(tmp,"rb");
	delete tmp;
    if (fl)
	{
		bool result=loadBMP(fl);
		fclose(fl);
		return result;
	}
	m_data.clear();
	this->loadDefaultTGATexture();
	return false;
}
