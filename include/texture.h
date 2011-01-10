/*! \file    texture.h
    \author  HiddenSeeker, Falc_ON, Victor.Grig
    \brief   Contains a texture-related staff
*/

#include "primitives/hcolor.h"
#include "templates/hstring.h"
#include "templates/hwstring.h"
#include "templates/hlvector.hpp"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#pragma once


namespace sad
{
	/*! Texture class
	*/
	class Texture
	{
	public:
		/*! Texture mode
		*/
		enum Mode
		{
			BORDER_REPEAT,
			BORDER_CLAMP,
			COMBINE_REPLACE,
			COMBINE_ADD,
			COMBINE_MODULATE
		};

	private:

		hst::vector<Uint8> m_data;   //!< Bits of texture
		Uint8              m_filter; //!< Filtering method
		Uint8              m_bpp;    //!< Bits per pixel
		unsigned int       m_width;  //!< Width
		unsigned int       m_height; //!< Height
		unsigned int       m_id;     //!< ID of texture
		Mode               m_mode;   //!< Texture mode

		/*! Loads a BMP file, from file
		*/
		bool               loadBMP(FILE * file);
	public:

		/*! Empty texture
		*/
		Texture();
		~Texture();
		/*! Builds a mipmap
		*/
		void buildMipMaps();
		/*! Loads an image, depending of file extension
		\param[in] filename name of file
		*/
		bool load(const hst::string & filename);
		/*! Loads an image, depending of file extension
		\param[in] filename name of file
		*/
		bool load(const hst::wstring & filename);
		/*! Loads a BMP from file
		\param[in] filename name of file
		*/
		bool loadBMP(const hst::string & filename);
		/*! Loads a BMP from file
		\param[in] filename name of file
		*/
		bool loadBMP(const hst::wstring & filename);
		/*! Loads a TGA from file
		\param[in] filename name of file
		*/
		bool loadTGA(const hst::string & filename);
		/*! Loads a TGA from file
		\param[in] filename name of file
		*/
		bool loadTGA(const hst::wstring & filename);
		/*! Loads a PNG from a file
		\param[in] filename name of file
		*/
		bool loadPNG(const hst::string & filename);
		/*! Loads a PNG from a file
		\param[in] filename name of file
		*/
		bool loadPNG(const hst::wstring & filename);
		/*! Loads default texture
		*/
		void loadDefaultTGATexture();
		/*! Disables texture
		*/
		void disable();
		/*! Enables texture
		*/
		void enable();
		/*! Sets an alpha-channel value for a color
		    \param[in] a alpha-channel value
		*/
		void setAlpha(Uint8 a);
		/*! Sets an alpha-channel value for a color
		    \param[in] a    alpha-channel value
			\param[in] clr  color
			\param[in] prec precision
		*/
		void setAlpha(Uint8 a, const hst::color & clr, Uint8 prec=0);
		/*! Sets a mode for texture
		    \param[in] mode mode of texture
		*/
		void setMode(Texture::Mode mode);
		/*! Saves a texture to a precompiled source code
		*/
		void save(const char * method, const char * file);
	};
}
