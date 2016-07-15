#include "command.h"

history::Command::Command() 
{
    
}

history::Command::~Command()
{

}

void history::Command::commitWithoutUpdatingUI(core::Editor * e)
{
    this->commit(e);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void history::Command::blockedSetLineEditText(QLineEdit* edit, QString text) const
{
    int pos = edit->cursorPosition();
    bool b = edit->blockSignals(true);
    edit->setText(text);
    edit->blockSignals(b);
    edit->setCursorPosition(pos);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void history::Command::blockedSetTextEditText(QTextEdit* edit, QString text) const
{
    int pos = edit->textCursor().position();
    bool b = edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(b);
    QTextCursor current = edit->textCursor();
    QTextCursor endcursor = current;
    endcursor.movePosition(QTextCursor::End);
    if (pos <= endcursor.position()) {
        current.setPosition(pos);
        edit->setTextCursor(current);
    } else {
        edit->setTextCursor(endcursor);
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void history::Command::blockedSetPlainTextEditText(QPlainTextEdit* edit, QString text) const
{
    int pos = edit->textCursor().position();
    bool b = edit->blockSignals(true);
    edit->setPlainText(text);
    edit->blockSignals(b);
    QTextCursor current = edit->textCursor();
    QTextCursor endcursor = current;
    endcursor.movePosition(QTextCursor::End);
    if (pos <= endcursor.position()) {
        current.setPosition(pos);
        edit->setTextCursor(current);
    } else {
        edit->setTextCursor(endcursor);
    }
}
