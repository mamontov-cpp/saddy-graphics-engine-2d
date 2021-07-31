/*! \file textedit.h
    

    A text edit with support of autocompletion and additional signal,
    emitted after finishing text
 */
#pragma once
#include <QTextEdit>
#include <QTimer>

class QCompleter;

namespace gui
{

namespace textedit
{

/*! A text edit with support of autocompletion and additional signal,
    emitted after finishing text. Most of implementation taken from 
    http://qt-project.org/doc/qt-4.8/tools-customcompleter-textedit-h.html
 */
class TextEdit : public QTextEdit
{
Q_OBJECT
public:
    /*! Constructs a text edit widget
        \param[in] parent a parent widget
     */ 
    TextEdit(QWidget* parent = nullptr);
     /*! Destructor for widget
      */
    virtual ~TextEdit() override;
    /*! Sets a completer for text edit
        \param[in] c a completer
     */
    void setCompleter(QCompleter* c);
    /*! Returns a completer for edit
        \return a completer
     */
    QCompleter* completer() const;
    /*! Returns case sensitivity for completer
        \return sensitivity
     */
    virtual Qt::CaseSensitivity caseSensitivity() const;
signals:
    /*! Emitted, when text editing is finished
     */
    void textEditingFinished();
protected:
    /*! A handler for key press event
        \param[in] e event
     */
    void keyPressEvent(QKeyEvent* e) override;
    /*! A handler for focus in event
        \param[in] e event
     */
    void focusInEvent(QFocusEvent* e) override;
private slots:
    /*! Inserts completion into text area
        \param[in] completion a completion part
     */
    void insertCompletion(const QString& completion);
    /*! Emits signal textEditingFinished
     */
    void emitTextEditingFinished();
protected:
    /*! Returns text under cursor
        \return text under cursor
     */
    virtual QString textUnderCursor() const;
    /*! Returns minimal prefix length for context
        \return prefix
     */
    virtual int minCompletionPrefixLength() const;
    /*! An attached completer
     */
    QCompleter* m_completer;
    /*! A timer, which indicated, whether editing is finished
     */
    QTimer m_editing_finished_timer;
    /*! Timeout for editing to be finished
     */
    static int TIMEOUT;
};

}

}
