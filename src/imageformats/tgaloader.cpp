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
		return false;

	/*! Buffer of tga header
	 */
	const unsigned int header_buffer_size = 18;
	sad::uchar header_buffer[header_buffer_size];
	fread(header_buffer, sizeof(sad::uchar), header_buffer_size, handler);

	sad::imageformats::TGAHeader header;

	return true;
}


sad::imageformats::TGALoader::~TGALoader()
{

}
