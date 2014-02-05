/*! \file    texture.h
    \author  HiddenSeeker, Falc_ON, Victor.Grig
    
	Contains definition of texture class
*/
#pragma once

#include "sadcolor.h"
#include "sadrect.h"
#include "sadstring.h"
#include "sadwstring.h"
#include "sadvector.h"

#include "resource/resource.h"

#include <stdio.h>


namespace sad
{
class TextureContainer;
class Renderer;

/*! A main texture class, which stores all related data to a texture
	providing simple interface for working with it
 */
class Texture: public resource::Resource
{
SAD_OBJECT
public:
	/*! A texture pixels stored as bytes in RGBA order
	 */
	sad::Vector<sad::uchar> Data;   
	/*! Bits per pixel
	 */
	sad::uchar              Bpp;    
	/*! Width of a texture in pixels
	 */
	unsigned int       Width; 
	/*! Height of a texture in pixels
	 */
	unsigned int       Height;
	/*! Id of a texture on GPU
	 */
	unsigned int       Id;     
	/*! Determines, whether texture is used in OpenGL context
	 */
	bool               OnGPU; 
	/*! Creates new empty texture
	 */
	Texture();
	/*! You can inherit texture to implement it procedural generation and other
		interesting stuff.
	 */
	virtual ~Texture();
	/*! Gets a pointer to pixel 
		\param[in] i  row
		\param[in] j  col
	 */
	inline sad::uchar *  pixel(unsigned int i,unsigned int j)
	{ 
		return   &(Data[(i * Width + j)*(Bpp >> 3)]);
	}
	/*! Sets pixel alpha component value
		\param[in] i row
		\param[in] j col
		\param[in] alpha alpha component value
	 */
	inline void setPixelAlpha(unsigned int i, unsigned int j, sad::uchar alpha)
	{ 
		Data[(i * Width + j)*(Bpp >> 3) + 3] =alpha; 
	}
	/*! Uploads a texture to a GPU, building a mipmaps if can. Note, that if
		texture has zero size it won't be uploaded
	 */
	void upload();
	/*! Loads default texture, hardcoded inside of engine
	 */
	void loadDefaultTexture();
	/*! Loads a texture from specified file, using specified renderer for building mip maps.
		\param[in] file a file, via which a resource should be loaded
		\param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
		\param[in] v  an options for loading a resource
		\param[in] store_links hints, whether we should store a links
		\return whether loading was successfull
	 */
	virtual bool load(
		const sad::resource::PhysicalFile & file,
		sad::Renderer * r = NULL,
		const picojson::value& options = picojson::value(picojson::object_type, false),
		bool store_links = true
	);
	/*! Loads an image, depending of file extension
		\param[in] filename name of file
		\param[in] r renderer where texture loader is getting from
		\return whether load was successfull
	 */
	bool load(const sad::String & filename, sad::Renderer * r = NULL);
	/*! Loads an image, depending of file extension
		\param[in] filename name of file
        \param[in] r renderer where texture loader is getting from
		\return whether load was successfull
	 */
	bool load(const sad::WString & filename, sad::Renderer * r = NULL);
	/*! Binds a texture, making it current in context. Can upload a texture, if needed.
	 */
	void bind();
	/*! Unloads a texture from videocard memory
	 */
	void unload();
	/*! Sets an alpha-channel value for a color
		\param[in] a alpha-channel value
	 */
	void setAlpha(sad::uchar a);
	/*! Sets alpha-channel value for a color 
		\param[in] a alpha-channel value
        \param[in] clr color
	 */
	void setAlpha(sad::uchar a, const sad::Color & clr);
	/*! Sets alpha-channel value for a color in a rectangle
        \param[in] a  alpha-channel value
        \param[in] clr color
        \param[in] rect rectangle
     */
    void setAlpha(sad::uchar a, const sad::Color & clr,const sad::Rect2D & rect);
	/*! Returns a renderer, which should render a texture
	 */
	sad::Renderer * renderer() const;
	/*! Returns width of texture
		\return width of texture
	 */
	inline unsigned int width()  const 
	{ 
		return Width; 
	}
	/*! Returns height of texture
		\return height of texture
	 */
	inline unsigned int height() const 
	{ 
		return Height;
	}
	/*! Returns bits per pixels
		\return bits per pixels
	 */
	inline sad::uchar bpp()    const
	{ 
		return Bpp;
	}
	/*! Returns width of texture
		\return width of texture
	 */
	inline unsigned int& width()    
	{ 
		return Width; 
	}
	/*! Returns height of texture
		\return height of texture
	 */
	inline unsigned int& height()   
	{ 
		return Height;
	}
	/*! Returns bits per pixels
		\return bits per pixels
	 */
	inline sad::uchar& bpp()      
	{ 
		return Bpp;
	}
	inline sad::uchar * data() const 
	{ 
		return Data.data(); 
	}
	/*! Return data as vector
		\return data of vector with pixels
	 */
	inline sad::Vector<sad::uchar> & vdata() 
	{ 
		return Data; 
	}
	/*! Sets a rendererer for texture uploading
	 */
	inline void setRenderer(sad::Renderer * r)
	{
		m_renderer = r;
	}
	/*! Makes a texture square with power of 
		two sides, filling added pixels with zero bytes
	 */
	void convertToPOTTexture();
protected:
	/*! A renderer, which is held by a texture
	 */
	sad::Renderer * m_renderer;
};

}
