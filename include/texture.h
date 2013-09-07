/*! \file    texture.h
    \author  HiddenSeeker, Falc_ON, Victor.Grig
    \brief   Contains a texture-related staff
*/

#include "primitives/hcolor.h"
#include "primitives/hrect.h"
#include "templates/hstring.h"
#include "templates/hwstring.h"
#include "templates/hlvector.hpp"
#include <stdio.h>

#ifdef WIN32
	#ifndef NOMINMAX
    #define NOMINMAX 
    #endif
	#include <windows.h>
	#include <gl/gl.h>
	#include <gl/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#include <renderer.h>


#pragma once

namespace tga
{
class Info;
}

namespace sad
{
	class Texture;
	/*! Texture loader class for specific formats, which can be put into a TextureManager
		to perform all kinds of loading
	 */
	class TextureLoader
	{
	 public:
		 /*! Loads a texture
			 \param[in] file
			 \param[in] texture
		  */
		 virtual bool load(FILE * file, sad::Texture * texture) = 0;

		 virtual ~TextureLoader();
	};
	class BMPTextureLoader: public sad::TextureLoader
	{
	 public:
		 /*! Loads a texture
			 \param[in] file
			 \param[in] texture
		  */
		 virtual bool load(FILE * file, sad::Texture * texture);

		 ~BMPTextureLoader();
	};
	class TGATextureLoader;
	/*! Texture class
	*/
	class Texture
	{
	public:
		friend class TGATextureLoader;
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
		bool               m_generated; //!< Determines, whether texture is used in OpenGL context
		/*! Gets a pointer to pixel 
		    \param[in] i  row
			\param[in] j  col
		*/
		inline Uint8 *  pixel(unsigned int i,unsigned int j)
		{ return   &(m_data[(i*m_width+j)*(m_bpp >> 3)]);}
		/*! Sets pixel alpha component
			\param[in] i row
			\param[in] j col
			\param[in] alpha alpha component
		 */
		inline void setPixelAlpha(unsigned int i, unsigned int j, Uint8 alpha)
		{  m_data[(i*m_width+j)*(m_bpp >> 3)+3] =alpha; }
		/*! Upscales a texture, using a simple resize
		    \param[in] width new width
			\param[in] height new height
		*/
		void scaleFast(int width, int height);
		/*! Upscales a texture, using a simplified bilinear resize
		    \param[in] width new width
			\param[in] height new height
		*/
		void scale(int width, int height);
		/*! Loads a BMP file, from file
			\param[in] file file handle
		*/
		bool               loadBMP(FILE * file);
		/*! Reloads TGA from info
			\param textureInfo a texture information
		*/
		void                reverseTGA(const tga::Info & textureInfo);

	protected:
		/*! Loads a TGA image from specified file
			\param[in] hFile supplied file
			\return whether image was succesfully loaded
		 */
	    bool loadTGA(FILE * hFile);
	public:
		/*! Makes a texture POT
		*/
		void makePOT();
		/*! Empty texture
		*/
		Texture();
		~Texture();
		/*! Builds a mipmap
		*/
		void buildMipMaps();
		/*! Loads an image, depending of file extension
		    \param[in] filename name of file
		    \param[in] r renderer where texture loader is getting from
		*/
		bool load(const hst::string & filename, sad::Renderer * r = sad::Renderer::ref());
		/*! Loads an image, depending of file extension
		    \param[in] filename name of file
            \param[in] r renderer where texture loader is getting from
		*/
		bool load(const hst::wstring & filename, sad::Renderer * r = sad::Renderer::ref());
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
		/*! Sets alpha-channel value fro a color in a rectangle
			\param[in] a  alpha-channel value
			\param[in] clr color
			\param[in] rect rectangle
		 */
		void setAlpha(Uint8 a, const hst::color & clr,const hRectF & rect);
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

		inline unsigned int width()  const { return m_width; }
		inline unsigned int height() const { return m_height;}
		inline Uint8 bpp()    const { return m_bpp;}
		inline unsigned int& width()    { return m_width; }
		inline unsigned int& height()   { return m_height;}
		inline Uint8& bpp()      { return m_bpp;}
		inline Uint8 * data() const { return m_data.data(); }
		inline hst::vector<Uint8> & vdata() { return m_data; }
		/*! Unloads a texture from videocard memory
		 */
		void unload();
	};
}
