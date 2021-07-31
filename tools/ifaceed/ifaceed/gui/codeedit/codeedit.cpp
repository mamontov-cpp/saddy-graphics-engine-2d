#include "codeedit.h"

#include "linenumberarea.h"

#include "highlighter.h"

#include <QScrollBar>
#include <QPainter>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QKeyEvent>

#include <algorithm>

#ifndef HAVE_QT5
    #define HAVE_QT5 (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif

gui::codeedit::CodeEdit::CodeEdit(QWidget* parent) : gui::textedit::TextEdit(parent)
{
    this->setAcceptRichText(false);

    m_line_number_area = new gui::codeedit::LineNumberArea(this);

    m_highlighter = new gui::codeedit::Highlighter(this->document());

    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    m_alphabet.insert('.');
    for(char i = 'a'; i <= 'z'; i++)
    {
        m_alphabet.insert(QChar(i));
    }
    for(char i = 'A'; i <= 'Z'; i++)
    {
        m_alphabet.insert(QChar(i));
    }
    for(char i = '0'; i <= '9'; i++)
    {
        m_alphabet.insert(QChar(i));
    }


    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

gui::codeedit::CodeEdit::~CodeEdit()
{
    delete m_line_number_area;
    m_line_number_area = nullptr;
    delete m_highlighter;
}

int gui::codeedit::CodeEdit::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated 
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't 
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void gui::codeedit::CodeEdit::lineNumberAreaPaintEvent(QPaintEvent* e)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(m_line_number_area);
    painter.fillRect(e->rect(), QColor(228,228,228));
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block 
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = static_cast<int>(this->document()->documentMargin()) - 1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block

#ifdef HAVE_QT5
        additional_margin = static_cast<int>(this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height());
#else    
        additional_margin = static_cast<int>(this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersect(this->viewport()->geometry()).height());
#endif
    // Shift the starting point
    top += additional_margin;

    int bottom = top + static_cast<int>(this->document()->documentLayout()->blockBoundingRect(block).height());

    QColor col_1(128, 0, 0);      // Current line (custom green)
    QColor col_0(128, 128, 128);    // Other lines  (custom darkgrey)

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= e->rect().bottom()) {
        if (block.isVisible() && bottom >= e->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-5, top,
                             m_line_number_area->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(this->document()->documentLayout()->blockBoundingRect(block).height());
        ++blockNumber;
    }
    
}

int gui::codeedit::CodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 +  fontMetrics().width(QLatin1Char('9')) * (digits);

    return space;
}

Qt::CaseSensitivity gui::codeedit::CodeEdit::caseSensitivity() const
{
    return Qt::CaseSensitive;
}

void gui::codeedit::CodeEdit::updateLineNumberAreaWidth(int)
{ 
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void gui::codeedit::CodeEdit::updateLineNumberArea(QRectF)
{
    updateLineNumberArea();
}
void gui::codeedit::CodeEdit::updateLineNumberArea(int)
{
    updateLineNumberArea();
}

void gui::codeedit::CodeEdit::updateLineNumberArea()
{
    if (m_line_number_area == nullptr)
    {
        return;
    }
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    m_line_number_area->update(0, rect.y(), m_line_number_area->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        m_line_number_area->scroll(0, dy);
    }

    // Addjust slider to alway see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());
}

void gui::codeedit::CodeEdit::resizeEvent(QResizeEvent *e)
{
    gui::textedit::TextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_line_number_area->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void gui::codeedit::CodeEdit::keyPressEvent(QKeyEvent* e)
{
    bool handled = false;
    if (e->key() == Qt::Key_Tab)
    {
        QTextCursor cursor = textCursor();
        if (cursor.hasSelection())
        {
            handled = true;
            if (e->modifiers() & Qt::ControlModifier)
            {
                unindentSelection(cursor);
            }
            else
            {
                indentSelection(cursor);
            }
        }        
    }
    if (!handled)
    {
        this->gui::textedit::TextEdit::keyPressEvent(e);
    }    
}

void gui::codeedit::CodeEdit::indentSelection(QTextCursor& cursor)
{    
    int startpos = cursor.selectionStart();
    QString text = cursor.selectedText();
    QStringList list = text.split(QChar::ParagraphSeparator);
    for(size_t i = 0; i < list.size(); i++)
    {
        if (list[i].size() != 0)
        {
            list[i] = "    " + list[i];
        }
    }
#if  (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    text = list.join(QChar::ParagraphSeparator);
#else
    text = list.join(QChar(QChar::ParagraphSeparator));
#endif
    cursor.insertText(text);
    cursor.setPosition(startpos),
    cursor.setPosition(startpos + text.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}

void gui::codeedit::CodeEdit::unindentSelection(QTextCursor& cursor)
{
    int startpos = cursor.selectionStart();
    QString text = cursor.selectedText();
    QStringList list = text.split(QChar::ParagraphSeparator);
    for(size_t i = 0; i < list.size(); i++)
    {
        if (list[i].startsWith("\t"))
        {
            list[i] = list[i].mid(1);
        }
        if (list[i].startsWith("    "))
        {
            list[i] = list[i].mid(4);
        }
    }
#if  (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    text = list.join(QChar::ParagraphSeparator);
#else
    text = list.join(QChar(QChar::ParagraphSeparator));
#endif
    cursor.insertText(text);
    cursor.setPosition(startpos),
    cursor.setPosition(startpos + text.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}

QString  gui::codeedit::CodeEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    int position = tc.position();
    const QString& me  = this->toPlainText();
    QString result  = "";
    int i = position-1;
    bool valid = true;
    while(valid && i >= 0 && i < me.length())
    {
        if (m_alphabet.contains(me[i]))
        {
            result += me[i];
        }
        else
        {
            valid = false;
        }
        i--;
    }

    std::reverse(result.begin(), result.end());
    if (position < me.length())
    {
        if (m_alphabet.contains(me[position]) == false && me[position] != '\n' && me[position] != '\r')
        {
            result += me[position];
        }
    }
    if (result.startsWith("E") == false && result.startsWith("co") == false)
    {
        result = this->gui::textedit::TextEdit::textUnderCursor();
    }
    return result;
}

int gui::codeedit::CodeEdit::minCompletionPrefixLength() const
{
    return 2;
}

void gui::codeedit::CodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        
        QColor lineColor = QColor(232, 232, 255);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
