/*!	\file fontmanager.h
	\author Kolesov Dmitiy, HiddenSeeker
	\brief Declaration of FontManager class

	This file contain declaration of FontManager class, which work with font
	load, render and free it.
*/
#pragma once
#include "types.h"
#include "texturemanager.h"
#include "renderer.h"

namespace sad
{
class Renderer;

/*! Basic font class
*/
class BasicFont
{
 private:
 public:
	 /*! Renders a string
	     \param[in] str string
		 \param[in] p   upper-left point in window coordinates
	 */
	 virtual void render(const hst::string & str,const pointf & p)=0; 
	 /*! Returns a estimated size of label
	     \param[in] str string
	 */
	 virtual hRectF size(const hst::string & str)=0;
	 /*! Destroys an exemplar
	 */
	 virtual ~BasicFont();
};


}

namespace sad
{
	/*! Class of texture mapped font. 
	    This class contains a texture, where are printed all glyphs, and a bunch of rects
		where are placed texture coordinated. Hint: use Texture::enable to restore previous
		state
	*/
	class TMFont: public BasicFont
	{
		private:
			sad::Texture * m_tex;         //!< Current texture
			pointf       m_ul[255];     //!< Glyph rectangles (upper left)
			pointf       m_lr[255];     //!< Glyph rectangles (lower right)

			/*! Determines a background and text and sets alpha component.
			    Uses Luv model to determine it.
			*/
			void alphaDetermine();
	    public:
			/*! Empty font
			*/
			TMFont();
			/*! Loads a font from file
			    \param[in] tex texture file
				\param[in] cfg mapping file (see examples/times_red.cfg for format)
				\param[in] bk  background texture color
				\param[in] fontdetermine determines an alpha or not?
			*/
			bool load(
			           const hst::string & tex, 
					   const hst::string & cfg, 
					   const hst::color & bk=hst::color(255,255,255),
					   bool fontdetermine=true,
					   sad::Renderer * renderer = sad::Renderer::ref() 
					  );
			/*! Renders a string
				\param[in] str string
				\param[in] p   upper-left point in window coordinates
			*/
	        void render(const hst::string & str,const pointf & p);
			/*! Returns a estimated size of label
				\param[in] str string
			*/
			hRectF size(const hst::string & str);
			/*! Destroys a file
			*/
			~TMFont();
	};
}


namespace sad
{
	/*!	\class FontManager
		Keeps a collection of fonts, and provides accessing interface
	*/
	class FontManager
	{
	 private:
			 hst::hash<hst::string, sad::BasicFont *> m_fonts;
			 os::mutex                                m_m;    //!< Mutex to block side effects

			 FontManager(const FontManager & o);
			 FontManager & operator=(const FontManager & o);
	 public:
			FontManager();
			/*! Instance of font manager
			 */
			static FontManager * ref();
		    /*! Adds a font and associates a key to it
			    \param[in] font font pointer
				\param[in] name associated name
			*/
		    void add(sad::BasicFont * font,const hst::string & name );
			/*!	Returns an object by a key
				\param[in] key associated key
				\return pointer to font object
			*/
		    sad::BasicFont * get(const hst::string & key);
		    /*!	Deletes a font by a key
			    \param[in] key 
		    */
			void remove(const hst::string &key);
	     	/*! Destructor
		    */
			~FontManager();
	};

}


