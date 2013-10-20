/*!	\file fontmanager.h
	\author Kolesov Dmitiy, HiddenSeeker
	\brief Declaration of FontManager class

	This file contain declaration of FontManager class, which work with font
	load, render and free it.
*/
#pragma once
#include "sadmutex.h"
#include "types.h"
#include "texturemanager.h"
#include "renderer.h"
#include "font.h"

namespace sad
{
class Renderer;
	/*! Class of texture mapped font. 
	    This class contains a texture, where are printed all glyphs, and a bunch of rects
		where are placed texture coordinated. Hint: use Texture::enable to restore previous
		state
	*/
class TMFont: public sad::Font
	{
		private:
			sad::Texture * m_tex;         //!< Current texture
			sad::Point2D       m_ul[255];     //!< Glyph rectangles (upper left)
			sad::Point2D       m_lr[255];     //!< Glyph rectangles (lower right)

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
				\param[in] renderer linked renderer
			*/
			bool load(
			           const sad::String & tex, 
					   const sad::String & cfg, 
					   const sad::Color & bk=sad::Color(255,255,255),
					   bool fontdetermine=true,
					   sad::Renderer * renderer = sad::Renderer::ref() 
					  );
			/*! Renders a string
				\param[in] str string
				\param[in] p   upper-left point in window coordinates
			*/
	        void render(const sad::String & str,const sad::Point2D & p);
			/*! Returns a estimated size of label
				\param[in] str string
			*/
			sad::Rect2D size(const sad::String & str);
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
			 sad::Hash<sad::String, sad::BasicFont *> m_fonts;
			 ::sad::Mutex                                m_m;    //!< Mutex to block side effects

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
		    void add(sad::BasicFont * font,const sad::String & name );
			/*!	Returns an object by a key
				\param[in] key associated key
				\return pointer to font object
			*/
		    sad::BasicFont * get(const sad::String & key);
		    /*!	Deletes a font by a key
			    \param[in] key 
		    */
			void remove(const sad::String &key);
	     	/*! Destructor
		    */
			~FontManager();
	};

}


