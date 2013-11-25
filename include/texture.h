/*! \file    texture.h
    \author  HiddenSeeker, Falc_ON, Victor.Grig
    \brief   Contains a texture-related staff
*/

#include "sadcolor.h"
#include "sadrect.h"
#include "sadstring.h"
#include "sadwstring.h"
#include "sadvector.h"
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

namespace sad
{
class TextureContainer;

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

		sad::Vector<sad::uchar> m_data;   //!< Bits of texture
		sad::uchar              m_filter; //!< Filtering method
		sad::uchar              m_bpp;    //!< Bits per pixel
		unsigned int       m_width;  //!< Width
		unsigned int       m_height; //!< Height
		unsigned int       m_id;     //!< ID of texture
		Mode               m_mode;   //!< Texture mode
		/*! Determines, whether texture is used in OpenGL context
		 */
		bool               m_ongpu; 
		/*! Gets a pointer to pixel 
		    \param[in] i  row
			\param[in] j  col
		*/
		inline sad::uchar *  pixel(unsigned int i,unsigned int j)
		{ return   &(m_data[(i*m_width+j)*(m_bpp >> 3)]);}
		/*! Sets pixel alpha component
			\param[in] i row
			\param[in] j col
			\param[in] alpha alpha component
		 */
		inline void setPixelAlpha(unsigned int i, unsigned int j, sad::uchar alpha)
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
	public:
		/*! Makes a texture POT
		*/
		void makePOT();
		/*! Empty texture
		*/
		Texture();
		~Texture();
		/*! Uploads a texture to a GPU, building a mipmap
		*/
		void upload();
		/*! Loads an image, depending of file extension
		    \param[in] filename name of file
		    \param[in] r renderer where texture loader is getting from
		*/
		bool load(const sad::String & filename, sad::Renderer * r = sad::Renderer::ref());
		/*! Loads an image, depending of file extension
		    \param[in] filename name of file
            \param[in] r renderer where texture loader is getting from
		*/
		bool load(const sad::WString & filename, sad::Renderer * r = sad::Renderer::ref());
		/*! Loads default texture
		 */
		void loadDefaultTGATexture();
		/*! Enables texture
		*/
		void bind();
		/*! Sets an alpha-channel value for a color
		    \param[in] a alpha-channel value
		*/
		void setAlpha(sad::uchar a);
		/*! Sets alpha-channel value fro a color in a rectangle
			\param[in] a  alpha-channel value
			\param[in] clr color
			\param[in] rect rectangle
		 */
		void setAlpha(sad::uchar a, const sad::Color & clr,const sad::Rect2D & rect);
		/*! Sets an alpha-channel value for a color
		    \param[in] a    alpha-channel value
			\param[in] clr  color
			\param[in] prec precision
		*/
		void setAlpha(sad::uchar a, const sad::Color & clr, sad::uchar prec=0);
		/*! Sets a mode for texture
		    \param[in] mode mode of texture
		*/
		void setMode(Texture::Mode mode);
		/*! Saves a texture to a precompiled source code
		*/
		void save(const char * method, const char * file);

		inline unsigned int width()  const { return m_width; }
		inline unsigned int height() const { return m_height;}
		inline sad::uchar bpp()    const { return m_bpp;}
		inline unsigned int& width()    { return m_width; }
		inline unsigned int& height()   { return m_height;}
		inline sad::uchar& bpp()      { return m_bpp;}
		inline sad::uchar * data() const { return m_data.data(); }
		inline sad::Vector<sad::uchar> & vdata() { return m_data; }
		/*! Unloads a texture from videocard memory
		 */
		void unload();

	/*! Returns a container, which texture belongs to
		\return a container, which texture belongs to
	 */
	sad::TextureContainer * container() const;
	/*! Returns a renderer, which should render a texture
	 */
	sad::Renderer * renderer() const;
	/*! Sets container, where texture belongs to
		\param[in] container a container, where texture should belong
	 */
	void setContainer(sad::TextureContainer * container);
protected: 
	/*! A container for textures
	 */
	sad::TextureContainer * m_container;
};

}
