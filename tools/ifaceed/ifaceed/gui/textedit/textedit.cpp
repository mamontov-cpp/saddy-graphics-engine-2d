#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>

gui::textedit::TextEdit::TextEdit(QWidget *parent)
 : QTextEdit(parent), m_completer(0)
{
     this->setAcceptRichText(false);
     connect(&m_editing_finished_timer, SIGNAL(timeout()), this, SLOT(emitTextEditingFinished()));
}

gui::textedit::TextEdit::~TextEdit()
{
}

void gui::textedit::TextEdit::setCompleter(QCompleter* completer)
{
    if (m_completer)
        QObject::disconnect(m_completer, 0, this, 0);

    m_completer = completer;

    if (!m_completer)
        return;

    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(this->caseSensitivity());
    QObject::connect(
        m_completer, 
        SIGNAL(activated(QString)),
        this, 
        SLOT(insertCompletion(QString))
    );
}

QCompleter* gui::textedit::TextEdit::completer() const
{
    return m_completer;
}

Qt::CaseSensitivity gui::textedit::TextEdit::caseSensitivity() const
{
    return Qt::CaseInsensitive;
}

void gui::textedit::TextEdit::insertCompletion(const QString& completion)
{
    if (m_completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void gui::textedit::TextEdit::emitTextEditingFinished()
{
    emit textEditingFinished();
}

QString gui::textedit::TextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

int gui::textedit::TextEdit::minCompletionPrefixLength() const
{
    return 3;
}

void gui::textedit::TextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_completer)
        m_completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void  gui::textedit::TextEdit::keyPressEvent(QKeyEvent *e)
{
    m_editing_finished_timer.stop();
    m_editing_finished_timer.setSingleShot(true);
    m_editing_finished_timer.setInterval(this->TIMEOUT);
    m_editing_finished_timer.start();

    if (m_completer && m_completer->popup()->isVisible()) 
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) 
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
                e->ignore();
                return; // let the completer do default behavior
        default:
            break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+Space
    if (!m_completer || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < this->minCompletionPrefixLength()
                    || eow.contains(e->text().right(1)))) {
        m_completer->popup()->hide();
        return;
    }

    if (completionPrefix != m_completer->completionPrefix()) {
        m_completer->setCompletionPrefix(completionPrefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
                + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    m_completer->complete(cr); // popup it up!
}


int gui::textedit::TextEdit::TIMEOUT = 3000;
