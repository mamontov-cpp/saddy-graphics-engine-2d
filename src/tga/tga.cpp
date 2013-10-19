#include "tga/tga.h"
#include <algorithm>

using namespace tga;

void  sad::Texture::reverseTGA(const tga::Info & textureInfo)
{
	// If we don't need to flip anything
	if (textureInfo.m_vertflip == false && textureInfo.m_horzflip == false)
		return;
	unsigned int rowsize = textureInfo.m_TGA_width * 4; // 4 is four bytes per row
	unsigned int halfpixelwidth = textureInfo.m_TGA_width / 2;
	unsigned int halfwidth =  halfpixelwidth * 4;
	unsigned int halfheight = textureInfo.m_TGA_height / 2;

	sad::uchar * flipbuffer = new sad::uchar[rowsize];
	sad::uchar * begin = &(m_data[0]);
	if (textureInfo.m_horzflip && (m_height % 2) == 1)
	{
		unsigned int offset = halfheight * rowsize;
		sad::uchar * p1 = begin + offset;
		sad::uchar * p2 = begin + (offset + rowsize - halfwidth);
		// Flip horizontally
		memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
		memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
		memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));
	}

	for(unsigned int row = 0; row < halfheight; row++)
	{
		unsigned int offset1 = row * rowsize;
		unsigned int offset2 =(textureInfo.m_TGA_height - 1 - row) * rowsize;
		sad::uchar * begin1 = begin + offset1;
		sad::uchar * begin2 = begin + offset2;
		if (textureInfo.m_horzflip)
		{
			// Flip horizontally first row
			sad::uchar * p1 = begin1;
			sad::uchar * p2 = begin1 + (rowsize - halfwidth);
			memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
			memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
			memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));

			// Flip horizontally second row			
			p1 = begin2;
			p2 = begin2 + (rowsize - halfwidth);
			memcpy(flipbuffer, p1, halfwidth * sizeof(sad::uchar));
			memcpy(p1, p2, halfwidth * sizeof(sad::uchar));
			memcpy(p2, flipbuffer, halfwidth * sizeof(sad::uchar));
		}
		if (textureInfo.m_vertflip == false)
		{
			memcpy(flipbuffer, begin1, rowsize * sizeof(sad::uchar));
			memcpy(begin1, begin2, rowsize * sizeof(sad::uchar));
			memcpy(begin2, flipbuffer, rowsize * sizeof(sad::uchar));
		}
	}
	delete flipbuffer;
}

// Loading TGA texture.
bool sad::Texture::loadTGA(const sad::String & filename)
{
	bool result = false;	// Result of this function's work
	FILE* hFile;			// File descriptor
	hFile = fopen(filename.data(),"rb");					// Open file by reading bytes
	return this->loadTGA(hFile);
}

// Loading TGA texture with wide name.
bool sad::Texture::loadTGA(const sad::WString & filename)
{
	bool result = false;	// Result of this function's work
	FILE* hFile;			// File descriptor

#ifdef WIN32
#ifndef MINGW32
	hFile = _wfopen(filename.data(), L"rb");	// Open file by reading bytes
#else
	char * newfname=new char[2*filename.length()+2];
	wcstombs(newfname,filename.data(),2*filename.length()+2);
	hFile=fopen(newfname,"rb");
	delete newfname;
#endif
#else
	char * newfname=new char[2*filename.length()+2];
	wcstombs(newfname,filename.data(),2*filename.length()+2);
	hFile=fopen(newfname,"rb");
	delete newfname;
#endif
	return this->loadTGA(hFile);
}


bool sad::Texture::loadTGA(FILE * hFile)
{
	bool result = true;
	Info textureInfo(&m_data); // Object with data of texture.
	if (hFile)				   // If file was opened successfuly
	{
		result = readTGA(hFile, textureInfo);
		fclose(hFile);		   // Close the file
	}
	else
	{
        this->loadDefaultTGATexture();
		return false;
	}

	// Copying data of TGA texture to object
	m_bpp = textureInfo.m_TGA_bpp;
	m_height = textureInfo.m_TGA_height;
	m_width = textureInfo.m_TGA_width;

	if (result)
	{
		reverseTGA(textureInfo);
	}

	if (!result)
		sad::Texture::loadDefaultTGATexture();

	return result;
}

// Reads data from TGA file.
bool tga::readTGA(FILE *handler, tga::Info & data)
{
	bool result = true;
	sad::uchar headerBuf[18];			// Bytes of TGA header
	TGAHeader header;				// Header of TGA file

	// Reading bytes of header
	fread(headerBuf, sizeof(sad::uchar), sizeof(headerBuf), handler);

	header = getHeader(headerBuf);	// Get the object of header

	// Fill the object of texture's data
	data.m_TGA_bpp	  = header.bitsPerPix;
	data.m_TGA_height = header.height;
	data.m_TGA_width  = header.width;
	data.m_vertflip   = ( header.imageDescriptor & 32 )?true:false;
    data.m_horzflip   = ( header.imageDescriptor & 16 )?true:false;

	data.m_TGA_imageSize = data.m_TGA_height * data.m_TGA_width * data.m_TGA_bpp / 8;

	if (header.imageType == TGA_TRUECOLOR_RLE)		// If the texture was compressed
		result = loadCompressed(data,handler);

	else if ( header.imageType == TGA_TRUECOLOR )	// Else if the texture wasn't compressed 
		result = loadUnCompressed(data, handler);

	else											// Other types of this texture format isn't supported
		result = false;


	return result;
}


sad::TGATextureLoader::~TGATextureLoader()
{


}

bool sad::TGATextureLoader::load(FILE * file, sad::Texture * texture)
{
	tga::Info textureInfo(&(texture->vdata()));		// Object with data of texture.
	bool result = readTGA(file, textureInfo);
	if (result)
	{
		texture->bpp() = textureInfo.m_TGA_bpp;
		texture->height() = textureInfo.m_TGA_height;
		texture->width() = textureInfo.m_TGA_width;
		texture->reverseTGA(textureInfo);
		if (texture->bpp()==24) texture->bpp()=32;
	}
	else
	{
		texture->vdata().clear();
	}
	return result;
}