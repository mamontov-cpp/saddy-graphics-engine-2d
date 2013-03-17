#include "texture.h"
#include "png/zlib.h"
#include "png/png.h"

static inline void bpp_dependent_copy(std::vector<unsigned char> & output, hst::vector<Uint8> & m_data, Uint8 m_bpp)
{
	size_t outsize=output.size();
	unsigned char * p=NULL;
	if (outsize)
		p=&(output[0]);
	int new_size=output.size();
	
	if (m_bpp==24)
		new_size=new_size/3*4;
    
	m_data.rescale(new_size);
	
	//Use memcpy when it doesn't converts
	if (m_bpp==32)
	{
		memcpy(m_data.data(),&output[0],outsize*sizeof(char));
		return;
	}
	//Convert
	Uint8 * dst=m_data.data();
    for (unsigned int i=0;i<output.size();i++)
    {
	   *(dst++)=*(p++);
	   if ((i+1 % 3) ==0 ) *(dst++)=255;
	}
}
bool sad::Texture::loadPNG(const hst::string & filename)
{
        m_data.clear();
	std::vector<unsigned char> buffer;
	png::loadFile(buffer,filename.data());
	//Handle errors
	if (buffer.size()==0) {m_data.clear();this->loadDefaultTGATexture();return false; }
	
	std::vector<unsigned char> output;
	png::decode(output,&(buffer[0]),(unsigned long)buffer.size());

        if (png::error()) 
	{
	    m_data.clear();this->loadDefaultTGATexture();
		return false;
        }
	this->m_data.clear();


	this->m_bpp=(Uint8)png::bpp(png::info());
	this->m_height=png::info().height;
	this->m_width=png::info().width;
    bpp_dependent_copy(output,m_data,m_bpp);
	m_bpp=32;
	return true;
}

bool sad::Texture::loadPNG(const hst::wstring & filename)
{
	std::vector<unsigned char> buffer;
	png::loadFile(buffer,filename.data());
	//Handle errors
	if (buffer.size()==0) 
	{m_data.clear();this->loadDefaultTGATexture();return false;}
	
	std::vector<unsigned char> output;
	png::decode(output,&(buffer[0]),(unsigned long)buffer.size());
	if (png::error())
	{m_data.clear();this->loadDefaultTGATexture();	return false; }

	this->m_data.clear();


	this->m_bpp=(Uint8)png::bpp(png::info());
	this->m_height=png::info().height;
	this->m_width=png::info().width;
	bpp_dependent_copy(output,m_data,m_bpp);
	if (m_bpp==24) m_bpp=32;

	return true;
}


sad::PNGTextureLoader::~PNGTextureLoader()
{

}

bool sad::PNGTextureLoader::load(FILE * file, sad::Texture * texture)
{
	hst::vector<Uint8> & m_data = texture->vdata();
	unsigned int & m_height = texture->height();
	unsigned int & m_width = texture->width();
	Uint8 & m_bpp = texture->bpp();

	m_data.clear();
	std::vector<unsigned char> buffer;
	fseek(file, 0L, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	buffer.resize((size_t)size);
    fread((char*)(&buffer[0]), 1, size, file);


	//Handle errors
	if (buffer.size()==0) {m_data.clear();texture->loadDefaultTGATexture();return false; }
	
	std::vector<unsigned char> output;
	png::decode(output,&(buffer[0]),(unsigned long)buffer.size());

    if (png::error()) 
	{
	    m_data.clear();texture->loadDefaultTGATexture();
		return false;
    }
	m_data.clear();


	m_bpp=(Uint8)png::bpp(png::info());
	m_height=png::info().height;
	m_width=png::info().width;
    bpp_dependent_copy(output,m_data,m_bpp);
	m_bpp=32;
	return true;
}