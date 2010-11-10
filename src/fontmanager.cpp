//Димон, в твоём контейнере hhash присутствуют ошибки! ИМХО
//Обрати внимание на стр 375 в файле hhash_src.hpp, например
//из-за этих ошибок не комплируется мой FontManager.
//Если я неправ, разъясни в чём.
//Класс FontManager готов, вроде бы, но не компилируется из-за ошибок в хэш контейнере.
#include "fontmanager.h"

/*	function create font object and return pointer
	\param[in] file_name name of file with font, it will be key for this font object
	\return pointer to font object
*/
pFont FontManager::createFont(hString &file_name) {
	pFont temp;
	if (!m_fonts.contains(file_name)) {
		temp =  new Font(file_name);
		m_fonts.insert(file_name, temp);
	} else {
		temp = NULL;
	}
	return temp;

}
/*	function return font object with key
	\param[in] key key for object for returning
	\return pointer to font object
*/
pFont FontManager::getFont(hString &key) {
	pFont result = NULL;
	if (m_fonts.contains(key)) {
		result = m_fonts[key];
	}
	return result;
}
/*	function delete font object
	\param[in] key key of deleting object
*/
void FontManager::removeFont(hString &key) {
	if (m_fonts.contains(key)) {
		m_fonts.remove(key);
	}
}
/* destructor delete all fonts object keeping in this manager
*/
FontManager::~FontManager() {
	pFont temp;
	hhash<hString, Font*>::iterator font_iterator;
	font_iterator = m_fonts.begin();
	while (font_iterator != m_fonts.end()) {
		temp = font_iterator.value();
		delete temp;
		++font_iterator;
	}
}

//Чтобы менеджер не ругался на анрезолвед экстерналы следующие функции описаны пустотельно, временно.

/*! Destructor free loaded font
*/
Font::~Font() {}
/*!	Default constructor, no load anything
*/
Font::Font() {}
/*! Copying constructor, copy all properties of getting object
	\param[in] other object for copying, this object will be copy of other
*/
Font::Font(Font &other) {}
/*! Constructor load font and size
	\param[in] file_name name (and path, if need) of file with true type font
*/
Font::Font(hString &file_name) {}