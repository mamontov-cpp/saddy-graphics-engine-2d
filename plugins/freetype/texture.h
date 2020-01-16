/*! \file texture.h
    

    A mini-texture class, needed to provide lazy upload of texture for building a
    call list for freetype
 */
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
#include <vector>

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "bindable.h"


namespace sad
{

namespace freetype
{
/*! A mini-texture class, needed to provide lazy upload of textures
 */
class Texture: public sad::Bindable
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
    virtual ~Texture();
    
    /*! Stores a bitmap in texture, providing necessary operations
        \param[in] bitmap a bitmap to store
     */
    void storeBitmap(FT_Bitmap & bitmap);
    /*! Uploads a texture, cleaning pixel field
     */
    void upload();
    /*! Marks texture as current
     */
    virtual void bind();
    /*! Unloads texture from GPU
     */ 
    void unload();
    /*! Copies pixel data
     *  \param[in] dx destination col
     *  \param[in] dy destination row
     *  \param[in] sx source column
     *  \param[in] sy source row
     *  \param[in] bitmap source bitmap
     */
    void copyPixel(int dx, int dy, int sx, int sy, FT_Bitmap & bitmap);
    /*! Copies row data
     *  \param[in] dx destination col
     *  \param[in] dy destination row
     *  \param[in] sx source column
     *  \param[in] sy source row
     *  \param[in] width a width data
     *  \param[in] bitmap source bitmap
     */
    void copyRow(int dx, int dy, int sx, int sy, int width, FT_Bitmap & bitmap);
    /*! Copies row data
     *  \param[in] dx destination col
     *  \param[in] dy destination row
     *  \param[in] sx source column
     *  \param[in] sy source row
     *  \param[in] width a width data
     *  \param[in] bitmap source bitmap
     */
    void copySubImage(int dx, int dy, int sx, int sy, int width, int height, FT_Bitmap & bitmap);

};

}

}
