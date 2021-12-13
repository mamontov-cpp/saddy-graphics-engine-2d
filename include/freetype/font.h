// ReSharper disable CppDoxygenUnresolvedReference
/*! \file   freetype/font.h
    \brief  Contains an implementation of font, which is loaded and rendered via FreeType.
    Here is placed an implementation of freetype font, based on NeHe Tutorial for 
    MSVC by Sven Olsen,2003
*/
#pragma once
#include "../font.h"
#include "../resource/resource.h"

#include <texture.h>

namespace sad
{
class Renderer;

namespace freetype
{

class FontImpl;

/** A font, which is loaded and 
    rendered via Freetype fonts library
 */
class Font: public sad::Font
{
SAD_OBJECT
public:
    /*! Creates an empty font
     */
    Font();
    /*! Loads a font from specified file, using specified renderer for building mip maps.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (nullptr if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    ) override;
    /*! Loads a font
        \param[in] filename a path to TTF or other files, to be loaded via freetype
     */
    virtual bool load(const sad::String & filename);
    /*! Renders a string
        \param[in] str string
        \param[in] p   upper-left point in window coordinates
        \param[in] flags a flag value
     */
    virtual void render(const sad::String & str,const sad::Point2D & p, int flags = 0)  override;
    /*! Fills geometries with related font data
     *  \param[in] data a data
     *  \param[in] g geometries
     *  \param[in] str string
     *  \param[in] p   upper-left point in viewport coordinates
     *  \param[in] flags a flags for rendering
     */
    virtual void fillGeometries(const sad::Font::GeometryRenderData& data, sad::os::GLFontGeometries& g, const sad::String & str, const sad::Point2D & p, int flags = 0) override;
    /*! Renders text line to a texture. Before output all new line string are stripped.
        Texture's memory should be freed manually
        \param[in] string a string texture
        \param[in] height a height for rendering
        \return rendered image
     */
    sad::Texture * renderToTexture(
        const sad::String & string,
        unsigned int height 
    ) const;
    /*! Returns a estimated size of label, rendered with specified size
        \param[in] str string
        \param[in] flags a flag value
        \return size of label
     */
    virtual sad::Size2D size(const sad::String & str, int flags = 0)  override;
    /*! An ascent for font
        \return ascent for font
     */
    virtual float ascent() const override;
    /*! Returns a builtin line spacing
        \return line spacing
     */
    virtual float builtinLineSpacing() const override;
    /*! Unloads all resources from GPU. By default does nothing
     */
    virtual void unloadFromGPU() override;
    /*! Destructor
     */
    virtual ~Font() override;
    /*! Dumps all parameters of glyphs to string
        \return string of glyph parameters
     */
    sad::String dumpGlyphParameters() const;
    /*! Sets minimal font size for cache
        \param[in] size size for minimal cache for font
     */
    static void setMinCacheFontSize(unsigned int size);
    /*! Sets maximum font size for cache
        \param[in] size size for maximum cache for font
     */
    static void setMaxCacheFontSize(unsigned int size);
private:
    sad::freetype::FontImpl * m_dptr; //!< A private implementation of font
    sad::Renderer * m_renderer; //!<  A stored renderer for a font
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::freetype::Font)
