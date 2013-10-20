/*! \file  font.h
	\author HiddenSeeker

	Defines a basic class, for all renderable fonts in Saddy engine
 */
#pragma once
#include "sadpoint.h"
#include "sadsize.h"
#include "sadstring.h"

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
	virtual void setSize(unsigned int size) = 0;
	/*! Kept for purpose of inheritance
	 */
	virtual ~Font();
protected:
	/*! Size of font in pixels
	 */
	unsigned int m_size;
};

}
