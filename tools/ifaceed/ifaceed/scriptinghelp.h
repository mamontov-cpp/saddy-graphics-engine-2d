/*! \file scriptinghelp.h
    

    Describes a dialog for showing scripting help
 */
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <QDialog>
#include <QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QTextEdit>

#include "ui_scriptinghelp.h"

/*! A dialog for showing help for scripting editor
 */
class ScriptingHelp: public QDialog
{
Q_OBJECT
public:
    /*! Creates new file list for reloading
        \param[in] parent a parent widget for selecting
     */
    ScriptingHelp(QWidget* parent = nullptr);
    /*! Could be inherited
     */
    virtual ~ScriptingHelp();
    /*! Sets help text
     */
    void setText(const QString& text);
public slots:
    /*! Applies search Called, when return on request edit is pressed
     */
    void applySearch();
    /*! Cancels search in scripting help
     */
    void cancelSearch();
protected:
    /*! UI for dialog
     */
    Ui::ScriptingHelp  m_ui;
    /*! A last requested part
     */
    QString m_last_request;
    /*! A list of extra selections
     */
    QList<QTextEdit::ExtraSelection> m_extra_selections;
    /*! A position in extra selection list
     */
    int m_position;
};