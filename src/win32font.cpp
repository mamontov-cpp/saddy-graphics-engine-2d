#include "fontmanager.h"


#ifndef WIN32
#error   "Why you are compiling it, dumbass?"
#endif

//Чтобы менеджер не ругался на анрезолвед экстерналы следующие функции описаны пустотельно, временно.

/*! Destructor free loaded Win32Font
*/
Win32Font::~Win32Font() {
	glDeleteLists(m_Win32Font, 96);//удалить все 96 списков отображения, т.е. загруженных символов
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
#ifdef WIN32
	m_hdc = other.m_hdc;
#endif
}
/*! Constructor load Win32Font and size
	\param[in] name name (and path, if need) of file with true type Win32Font
*/
Win32Font::Win32Font(hString &name, int size) {
	loadWin32Font(name, size);
}
#ifdef WIN32
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
#endif
/*! function load Win32Font from file with name
	\param[in] name name of file with Win32Font
	\return Win32Font pointer
*/
void Win32Font::loadWin32Font(hString &name, int size) {
#ifdef WIN32
	tfont temp_Win32Font = CreateFontA(
		-size, //Размер шрифта, знак минус означает: считать размер символа а не ячейки
		0, //ширина шрифта
		0, //угол отношения
		0, //угол наклона
		FW_NORMAL, //жирность шрифта
		false, //курсив
		false, //подчеркнуты
		false, //зачеркнутый
		ANSI_CHARSET, //идентификатор набора символов
		OUT_TT_PRECIS, //предпочитать ttf шрифты, если есть несколько с одним именем
		CLIP_DEFAULT_PRECIS, //точность отсечения
		ANTIALIASED_QUALITY, //кач-во вывода
		FF_DONTCARE|DEFAULT_PITCH, //семейство и шаг
		name.data() //имя шрифта
		);
	m_Win32Font = glGenLists(96);
	SelectObject(m_hdc, temp_Win32Font);//выбор этого фонта
	wglUseFontBitmaps(m_hdc, 32, 96, m_Win32Font);//Построить списки отображения для 96 символов начиная с пробела
#endif
}