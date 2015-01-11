/*! \file codedit.h
	\author HiddenSeeker

	A code editor, based on both examples of rich editor and other
	http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
	http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
 */
#pragma once
#include "../textedit/textedit.h"

#include <QResizeEvent>
#include <QSyntaxHighlighter>

namespace gui
{

namespace codeedit
{

/*!A code editor, based on both examples of rich editor and other
	http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
	http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
	with patch from http://stackoverflow.com/questions/2443358/how-to-add-lines-numbers-to-qtextedit/24596246#24596246
 */
class CodeEdit : public gui::textedit::TextEdit
{
Q_OBJECT
public:
	/*! Constructs a text edit widget
		\param[in] parent a parent widget
	 */ 
	CodeEdit(QWidget* parent = 0);
     /*! Destructor for widget
	  */
	virtual ~CodeEdit();
	/*! Gets first visible block 
		\return first block id
	 */
	int getFirstVisibleBlockId();
	/*! An event for handling painting line number area
		\param[in] e event
	 */ 
	void lineNumberAreaPaintEvent(QPaintEvent* e);
	/*! Returns width for line number area
		\return line number width
	 */
    int lineNumberAreaWidth();
protected:
	/*! Handles resize event
		\param[in] event
	 */
    void resizeEvent(QResizeEvent* e);
private slots:
	/*! Updates line number width
		\param[in] newBlockCount new block count
	 */
    void updateLineNumberAreaWidth(int newBlockCount);
	/*! Updates line number area
		\param[in] rect rectangle
	 */
	void updateLineNumberArea(QRectF rect);
	/*! Updates line number area for position
		\param[in] pos position of slider
	 */
    void updateLineNumberArea(int  pos);
	/*! Updates line number area
	 */
    void updateLineNumberArea();  
	/*! Highights current line
	 */
	void highlightCurrentLine();
private:
	/*! A line number area to work with
	 */
	QWidget* m_line_number_area;
	/*! A highlighter element
	 */
	QSyntaxHighlighter* m_highlighter;
};

}

}
