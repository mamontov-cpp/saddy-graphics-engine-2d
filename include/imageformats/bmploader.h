/*! \file bmploader.h
	\author HiddenSeeker

	Defines a loader for BMP images
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for PNG file format
 */
class BMPLoader: public sad::imageformats::Loader
{
public:
	/*! Loads a texture from file stream. file must be opened in binary format for reading
		\param[in] file
		\param[in] texture
		\return true on success
	 */
	virtual bool load(FILE * file, sad::Texture * texture);
	/*! Kept for purpose of inheritance
	 */
	virtual ~BMPLoader();
};

}

}
