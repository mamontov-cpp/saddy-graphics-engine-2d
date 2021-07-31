#include "gui/table/sadstringdelegate.h"

#include "stringeditordialog.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"


gui::table::SadStringDelegate::SadStringDelegate() : gui::table::Delegate()
{

}

gui::table::SadStringDelegate::~SadStringDelegate()
{
    
}

void gui::table::SadStringDelegate::set(const sad::db::Variant& v)
{
    QPlainTextEdit* edit = static_cast<QPlainTextEdit*>(static_cast<QTableWidget*>(m_my_widget)->cellWidget(0, 0));
    int pos = edit->textCursor().position();
    bool b = m_my_widget->blockSignals(true);
    QString value = v.get<QString>().value();
    this->setCurrentValue<QString>(value);
    edit->setPlainText(value);
    m_my_widget->blockSignals(b);
    QTextCursor c = edit->textCursor();
    QTextCursor c2 = c;
    c2.movePosition(QTextCursor::End);
    if (pos <= c2.position()) {
        c.setPosition(pos);
        edit->setTextCursor(c);
    } else {
        edit->setTextCursor(c2);
    }
}

void gui::table::SadStringDelegate::widgetChanged()
{
    QString old_value = this->currentValue<QString>();
    QString i = static_cast<QPlainTextEdit*>(static_cast<QTableWidget*>(m_my_widget)->cellWidget(0, 0))->toPlainText();
    if (old_value != i)
    {
        if (this->isLinkedToDatabase())
        {
            m_editor->history()->add(new history::database::ChangeProperty<QString>(old_value, i, this));
        }
        else
        {
            m_editor->history()->add(
                new history::customobject::ChangeProperty<sad::String>(m_object, Q2STDSTRING(m_property_name), Q2STDSTRING(old_value), Q2STDSTRING(i))
            );
        }
    }
    this->setCurrentValue<QString>(i);
}

void gui::table::SadStringDelegate::moreClicked()
{
    QString old_value = this->currentValue<QString>();
    StringEditorDialog dlg(m_my_widget);
    dlg.setValue(old_value);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString new_value = dlg.value();
        QPlainTextEdit* edit = static_cast<QPlainTextEdit*>(static_cast<QTableWidget*>(m_my_widget)->cellWidget(0, 0));
        edit->setPlainText(new_value);
    }
}

void gui::table::SadStringDelegate::makeEditor()
{
    QString value = this->currentValue<QString>();

    QTableWidget* w = new QTableWidget();
    w->horizontalHeader()->hide();
    w->verticalHeader()->hide();

    w->setRowCount(1);
    w->setColumnCount(2);

    QPlainTextEdit * d = new QPlainTextEdit();
    d->setPlainText(value);
    QSizePolicy policy = d->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    d->setSizePolicy(policy);
    w->setCellWidget(0, 0, d);

    QPushButton* pb = new QPushButton();
    pb->setText("...");
    w->setColumnWidth(1, 30);
    w->setCellWidget(0, 1, pb);
    
    w->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::LineEdit));

    m_my_widget = w;

    this->insertToTable();

    // Reset borders
    w->setFixedHeight(m_widget->rowHeight(m_row) - 2);
    w->setFixedWidth(m_widget->columnWidth(1) - 2);
    w->setRowHeight(0, m_widget->rowHeight(m_row) - 2);
    w->setColumnWidth(0, w->width() - w->columnWidth(1) - 2);

    QObject::connect(d, SIGNAL(textChanged()), this, SLOT(widgetChanged()));
    QObject::connect(pb, SIGNAL(clicked()), this, SLOT(moreClicked()));
}

void gui::table::SadStringDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged()));
    QObject::disconnect(this, SLOT(moreClicked()));
}
