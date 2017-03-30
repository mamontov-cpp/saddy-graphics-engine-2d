#include "stringeditordialog.h"

StringEditorDialog::StringEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void StringEditorDialog::setValue(const QString& value)
{
    ui.txtCodeEdit->setPlainText(value);
}

QString StringEditorDialog::value() const
{
    return ui.txtCodeEdit->toPlainText();
}

StringEditorDialog::~StringEditorDialog()
{
}
