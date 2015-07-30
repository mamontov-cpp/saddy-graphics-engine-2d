#include "imageformats/tgaloader.h"

#include <algorithm>

namespace sad
{

namespace imageformats
{

enum ImageType
{
	IT_NO_DATA		  = 0,		/*!< Image doesn't contains any data.					 */
	IT_COLORMAP       = 1,		/*!< Color map without RLE compression.					 */
	IT_TRUECOLOR	  = 2,		/*!< RGB image without RLE compression.					 */
	IT_MONOCHROME	  = 3,		/*!< Monochrome image without RLE compression.			 */
	IT_COLORMAP_RLE   = 9,		/*!< Color map with RLE compression.					 */
	IT_TRUECOLOR_RLE  = 10,		/*!< RGB image with RLE compression.					 */
	IT_MONOCHROME_RLE = 11 	    /*!< Monochrome image with RLE compression.				 */
};

struct TGAHeader
{
	unsigned char  idLength;			/*!< Length of ID field.						(1 byte) */
	unsigned char  colorMapType;		/*!< Type of color map.							(1 byte) */
	unsigned char  imageType;			/*!< type of image.								(1 byte) */

	unsigned short colorMapOrigin;		/*!< First entry of color map.					(2 byte) */
	unsigned short colorMapSize;		/*!< Length of color map.						(2 byte) */
	unsigned char  colorMapEntrySize;	/*!< Size of color map.							(1 byte) */

	unsigned short XOrigin;				/*!< Horizontal coordinate of image's origin.	(2 byte) */
	unsigned short YOrigin;				/*!< Vertical coordinate of image's origin.		(2 byte) */
	unsigned short width;				/*!< Width of the image.						(2 byte) */
	unsigned short height;				/*!< Height of the image.						(2 byte) */
	unsigned char  bitsPerPix;			/*!< Bits per pixel.							(1 byte) */
	unsigned char  imageDescriptor;		/*!< Bits of image descriptor.					(1 byte) */
};

}

}

static const unsigned int  supported_image_types_count = 2;
static const unsigned char supported_image_types[supported_image_types_count] = { 
	sad::imageformats::IT_TRUECOLOR, 
	sad::imageformats::IT_TRUECOLOR_RLE 
};

bool sad::imageformats::TGALoader::load(FILE * file, sad::Texture * texture)
{
	if (file == NULL || texture == NULL)
	{
		return false;
	}
	m_file = file;
	/*! Try to read the tga header
	 */
	const unsigned int header_buffer_size = 18;
	sad::uchar header_buffer[header_buffer_size];
	unsigned int read_byte = fread(
		header_buffer, 
		sizeof(sad::uchar), 
		header_buffer_size, 
		file
	);
	if (read_byte != header_buffer_size)
	{
		return false;
	}

	/*! Decode header
	 */ 
	sad::imageformats::TGAHeader header;
	header.idLength			= header_buffer[0];
	header.colorMapType		= header_buffer[1];
	header.imageType		= header_buffer[2];
	header.colorMapOrigin	= header_buffer[3]  + 256 * header_buffer[4];
	header.colorMapSize		= header_buffer[5]  + 256 * header_buffer[6];
	header.colorMapEntrySize= header_buffer[7];
	header.XOrigin			= header_buffer[8]  + 256 * header_buffer[9];
	header.YOrigin			= header_buffer[10] + 256 * header_buffer[11];
	header.width			= header_buffer[12] + 256 * header_buffer[13];
	header.height			= header_buffer[14] + 256 * header_buffer[15];
	header.bitsPerPix		= header_buffer[16];
	header.imageDescriptor	= header_buffer[17];

	bool image_unsupported = header.imageType == sad::imageformats::IT_TRUECOLOR
						  && header.imageType == sad::imageformats::IT_TRUECOLOR_RLE;
	if (header.width == 0 || header.height == 0 || image_unsupported)
	{
		return false;
	}

	texture->vdata().clear();
	// Resize to ARGB size and set texture properties
	texture->width() = header.width;
	texture->height() = header.height;
	texture->bpp() = 32;
	texture->vdata().resize(header.width * header.height * 4, 255);
	
	// Set bytes per pixel
	m_bypp = header.bitsPerPix / 8;
	bool result = false;
	if (header.imageType == IT_TRUECOLOR_RLE) 
	{
		result = loadCompressed(texture);
	}
	if ( header.imageType == IT_TRUECOLOR )	
	{
		result = loadRaw(texture);
	}

	// This is not a mistake! We should not flip image, otherwise texture 
	// will be flipped
	m_flip_x = (header.imageDescriptor & 16) != 0;
	m_flip_y = (header.imageDescriptor & 32) == 0;
	if ((m_flip_x || m_flip_y) && result)
	{
		this->flip(texture);
	}
	return result;
}


sad::imageformats::TGALoader::~TGALoader()
{

}

bool sad::imageformats::TGALoader::loadRaw(sad::Texture * texture)
{
	unsigned int raw_image_size = m_bypp * texture->width() * texture->height();
	sad::uchar * begin = &(texture->vdata()[0]);
	unsigned int bypp = m_bypp;
	for (unsigned int i = 0; i < raw_image_size; i += bypp)
	{
		if (fread(begin, sizeof(sad::uchar), bypp, m_file) != bypp)
		{
			return false;
		}
		std::swap(*begin, *(begin + 2));
		begin += 4; 
	}
    return true;
}

bool sad::imageformats::TGALoader::loadCompressed(sad::Texture * texture)
{
	bool result = true;				// Result
	
	// Allocate buffer and read data
	std::vector<unsigned char> buffer;
	fseek(m_file, 0L, SEEK_END);
	unsigned int size = ftell(m_file);
	fseek(m_file, 0L, SEEK_SET);
	buffer.resize((size_t)size, 255);
	size_t readbytes = fread((char*)(&buffer[0]), 1, size, m_file);
	if (readbytes != size)
	{
		buffer.clear();
		return false;
	}
	
	unsigned char * rle_buffer = &(buffer[0]);
	unsigned char * image_buffer = &(texture->vdata()[0]);
	unsigned int rle_buffer_pos = 0;
	unsigned int rle_buffer_size = buffer.size();
	unsigned int image_size = texture->vdata().size();
	unsigned int image_pos = 0;
	// A copied pixel
	unsigned char pixel[4];
	pixel[3] = 255;
	
	while (image_pos < image_size && rle_buffer_pos < rle_buffer_size)
	{
		unsigned char chunkheader = rle_buffer[rle_buffer_pos];
		// If raw section
		if (chunkheader < 128)
		{
			chunkheader++;
			// Move to next byte
			rle_buffer_pos++;
			if (rle_buffer_pos + m_bypp - 1 >= rle_buffer_size)
			{
				return false;
			}

			for(unsigned int i = 0; i < chunkheader; i++)
			{
				if (image_pos >= image_size)
				{
					return false;
				}
				memcpy(image_buffer + image_pos, rle_buffer + rle_buffer_pos, m_bypp);
				// Swap BGR to RGB
				std::swap(*(image_buffer + image_pos), *(image_buffer + image_pos + 2)); 
				rle_buffer_pos += m_bypp;
				image_pos += 4;
			}
		}
		else
		{
			// Unset ID bit
			chunkheader -= 127;
			// Move to next byte
			rle_buffer_pos++;
			if (rle_buffer_pos + m_bypp - 1 >= rle_buffer_size)
			{
				return false;
			}

			memcpy(pixel, rle_buffer + rle_buffer_pos, m_bypp);
			std::swap(*(pixel), *(pixel + 2));
			for(unsigned int i = 0; i < chunkheader; i++)
			{
				if (image_pos >= image_size)
				{
					return false;
				}
				memcpy(image_buffer + image_pos, pixel, 4);
				image_pos += 4;
			}
		}
	}
	return result;
}

void sad::imageformats::TGALoader::flip(sad::Texture * texture)
{
	unsigned int height = texture->height();
	unsigned int rowsize = texture->width() * 4; // 4 is four bytes per row
	unsigned int halfpixelwidth = texture->width() / 2;
	unsigned int halfwidth =  halfpixelwidth * 4;
	unsigned int halfheight = height / 2;

	sad::uchar * flipbuffer = new sad::uchar[rowsize];
	sad::uchar * begin = &(texture->vdata()[0]);
	if (m_flip_x && (height % 2) == 1)
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
		unsigned int offset2 =(height - 1 - row) * rowsize;
		sad::uchar * begin1 = begin + offset1;
		sad::uchar * begin2 = begin + offset2;
		if (m_flip_x)
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
		if (m_flip_y)
		{
			memcpy(flipbuffer, begin1, rowsize * sizeof(sad::uchar));
			memcpy(begin1, begin2, rowsize * sizeof(sad::uchar));
			memcpy(begin2, flipbuffer, rowsize * sizeof(sad::uchar));
		}
	}
	delete[] flipbuffer;
}
