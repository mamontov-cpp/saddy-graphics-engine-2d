/*! \file   freetype/font.h
    \author HiddenSeeker
	\brief  Contains an implementation of font, which is loaded and rendered via FreeType.
	Here is placed an implementation of freetype font, based on NeHe Tutorial for 
	MSVC by Sven Olsen,2003
*/
#pragma once
#include "../font.h"

namespace sad
{

namespace freetype
{

class FontImpl;

/** A font, which is loaded and 
	rendered via Freetype fonts library
 */
class Font: public sad::Font
{
public:
	/*! Creates an empty font
	 */
	Font();
	/*! Loads a font
	    \param[in] filename a path to TTF or other files, to be loaded via freetype
	 */
	virtual bool load(const sad::String & filename);
	/*! Renders a string
		\param[in] str string
		\param[in] p   upper-left point in window coordinates
	 */
	virtual void render(const sad::String & str,const sad::Point2D & p);
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str);
	/*! Returns a builtin line spacing
		\return line spacing
	 */
	virtual float builtinLineSpacing() const;
	/*! Destructor
	 */
	virtual ~Font();
private:
	sad::freetype::FontImpl * m_dptr; //!< A private implementation of font
};

}

}
