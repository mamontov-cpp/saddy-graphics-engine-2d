/*! \file  font.h
	\author HiddenSeeker

	Defines a basic class, for all renderable fonts in Saddy engine
 */
#pragma once
#include "sadpoint.h"
#include "sadsize.h"
#include "sadstring.h"
#include "sadcolor.h"

namespace sad
{

/*! A basic class for font, which renders all fonts in Saddy engine
 */ 
class Font
{
public:
	/*! Constructs new font with size of 14px
	 */
	Font();
	/*! Renders a string on screen
	    \param[in] str string
		\param[in] p   upper-left point in viewport coordinates
	 */
	virtual void render(const sad::String & str,const sad::Point2D & p) = 0; 
	/*! Sets a color, which label will be rendered with, if label is rendered,
		using this font
		\param[in] color color of font
	 */
	virtual void setColor(const sad::AColor & c);
	/*! Returns a color, which label will be rendered with, if label is rendered,
		using this font
		\return color
	 */
	virtual sad::AColor color() const; 
	/*! Returns a estimated size of label, rendered with specified size
	    \param[in] str string
		\return size of label
	 */
	virtual sad::Size2D size(const sad::String & str) = 0;
	/*! Returns size of font in pixels
		\return size of font in pixels
	 */
	inline unsigned int size() const
	{
		return m_size;
	}
	/*! Sets size of font in pixels
		\param[in] size size of font in points
	 */
	virtual void setSize(unsigned int size);
	/*! Kept for purpose of inheritance
	 */
	virtual ~Font();
protected:
	/*! Size of font in pixels
	 */
	unsigned int m_size;
	/*! A color of font
	 */
	sad::AColor m_color;
	/*! A buffer, used by fonts to store current color
	 */
	int m_current_color_buffer[4]; 
	/*! Sets current OpenGL state rendering color to an inner color
	 */
	void setCurrentColor();
	/*! Restores OpenGL state rendering color
	 */
	void restoreColor();
};

}
