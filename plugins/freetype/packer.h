/*! \file packer.h

    Describes a packer, used to create texture atlas for the font
 */
#pragma once
#include "glyph.h"
#include "texture.h"

namespace sad
{

namespace freetype
{

/*! A packer for packing all textures from glyph to texture atlas
 */
class Packer
{
public:
/*! A class of texture container for packer
 */
class T
{
public:
    /*! Creates a new texture wrapper
     */
    T();
    /*! Creates a new texture wrapper for texture
        \param[in] t glyph
     */
    T(sad::freetype::Glyph* t);
    /*! Returns width for texture
        \return width
     */
    int width() const;
    /*! Returns height for texture
        \return height
     */
    int height() const;
    /*! Returns area for texture
     */
    int area() const;
    /*! "Resizes" area. Just sets inner properties
        \param[in] w width
        \param[in] h height
     */
    void resize(double w, double h);
    /*! Just returns zero
        \param[in] x x value
        \param[in] y y value
     */
    int pixel(int x, int y);
    /*! Does nothing
        \param[in] x x value
        \param[in] y y value
        \param[in] p pixel
     */
    void pixel(int x, int y, int p);
    /*! Just returns false
        \param[in] x x value
        \param[in] y y value
     */
    bool is_transparent(int x, int y);
    /*! Copies data
        \param[in] block a block value
        \param[in] x x value
        \param[in] y y value
        \param[in] width width
        \param[in] height height
        \param[in] px
        \param[in] py
     */
    void copy_from(
        sad::freetype::Packer::T block,
        int x,
        int y,
        int width,
        int height,
        int px,
        int py
    );
protected:
    /*! A width for container
     */
    double m_width;
    /*! A height for container
     */
    double m_height;
    /*! A texture
     */
    sad::freetype::Glyph* m_t;
};
/*! Packs glyphs into textures
 *  \param[in] glyphs data
 */
static sad::freetype::Texture* pack(sad::freetype::Glyph* glyphs[256]);
};

}

}