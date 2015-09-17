#include "linenumberarea.h"
#include "codeedit.h"

gui::codeedit::LineNumberArea::LineNumberArea(gui::codeedit::CodeEdit* editor) : QWidget(editor)
{
    m_code_editor = editor;
}

gui::codeedit::LineNumberArea::~LineNumberArea()
{
    
}

QSize gui::codeedit::LineNumberArea::sizeHint() const 
{
    return QSize(m_code_editor->lineNumberAreaWidth(), 0);
}

void gui::codeedit::LineNumberArea::paintEvent(QPaintEvent* e) 
{
    m_code_editor->lineNumberAreaPaintEvent(e);
}
