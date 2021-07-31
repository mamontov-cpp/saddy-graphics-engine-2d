/*! \file stringeditordialog.h
    
    Contains editor for editing string values
 */
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <QDialog>
#include "ui_stringeditordialog.h"


/*! An editor for string values
 */
class StringEditorDialog : public QDialog
{
    Q_OBJECT
public:
    /*! Creates new editor for values
        \param[in] parent a parent widget
     */
    StringEditorDialog(QWidget* parent = nullptr);
    /*! Sets value for editing
        \param[in] value a value
     */
    void setValue(const QString& value);
    /*! Returns a value from editor
     */
    QString value() const;
    /*! Destructs editor
     */
    ~StringEditorDialog();
private:
    /*! An ui for editing
     */
    Ui::StringEditorDialog ui;
};
