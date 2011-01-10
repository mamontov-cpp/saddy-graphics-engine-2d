#include "texture.h"
#include "png/zlib.h"
#include "png/png.h"

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
	for (unsigned int i=0;i<output.size();i++)
		this->m_data.add(output[i]);
	

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
	if (png::error());
	{m_data.clear();this->loadDefaultTGATexture();	return false; }

	this->m_data.clear();


	this->m_bpp=(Uint8)png::bpp(png::info());
	this->m_height=png::info().height;
	this->m_width=png::info().width;
	for (unsigned int i=0;i<output.size();i++)
		this->m_data.add(output[i]);
	

	return true;
}
