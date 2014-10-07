/*!	\file fontmanager.h
	\author Kolesov Dmitiy, HiddenSeeker
	\brief Declaration of FontManager class

	This file contains declaration of FontManager class, which holds all fonts, 
	registered in saddy
*/
#pragma once
#include "font.h"
#include "sadmutex.h"
#include "sadptrhash.h"
#include "sadstring.h"


namespace sad
{
/*!	\class FontManager
	Keeps a collection of fonts, and provides accessing interface
 */
class FontManager
{
public:
	FontManager();
	/*! Instance of font manager
		\return global instance of font manager, registered in renderer
	 */
	static FontManager * ref();
	/*! Adds a font and associates a key to it. If some font exists already
		in manager, it is removed and freed, and new font replaces it
		\param[in] font font pointer
		\param[in] name associated name
	 */
	void add(sad::Font * font,const sad::String & name );
	/*!	Returns a font from container by it's registered name 
		\param[in] name associated name of font
		\return pointer to font object (NULL if not fount)
	 */
	sad::Font * get(const sad::String & name);
	/*!	Deletes a font from container, freeing it by memory if exists
		\param[in] name of font
	 */
	void remove(const sad::String &name);
private:
	/*! A font collection
	 */
	sad::PtrHash<sad::String, sad::Font> m_fonts;
	/*! Mutex, which is blocked, when collection is changed
	 */
	::sad::Mutex	m_m;    
	/*! Copying is DISABLED
		\param[in] o manager
	 */
	FontManager(const FontManager & o);
	/*! Copying is DISABLED
		\param[in] o manager
		\return self-reference
	 */
	FontManager & operator=(const FontManager & o);
};

}
