/*! \file tgaloader.h
	\author HiddenSeeker

	Defines a loader for TGA images
 */
#pragma once
#include "loader.h"

namespace sad
{

namespace imageformats
{
/*! Defines a loader for PNG file format
 */
class TGALoader: public sad::imageformats::Loader
{
public:
	/*! Loads a texture from file stream. file must be opened in binary format for reading
		\param[in] file
		\param[in] texture
		\return true on success
	 */
	virtual bool load(FILE * file, sad::Texture * texture) = 0;
	/*! Kept for purpose of inheritance
	 */
	virtual ~TGALoader();
};

}

}
