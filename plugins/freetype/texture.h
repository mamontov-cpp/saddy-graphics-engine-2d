/*! \file texture.h
    

    A mini-texture class, needed to provide lazy upload of texture for building a
    call list for freetype
 */
#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <vector>

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>


namespace sad
{

namespace freetype
{
/*! A mini-texture class, needed to provide lazy upload of textures
 */
class Texture
{
public:
    /*! A pixels, stored in RGB2
     */
    std::vector<unsigned char> Pixels;
    /*! A width of texture
     */
    float Width;
    /*! A height of texture
     */
    float Height;
    
    /*! Determines, whether texture is uploaded on GPU
     */
    bool IsOnGPU;
    /*! An Id of Texture on GPU
     */
    unsigned int  Id;

    /*! Creates empty texture
     */
    Texture();
    /*! Frees a texture id if on GPU
     */
    ~Texture();
    
    /*! Stores a bitmap in texture, providing necessary operations
        \param[in] bitmap a bitmap to store
     */
    void storeBitmap(FT_Bitmap & bitmap);
    /*! Uploads a texture, cleaning pixel field
     */
    void upload();
    /*! Marks texture as current
     */
    void bind();
    /*! Unloads texture from GPU
     */ 
    void unload();
};

}

}
