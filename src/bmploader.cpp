#include "texture.h"
#include "texturemanager.h"
namespace BMP
{
	struct Header
	{
		unsigned short type;
		unsigned int  size;
		unsigned short reserv1;
		unsigned short reserv2;
		unsigned int  offsetbits;
	};

	struct Info
	{
		unsigned int size;
		unsigned int width;
		unsigned int height;
		unsigned short planes;
		unsigned short bitcount;
		unsigned int  compression;
		unsigned int  sizeimage;
		int           xpels;
		int           ypels;
		unsigned int  cused;
		unsigned int  cimportant;
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

sad::BMPTextureLoader::~BMPTextureLoader()
{

}

bool sad::BMPTextureLoader::load(FILE * file, sad::Texture * texture)
{
	Uint8 & m_bpp = texture->bpp();
	unsigned int & m_width = texture->width();
	unsigned int & m_height = texture->height();
	hst::vector<Uint8> & m_data = texture->vdata();

	m_data.clear();
	BMP::Header head;
	fread(&(head.type),sizeof(head.type),1,file);
    fread(&(head.size),sizeof(head.size),1,file);
    fread(&(head.reserv1),sizeof(head.reserv1),1,file);
    fread(&(head.reserv2),sizeof(head.reserv2),1,file);
    fread(&(head.offsetbits),sizeof(head.offsetbits),1,file);

	if(head.type!=BITMAP_MAGIC_NUMBER) {texture->vdata().clear();texture->loadDefaultTGATexture();return false;}
	
	BMP::Info    info;
	
	READ(unsigned int, info.size);
	READ(unsigned int ,info.width);
	READ(unsigned int ,info.height);
	READ(unsigned short ,info.planes);
	READ(unsigned short ,info.bitcount);
	READ(unsigned int  ,info.compression);
	READ(unsigned int  ,info.sizeimage);
	READ(int          ,info.xpels);
	READ(int           ,info.ypels);
	READ(unsigned int ,info.cused);
	READ(unsigned int ,info.cimportant);
	
	if (ferror(file)) {texture->vdata().clear();texture->loadDefaultTGATexture();return false;}

	m_data.clear();
	texture->width()=info.width;
	texture->height()=info.height;
	texture->bpp()=(Uint8)(info.bitcount);

	

	//TODO: Add support for 8-bit color
	if (m_bpp!=24 && m_bpp!=32) {texture->vdata().clear();texture->loadDefaultTGATexture(); return false;}

	unsigned long size=m_width*m_height;
	
	texture->vdata().rescale(4*texture->width()*texture->height());
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
bool  sad::Texture::loadBMP(FILE * file)
{
	sad::TextureLoader * load = sad::TextureManager::instance()->loader("BMP");
	return load->load(file, this);
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

