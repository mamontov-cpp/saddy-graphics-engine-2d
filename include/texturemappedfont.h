/*! \file texturemappedfont.h
    

    Defines a simple font, which is defined through a texture, where all glyphs
    are stored. When user loads it, it loads to file - a file of texture and
    also a file config, where texture coordinates are stored
 */
#pragma once
#include "font.h"
#include "texture.h"
#include "sadrect.h"

#include "resource/resource.h"

namespace sad
{

class Renderer;
/*! A simple font, which is defined through a texture, where all glyphs
    are stored. When user loads it, it loads to file - a file of texture and
    also a file config, where texture coordinates are stored
 */
class TextureMappedFont: public sad::Font
{
SAD_OBJECT
public:
    /*! Creates new font. This font cannot render itself, because
        all data is empty and not set
     */
    TextureMappedFont();
    /*! Kept for purpose of inheritance
     */
    virtual ~TextureMappedFont() override;
    /*! Returns a estimated size of label, rendered with specified size
        \param[in] str string
        \param[in] flags a flags for rendering
        \return size of label
     */
    virtual sad::Size2D size(const sad::String & str, sad::Font::RenderFlags flags = sad::Font::FRF_None)  override;
    /*! Renders a string on screen
        \param[in] str string
        \param[in] p   upper-left point in viewport coordinates
        \param[in] flags a flags for rendering
     */
    virtual void render(const sad::String & str,const sad::Point2D & p, sad::Font::RenderFlags flags = sad::Font::FRF_None) override;
    /*! Fills geometries with related font data
     *  \param[in] data a data
        \param[in] g geometries
        \param[in] str string
        \param[in] p   upper-left point in viewport coordinates
        \param[in] flags a flags for rendering
     */
    virtual void fillGeometries(const sad::Font::GeometryRenderData& data, sad::os::GLFontGeometries& g, const sad::String & str, const sad::Point2D & p, sad::Font::RenderFlags flags = sad::Font::FRF_None) override;
    /*! Renders a string of text to a texture. A texture memory is not freed.
        A string rendered as line, so any newline symbols are stripped from rendered string.
        \param[in] str string
        \return rendered texture
     */
    sad::Texture * renderToTexture(const sad::String & str);
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
    /*! Loads a font from files filename.png and filename.cfg.
        You can generate font, using exporter tool in plugins directory

        A font texture file 
        must contain a transparent background, with glyphs blended with white-
        colored glyphs on it.

        \param[in] filename name of file
        \param[in] r renderer, which texture is attached to. nullptr is for global renderer
        \return true on success
     */
    bool load(const sad::String & filename, sad::Renderer * r = nullptr);
    /*! Loads a font from files filename.png and filename.cfg.
        You can generate font, using exporter tool in plugins directory.

        A font texture file 
        must contain a transparent background, with glyphs blended with white-
        colored glyphs on it.

        \param[in] texturefilename name of file of texture
        \param[in] configfilename name of config file
        \param[in] r renderer, which texture is attached to. nullptr is for global renderer
        \return true on success
     */
    bool load(
        const sad::String & texturefilename,  
        const sad::String & configfilename,
        sad::Renderer * r = nullptr
    );
    /*! Unloads all resources from GPU. By default does nothing
     */
    virtual void unloadFromGPU() override;
    /*! Returns a builtin line spacing
        \return line spacing
     */
    virtual float builtinLineSpacing() const override;
    /*! An ascent for font
        \return ascent for font
     */
    virtual float ascent() const override;
    /*! Sets size of font in pixels
        \param[in] size size of font in points
     */
    virtual void setSize(unsigned int size) override;
protected:
    sad::Texture * m_texture;             //!< A texture, where all glyphs are stored
    float          m_builtin_linespacing; //!< A builtin linespacing
    float          m_size_ratio;          //!< A ratio for size
    sad::Rect2D    m_glyphs[256];         //!< A stored glyphs, as texture rectangle
    sad::Size2D    m_sizes[256];          //!< A sizes for each glyph
    int            m_leftbearings[256];   //!< Left bearings 
    int            m_rightbearings[256];  //!< Right bearings 
    float          m_ascent;              //!< A builtin ascent
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::TextureMappedFont)