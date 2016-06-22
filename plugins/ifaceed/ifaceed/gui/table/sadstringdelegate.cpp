#include "gui/table/sadstringdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QPlainTextEdit>
#include <QPushButton>

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
    QPlainTextEdit* edit = static_cast<QPlainTextEdit*>(m_my_widget);
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
    QString oldvalue = this->currentValue<QString>();
    QString i = static_cast<QPlainTextEdit*>(m_my_widget)->toPlainText();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<QString>(oldvalue, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<sad::String>(m_object, Q2STDSTRING(m_property_name), Q2STDSTRING(oldvalue), Q2STDSTRING(i))
        );
    }
    this->setCurrentValue<QString>(i);
}

void gui::table::SadStringDelegate::makeEditor()
{
    QString value = this->currentValue<QString>();
    QPlainTextEdit * d = new QPlainTextEdit();
    d->setPlainText(value);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(textChanged()), this, SLOT(widgetChanged()));	
}

void gui::table::SadStringDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged()));
}
