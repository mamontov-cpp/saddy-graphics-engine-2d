/*! \file loader.h
	\author HiddenSeeker

	Defines a loader, which can load a texture from file stream,
	using specified format.
 */
#pragma once
#include "../texture.h"
#include <cstdio>

namespace sad
{

namespace imageformats
{
/*! Defines a loader, which can load a texture from file stream,
	using specified format.
 */
class Loader
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
	virtual ~Loader();
};

}

}
