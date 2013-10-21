#include "imageformats/bmploader.h"

#pragma pack(push, 1)

namespace sad
{

namespace imageformats
{

struct BMPHeader
{
	unsigned short type;
	unsigned int  size;
	unsigned short reserv1;
	unsigned short reserv2;
	unsigned int  offsetbits;
};

struct BMPImageHeader
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

}

}

#pragma pack(pop)

static const short bmp_magic_number = 19778;

bool sad::imageformats::BMPLoader::load(FILE * file, sad::Texture * texture)
{
	if (file == NULL || texture == NULL)
		return false;

	// Try to read header
	sad::imageformats::BMPHeader header;
	size_t read = fread(&header, sizeof(sad::imageformats::BMPHeader), 1, file);
	if (read != sizeof(sad::imageformats::BMPHeader))
	{
		return false;
	}

	// Check for magic number
	if (header.type != bmp_magic_number)
	{
		return false;
	}

	// Read image information
	sad::imageformats::BMPImageHeader image_header;
	read = fread(&image_header, sizeof(sad::imageformats::BMPImageHeader), 1, file);
	if (read != sizeof(sad::imageformats::BMPImageHeader))
	{
		return false;
	}

	// Check whether image is supported
	bool image_unsupported = (image_header.bitcount != 24 && image_header.bitcount != 32);
	if (image_header.width == 0 || image_header.height == 0 || image_unsupported)
	{
		return false;
	}

	unsigned char bypp = image_header.bitcount / 8;
	
	unsigned int width = image_header.width;
	unsigned int fileimagesize = image_header.width * image_header.height;  

	texture->width() = width;
	texture->height() = image_header.height;
	texture->bpp() = 32;
	texture->vdata().resize(fileimagesize * 4);

	int x = 0;
	int y = image_header.height - 1;
	
	sad::uchar * buffer = &(texture->vdata()[0]);
	for (unsigned long i = 0; i < fileimagesize; i++)
	{
		unsigned int offset = 4 * (y * width + x);
		unsigned char * offset_buffer = buffer + offset;

		if(fread(offset_buffer, sizeof(unsigned char), bypp, file) != bypp)
		{
			return false;
		}
		
		std::swap(*offset_buffer, *(offset_buffer+2));
				
		++x;
		if (x == width)
		{
			y -= 1;
			x =  0;
		}
	}

	return true;
}


sad::imageformats::BMPLoader::~BMPLoader()
{

}
