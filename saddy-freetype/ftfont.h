/*! \file   ftfont.h
    \author HiddenSeeker
	\brief  Contains an implementation of freetype font.
	Here is placed an implementation of freetype font, based on NeHe Tutorial for MSVC by Sven Olsen,2003
*/
#include "../include/fontmanager.h"
#include "../include/primitives/hcolor.h"
#ifdef WIN32
    #include <windows.h> 
	#include <GL/gl.h>
	#include <GL/glu.h>
#else
        #include <GL/gl.h>
	#include <GL/glu.h>
#endif
#pragma once

namespace coord_system
{
/*! Switches to window coordinate system
*/
void switchToWCS();

/*! Restores default cs
*/
void restore();
}

class FTFont: public sad::BasicFont
{
 private:
  	float       m_height;   //!< Height of the font
	GLuint *    m_texs;	    //!< Textures
	GLuint      m_base;	    //!< First id
	hst::acolor m_cl;       //!< Current color
	float       m_w[256];   //!< Widths
 public:
	 inline void setColor(const hst::acolor & cl) {m_cl=cl;} //!< Sets a color
     /*! Empty font
	 */
     FTFont();
	 /*! Loads a font
	     \param[in] fnt_file file of TTF font
		 \param[in] height  height of font
		 \param[in] cl      color
	 */
	 bool load(const char * fnt_file, unsigned int height,const hst::acolor & cl=hst::acolor(0,0,0,0));
	 /*! Renders a string
	     \param[in] str string
		 \param[in] rect defines an upper left corner of font
		 \param[in] z not used
	 */
	 void render(const hst::string & str,const hRectF & rect,float z);
	 /*! Same as other, but little faster
	     \param[in] str string
		 \param[in]  x x coordinate
		 \param[in]  y y coordinate
     */
	 void render(const hst::string & str, float x, float y);
	 /*! Currently returns only a height of font
	     \param[in] str string
	 */
	 hRectF size(const hst::string & str);
	 /*! Destructor
	 */
	 ~FTFont();
};

