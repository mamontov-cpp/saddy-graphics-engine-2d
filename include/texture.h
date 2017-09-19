/*! \file    texture.h
    \author  mamontov-cpp, Falc_ON, Victor.Grig
    
    Contains definition of texture class
*/
#pragma once

#include "sadcolor.h"
#include "sadrect.h"
#include "sadstring.h"
#include "sadwstring.h"
#include "sadvector.h"

#ifndef TEXTURE_LOADER_TEST
#include "resource/resource.h"
#endif

#include <stdio.h>

namespace tar7z
{
class Archive;
class Entry;
}

namespace sad
{
class Renderer;

/*! A main texture class, which stores all related data to a texture
    providing simple interface for working with it
 */
class Texture
#ifndef TEXTURE_LOADER_TEST
: public sad::resource::Resource
#endif
{
#ifndef TEXTURE_LOADER_TEST 
SAD_OBJECT
#endif
public:
/*! An inner buffer, that provides texture with data to upload
 */
class Buffer
{
public:
    /*! Returns buffer contents for a texture
        \return buffer
     */ 
    virtual sad::uchar* buffer() const = 0;
    /*! A destructor for buffer
     */ 
    virtual ~Buffer();
};
/*! A default buffer, that just stores a texture into vector 
 */
class DefaultBuffer: public Buffer
{
public:
    /*! Constructs new empty buffer
     */
    DefaultBuffer();
    /*! Returns buffer contents for a texture
        \return buffer
     */ 
    virtual sad::uchar* buffer() const;
    /*! A destructor for buffer
     */ 
    virtual ~DefaultBuffer();
    /*! A texture pixels stored as bytes, according to texture format
     */
    sad::Vector<sad::uchar> Data;
};
/*! A wrapper for just a pointer to array of pixels
 */
class PointerBuffer : public Buffer
{
public:
    /*! Constructs new empty buffer
        \param[in] p a pointer to buffer
     */
    PointerBuffer(sad::uchar* p);
    /*! Returns buffer contents for a texture
        \return buffer
     */
    virtual sad::uchar* buffer() const;
    /*! A destructor for buffer
     */
    virtual ~PointerBuffer();
    /*! A texture pixels stored as bytes, according to texture format
     */
    sad::uchar* Data;
};
/*! A buffer, which is pointing to tar7z archive, which has been loaded to memory
 */
class Tar7zArchiveBuffer: public Buffer
{
public:
    /*! Constructs new empty buffer
     */
    Tar7zArchiveBuffer();
    /*! Returns buffer contents for a texture
        \return buffer
     */ 
    virtual sad::uchar* buffer() const;
    /*! A destructor for buffer
     */ 
    virtual ~Tar7zArchiveBuffer();

    /*! Archive for buffer
     */
    tar7z::Archive* Archive;
    /*! Offset of buffer start for texture
     */
    size_t Offset;
};

/*! A buffer which contains default image
 */
class DefaultImageBuffer: public Buffer
{
public:
    /*! Constructs new empty buffer
     */
    DefaultImageBuffer();
    /*! Returns buffer contents for a texture
        \return buffer
     */ 
    virtual sad::uchar* buffer() const;
    /*! A destructor for buffer
     */ 
    virtual ~DefaultImageBuffer();
};
/*! Describes a format of engine's texture
 */
enum InternalFormat 
{
    SFT_R8_G8_B8_A8, //!< A default true-color format of texture
    SFT_R5_G6_B5,    //!< A format, which has 5 bits for red component, 6 bits for green component, 5 bits for blue component
    SFT_R4_G4_B4_A4, //!< A format, which has 4 bits for each component 
    SFT_R3_G3_B2,    //!< A format, which has 3 bits for red component, 3 bits for green component, 2 bits for blue component
};
    /*! Whether we should build mip-maps, when uploading texture to GPU
     */
    bool BuildMipMaps;
    /*! A buffer, which should contain pixels, which will be uploaded to GPU
     */
    sad::Texture::Buffer* Buffer;   
    /*! Bits per pixel
     */
    sad::uchar              Bpp;
    /*! Format of texture, defines how pixels are stored in texture
     */
    sad::Texture::InternalFormat Format;    
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
    inline sad::uchar *  pixel(unsigned int i,unsigned int j) const
    { 
        return   &((Buffer->buffer())[(i * Width + j)*(Bpp >> 3)]);
    }
    /*! Sets pixel alpha component value
        \param[in] i row
        \param[in] j col
        \param[in] alpha alpha component value
     */
    inline void setPixelAlpha(unsigned int i, unsigned int j, sad::uchar alpha) const
    { 
        (Buffer->buffer())[(i * Width + j)*(Bpp >> 3) + 3] =alpha; 
    }
    /*! Uploads a texture to a GPU, building a mipmaps if can. Note, that if
        texture has zero size it won't be uploaded
     */
    void upload();
    /*! Loads default texture, hardcoded inside of engine
     */
    void loadDefaultTexture();
    /*! Unloads all resources from GPU. By default does nothing
     */
    virtual void unloadFromGPU();
    /*! Returns whether resource supports loading from archive
        \return true. This resource supports loading from archives.
     */
    virtual bool supportsLoadingFromTar7z() const;
#ifndef TEXTURE_LOADER_TEST
    /*! Loads a texture from specified file, using specified renderer for building mip maps.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    );
    /*! Tries to load image from archive entry
        \param[in] e entry entry, containing data
        \param[in] r renderer
        \return whether load was successfull
     */
    bool load(tar7z::Entry* e, sad::Renderer* r = NULL);
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
#endif
    /*! Binds a texture, making it current in context. Can upload a texture, if needed.
     */
    void bind();
    /*! Unloads a texture from videocard memory
     */
    void unload();
    /*! Sets an alpha-channel value for a color
        \param[in] a alpha-channel value
     */
    void setAlpha(sad::uchar a) const;
    /*! Sets alpha-channel value for a color 
        \param[in] a alpha-channel value
        \param[in] clr color
     */
    void setAlpha(sad::uchar a, const sad::Color & clr) const ;
    /*! Sets alpha-channel value for a color in a rectangle
        \param[in] a  alpha-channel value
        \param[in] clr color
        \param[in] rect rectangle
     */
    void setAlpha(sad::uchar a, const sad::Color & clr,const sad::Rect2D & rect) const;
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
        return Buffer->buffer(); 
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
private:
    /*! Checks for errors in work of OpenGL and converts them into string
        \return string with error description (NULL if there wasn't any error)
    */
    unsigned char const * getGLError();
};

}

#ifndef TEXTURE_LOADER_TEST
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Texture)
#endif