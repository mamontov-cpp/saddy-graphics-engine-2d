#include "imageformats/tgaloader.h"


namespace sad
{

namespace imageformats
{

enum ImageType
{
	NO_DATA		  = 0;		/*!< Image doesn't contains any data.					 */
	COLORMAP      = 1;		/*!< Color map without RLE compression.					 */
	TRUECOLOR	  = 2;		/*!< RGB image without RLE compression.					 */
	MONOCHROME	  = 3;		/*!< Monochrome image without RLE compression.			 */
	COLORMAP_RLE  = 9;		/*!< Color map with RLE compression.					 */
	TRUECOLOR_RLE = 10;		/*!< RGB image with RLE compression.					 */
	MONOCHROME_RLE = 11;		/*!< Monochrome image with RLE compression.				 */
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
static const unsigned char supported_image_types[supported_image_types_count] = { sad::imageformats::TRUECOLOR, sad::imageformats::TRUECOLOR_RLE };

bool sad::imageformats::TGALoader::load(FILE * file, sad::Texture * texture)
{
	if (file == NULL || texture == NULL)
	{
		return false;
	}

	/*! Try to read the tga header
	 */
	const unsigned int header_buffer_size = 18;
	sad::uchar header_buffer[header_buffer_size];
	unsigned int read_byte = fread(header_buffer, sizeof(sad::uchar), header_buffer_size, handler);
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

	return true;
}


sad::imageformats::TGALoader::~TGALoader()
{

}
