/*! \file   freetype/font.h
    \author HiddenSeeker
	\brief  Contains an implementation of font, which is loaded and rendered via FreeType.
	Here is placed an implementation of freetype font, based on NeHe Tutorial for MSVC by Sven Olsen,2003
*/
#pragma once
#include "../font.h"
#include "sadcolor.h"
#include "sadhash.h"

namespace sad
{

namespace freetype
{

class FontImpl;
/** A class, that encapsulates an information for font
 */
struct FTFontInfo;

/** A class of freetype font
 */
class Font: public sad::Font
{
 public:
	 /*! Empty font
	  */
     Font();
	 /*! Sets height of color in pixels
	  */
	 bool setHeight(unsigned int height);   
	 /*! Sets color for a font
	  */
	 void setColor(const sad::AColor & cl);
	 /*! Loads a font
	     \param[in] fnt_file file of TTF font
		 \param[in] height  height of font
		 \param[in] cl      color
	 */
	 virtual bool load(const char * fnt_file, unsigned int height,const sad::AColor & cl=sad::AColor(0,0,0,0));
	 /*! Renders a string
		 \param[in] str string
		 \param[in] p   upper-left point in window coordinates
	 */
	 virtual void render(const sad::String & str,const sad::Point2D & p);
	 /*! Same as other, but little faster
	     \param[in] str string
		 \param[in]  x x coordinate
		 \param[in]  y y coordinate
     */
	 virtual void render(const sad::String & str, float x, float y);
	 /*! Currently returns only a height of font
	     \param[in] str string
	 */
	 virtual sad::Rect2D size(const sad::String & str);
	 /*! Destructor
	 */
	 virtual ~Font();
 private:
	/** Structure, that represents data, needed for rendering font with specified height
	 */
	struct FTHeightFont
	{
	 public:
		GLuint *     m_texs;	    //!< Textures
		GLuint       m_base;	    //!< First id
		float        m_w[256];      //!< Widths
		float        m_height;      //!< Height of font
	};
	/** Creates a new empty structure. To be rendered, structure must be populated in
		\see FTFont::buildFont()
		\param[in] height of font
		\return new font
	 */
	FTHeightFont * newFTHeightFont(unsigned int height);
	/** Deleted a generated height font
		\param[in] fnt font data
	 */
	void deleteFTHeightFont(FTHeightFont * fnt);
	/** Computes bounding box of specified height font
		\param[in] fnt    font
		\param[in] height height
		\param[in] str    string
		\return bounding box
	 */
	sad::Rect2D sizeOfFont(FTHeightFont * fnt, unsigned int height, const sad::String & str);
	/** Renders a specified box with height
		\param[in] fnt   font
		\param[in] height height
		\param[in] str    string
		\param[in] x      x coordinate
		\param[in] y      y coordinate
	 */
	void renderWithHeight(FTHeightFont * fnt, 
						  unsigned int height, 
						  const sad::String & str, 
						  float x, float y
						 );
	/** Destroys created face
	 */
	void shutdownFTFace();
	/** Cleanups a height container
	 */
	void cleanupHeightContainer();
	/** Builds height font for data
		\param[in] height height of font
		\return whether it was successfull
	 */
	bool buildHeightFont(unsigned int height);
	/** A key is a height of font, and container - is structure, needed to render that height
		so, when we needed to render it, all we need to get info and call render or size for it
	 */
	typedef  sad::Hash<unsigned int, 
					   FTFont::FTHeightFont *
					  > HeightContainer;
 private:
	sad::freetype::FontImpl * m_impl; //!< Implementation of font

	FTFontInfo *    m_info;          //!< Info about used freetype data
	unsigned int    m_renderheight;  //!< Current rendered height
	sad::AColor     m_rendercolor;   //!< Current rendered color
	HeightContainer m_lists_cache;   //!< Different created faces for font
};

}

}
