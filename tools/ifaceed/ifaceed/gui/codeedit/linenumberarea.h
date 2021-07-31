/*! \file linenumberarea.h
    

    A part of code editor, which displays line numbers,  based on both examples of rich editor and other
    http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
    http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
 */
#pragma once
#include "../textedit/textedit.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QPaintEvent>

class QCompleter;

namespace gui
{

namespace codeedit
{
class CodeEdit;
/*! A part of code editor, which displays line numbers based on both examples of rich editor and other
    http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
    http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
 */
class LineNumberArea : public QWidget
{
public:
    /*! Constructs a text edit widget
        \param[in] editor an editor
     */ 
    LineNumberArea(gui::codeedit::CodeEdit* editor);
     /*! Destructor for widget
      */
    virtual ~LineNumberArea() override;
    /*! Returns size hint for line number
        \return size hint
     */
    QSize sizeHint() const override;
protected:
    /*! A rendering event for widget
        \param[in] e event
     */
    void paintEvent(QPaintEvent* e) override;
    /*! A code editor
     */
    gui::codeedit::CodeEdit* m_code_editor;
};

}

}
