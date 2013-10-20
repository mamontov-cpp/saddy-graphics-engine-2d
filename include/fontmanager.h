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
/*!	\class FontManager
	Keeps a collection of fonts, and provides accessing interface
 */
class FontManager
{
private:
	sad::Hash<sad::String, sad::Font *> m_fonts;
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
	void add(sad::Font * font,const sad::String & name );
	/*!	Returns an object by a key
		\param[in] key associated key
		\return pointer to font object
	 */
	sad::Font * get(const sad::String & key);
	/*!	Deletes a font by a key
		\param[in] key 
	 */
	void remove(const sad::String &key);
	/*! Destructor
	 */
	~FontManager();
};

}
