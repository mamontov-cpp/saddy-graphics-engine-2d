/*!	\file fontmanager.h
	\author Kolesov Dmitiy, 
	\brief Declaration of font class

	This file contain declaration of font class, which work with font
	load, render and free it.
*/

/*!	\class This class work with one font: load, render and free it.
	Also class get and keep settings for render, size, colour, place...
*/
class Win32Font: public sad::BasicFont
{
public:
	/*! Destructor free loaded Win32Font
	*/
	~Win32Font();
	/*!	Default constructor, no load anything
	*/
	Win32Font();
	/*! Copying constructor, copy all properties of getting object
		\param[in] other object for copying, this object will be copy of other
	*/
	Win32Font(Win32Font &other);
	/*! Constructor load Win32Font and size
		\param[in] name name (and path, if need) of file with true type Win32Font
	*/
	Win32Font(hString &name, int size);
	/*!	function free current Win32Font and load new Win32Font
	\param[in] name name (and path, if need) of file with true type Win32Font
	*/
#ifdef WIN32
	/*!	Default constructor, no load anything
		\param hdc HDC
	*/
	Win32Font(HDC hdc);
	/*! Constructor load Win32Font and size
		\param[in] name name (and path, if need) of file with true type Win32Font
		\param hdc HDC
	*/
	Win32Font(hString &name, int size, HDC hdc);
#endif
	void reloadWin32Font(hString &filename);
	/*!	function return last error in human redable form
		\return last error as string
	*/
	hString getLastError();
	/*!	function create texture with text and return it
		\param[in] text text to drawing on texture
		\param[in] size size of Win32Font
		\param[in] color color of text
		\param[out] lenght lenght of texture
		\param[out] height height of texture
		\return texture with drawing text as GLuint*
	*/
	GLuint *drawText(hString &text, int size, hColor color, int *lenght, int *height);

protected:
	/*! function load Win32Font from file with name
		\param[in] name name of file with Win32Font
		\return Win32Font pointer
	*/
	void loadWin32Font(hString &name, int size);
	hString m_last_error;///<last error in human readable form
	GLuint m_Win32Font;///<loaded Win32Font
	bool m_Win32Font_loaded;
#ifdef WIN32
	HDC m_hdc;
#endif
};
