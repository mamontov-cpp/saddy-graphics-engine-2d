/*! \file   picopngloader.h
    \author HiddenSeeker

    \brief  Declaration of thin wrapper, upon PicoPNG (credit goes to  Lode Vandevenne)
*/
#include "../texture.h"
#pragma once

class PicoPNGTextureLoader: public sad::TextureLoader
{
public:
		 /*! Loads a texture
			 \param[in] file
			 \param[in] texture
		  */
		 virtual bool load(FILE * file, sad::Texture * texture);

		 virtual ~PicoPNGTextureLoader();
};

