/*! \file texturemappedfont.h
	\author HiddenSeeker

	Defines a simple font, which is defined through a texture, where all glyphs
	are stored. When user loads it, it loads to file - a file of texture and
	also a file config, where texture coordinates are stored
 */
#pragma once
#include "font.h"
#include "texture.h"
#include "sadrect.h"

namespace sad
{

class Renderer;
/*! A simple font, which is defined through a texture, where all glyphs
	are stored. When user loads it, it loads to file - a file of texture and
	also a file config, where texture coordinates are stored
 */
class TextureMappedFont: public sad::Font
{
public:
	/*! Creates new font. This font cannot render itself, because
		all data is empty and not set
	 */
	TextureMappedFont();
	/*! Kept for purpose of inheritance
	 */
	virtual ~TextureMappedFont();
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str);
	/*! Renders a string on screen
	    \param[in] str string
		\param[in] p   upper-left point in viewport coordinates
	 */
	virtual void render(const sad::String & str,const sad::Point2D & p);
	/*! Loads a font from files filename.png and filename.cfg.
		You can generate font, using exporter tool in plugins directory

		A font texture file 
		must contain a transparent background, with glyphs blended with white-
		colored glyphs on it.

		\param[in] filename name of file
		\param[in] r renderer, which texture is attached to. NULL is for global renderer
		\return true on success
	 */
	bool load(const sad::String & filename, sad::Renderer * r = NULL);
	/*! Loads a font from files filename.png and filename.cfg.
		You can generate font, using exporter tool in plugins directory.

		A font texture file 
		must contain a transparent background, with glyphs blended with white-
		colored glyphs on it.

		\param[in] texturefilename name of file of texture
		\param[in] configfilename name of config file
		\param[in] r renderer, which texture is attached to. NULL is for global renderer
		\return true on success
	 */
	bool load(
		const sad::String & texturefilename,  
		const sad::String & configfilename,
		sad::Renderer * r = NULL
	);

protected:
	sad::Texture * m_texture;       //!< A texture, where all glyphs are stored
	sad::Rect2D    m_glyphs[256];   //!< A stored glyphs, as texture rectangle
	sad::Size2I    m_sizes[256];    //!< Size of each glyph 
	sad::Size2I    m_typical_glyph; //!< A glyph for A letter 
};

}
