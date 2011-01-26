#include "fontmanager.h"

#ifdef WIN32

/*! Destructor free loaded Win32Font
*/
Win32Font::~Win32Font() {
	glDeleteLists(m_Win32Font, 96);
}
/*!	Default constructor, no load anything
*/
Win32Font::Win32Font() {
	m_Win32Font_loaded = false;
}
/*! Copying constructor, copy all properties of getting object
	\param[in] other object for copying, this object will be copy of other
*/
Win32Font::Win32Font(Win32Font &other) {
	m_Win32Font_loaded = other.m_Win32Font_loaded;
	m_Win32Font = other.m_Win32Font;
	m_hdc = other.m_hdc;
}
/*! Constructor load Win32Font and size
	\param[in] name name (and path, if need) of file with true type Win32Font
*/
Win32Font::Win32Font(hString &name, int size) {
	loadWin32Font(name, size);
}

/*!	Default constructor, no load anything
	\param hdc HDC
*/
Win32Font::Win32Font(HDC hdc) {
	m_hdc = hdc;
}
/*! Constructor load Win32Font and size
	\param[in] file_name name (and path, if need) of file with true type Win32Font
	\param hdc HDC
*/
Win32Font::Win32Font(hString &name, int size, HDC hdc) {
	m_hdc = hdc;
	loadWin32Font(name, size);
}

/*! function load Win32Font from file with name
	\param[in] name name of file with Win32Font
	\return Win32Font pointer
*/
void Win32Font::loadWin32Font(hString &name, int size) {
	tfont temp_Win32Font = CreateFontA(
		-size, // size of font
		0, // width
		0, // angle ratio
		0, // angle
		FW_NORMAL, //boldness
		false, // is italic
		false, // is underlined
		false, // is stroked
		ANSI_CHARSET, // charset
		OUT_TT_PRECIS, // prefed ttf
		CLIP_DEFAULT_PRECIS, // clipping precision
		ANTIALIASED_QUALITY, // antialiased quality
		FF_DONTCARE|DEFAULT_PITCH, // family and step
		name.data() // name of font
		);
	m_Win32Font = glGenLists(96);
	SelectObject(m_hdc, temp_Win32Font);// select the font
	wglUseFontBitmaps(m_hdc, 32, 96, m_Win32Font);// build lists
}

#endif
