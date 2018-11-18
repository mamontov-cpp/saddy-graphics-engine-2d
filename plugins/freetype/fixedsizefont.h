/*! \file fixedheightfont.h
    

    Describes a font with fixed height. Note, that this implementation
    does not contain color, since it should be cated by font implementation
 */
#pragma once
#include "glyph.h"
#include <font.h>
#include <sadstring.h>
#include <sadsize.h>
#include <sadpoint.h>
#include <sadvector.h>

#include <texture.h>

namespace sad
{

namespace freetype
{

class FixedSizeFont
{
public:
    /*! Creates new font height
        \param[in] library a library
        \param[in] face a font face, used for rendering
        \param[in] height a requested 
     */
    FixedSizeFont(FT_Library library, FT_Face face, unsigned int height);
    /*! Frees memory, from glyphs
     */
    ~FixedSizeFont();
    /*! Renders a fixed font
        \param[in] s string
        \param[in] p upper-left point
        \param[in] ratio a ratio for line-spacing, relative to line-spacing of font
        \param[in] flags a flag value
     */
    void render(const sad::String & s, const sad::Point2D & p, float ratio, sad::Font::RenderFlags flags);
    /*! Dumps all parameters of glyphs to string
        \return string of glyph parameters
     */
    sad::String dumpGlyphParameters() const;
    /*! Renders text line to a texture. Before output all new line string are stripped.
        Texture's memory should be freed manually
        \param[in] string a string texture
        \param[in] library a library fo text
        \param[in] face a face for rendering glyphs
        \param[in] height a height for rendering
        \return rendered image
     */
    sad::Texture * renderToTexture(
        const sad::String & string,
        FT_Library library,
        FT_Face face,
        unsigned int height 
    );
    /*! Returns size for fixed height
        \param[in] s rendered string
        \param[in] ratio a ratio for line-spacing, relative to line-spacing of font
        \param[in] flags a flags
     */
    sad::Size2D size(const sad::String & s, float ratio, sad::Font::RenderFlags flags);
    /*! Returns a built-in line spacing for fixed height font
        \return built-in linespacing
     */
    inline float builtinLineSpacing() const
    {
        return m_builtin_linespacing;
    }
    /*! An ascent for font
        \return ascent for font
    */
    virtual float ascent() const;
    /*! Appends uploaded textures to GPU
     */
    void uploadedTextures(sad::Vector<unsigned int> & textures);
    /*! Marks textures as unloaded
     */
    void markTexturesAsUnloaded();
    /*! Dumps glyph to file
     */
    void dumpToBMP();
protected:
    /*! A builtin  linespacing
     */
    float m_builtin_linespacing;
    /*! A bearing y for rendering all fonts
     */
    float m_bearing_y;
    /*! Whether font glyph textures is uploaded to GPU
     */
    bool m_on_gpu;
    /*! A height for font
     */
    float m_height; 
    /*! A glyphs table
     */
    sad::freetype::Glyph * m_glyphs[256];
    /*! A table of kerning, where row is previous char 
        and column is current
     */
    float m_kerning_table[256][256];
    /*! Sets bounding box size to specified height
        \param[in] library a library
        \param[in] face a face
        \param[in] height a height for requesting in pixels
     */
    void requestSize(FT_Library library, FT_Face face, unsigned int height);
    /*! Tries to compute kerning, filling kerning table
        \param[in] face face, for which should kerning be computed
     */
    void computeKerning(FT_Face face);
};

}

}
