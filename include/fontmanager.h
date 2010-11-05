/*!	\file fontmanager.h
	\author Kolesov Dmitiy
	\brief Declaration of FontManager class

	This file contain declaration of FontManager class, which work with font
	load, render and free it.
*/

#ifndef FONTMANAGER_H__
#define FONTMANAGER_H__
#include "types.h"
#include "SDL.h"
#include "SDL_TTF.h"

/*!	\class This class work with one font: load, render and free it.
	Also class get and keep settings for render, size, colour, place...
*/
class FontManager
{
public:
	/*! Destructor free loaded font
	*/
	~FontManager();
	/*!	Default constructor, no load anything
	*/
	FontManager();
	/*! Copying constructor, copy all properties of getting object
		\param[in] other object for copying, this object will be copy of other
	*/
	FontManager(FontManager &other);
	/*! Constructor load font and size, no load settings.
		\param[in] file_name name (and path, if need) of file with true type font
		\param[in] size size of font
	*/
	FontManager(hString &file_name, int size);
	/*!	Constructor load font and settings.
		\param[in] file_name name (and path, if need) of file with true type font
		\param[in] size size of font
		\param[in] colour color of font
		\param[in] place place on surface, for putting text
	*/
	FontManager(hString &filename, int size, hColor color, hRectF place);
	/*!	function set setting for font, independent of existing previous settings
		\param[in] colour colour of font
		\param[in] place place on surface, for putting text
	*/
	void setSettings(int colour, hRectF place);
	/*!	function free current font and load new font, with settings
	\param[in] file_name name (and path, if need) of file with true type font
		\param[in] size size of font
		\param[in] colour color of font
		\param[in] place place on surface, for putting text
	*/
	void reloadFont(hString &filename, int size, hColor color, hRectF place);
	/*!	function return last error in human redable form
		\return last error as string
	*/
	hString getLastError();
	/*!	function put text on surface of screen
		\param screen surface of screen
		\param text text for putting
	*/
	void putText(SDL_Surface screen, hString text);

protected:
	hString m_last_error;///<last error in human readable form
	hColor color;///<saved color for text
	hRectF place;///<saved place for putting text
	TTF_Font *font;///<loaded font
};

#endif //FONTMANAGER_H__
	