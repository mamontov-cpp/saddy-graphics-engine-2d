#include "gui/table/signedchardelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"

gui::table::SignedCharDelegate::SignedCharDelegate() : gui::table::Delegate()
{

}

gui::table::SignedCharDelegate::~SignedCharDelegate()
{
    
}

void gui::table::SignedCharDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    signed char value = v.get<signed char>().value();
    this->setCurrentValue<signed char>(value);
    static_cast<QSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::SignedCharDelegate::widgetChanged(int i)
{
    signed char old_value = this->currentValue<signed char>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<signed char>(old_value, i, this));
    }
    else
    {	
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<signed char>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<signed char>(i);
}

void gui::table::SignedCharDelegate::makeEditor()
{
    signed char value = this->currentValue<signed char>();
    QSpinBox * d = new QSpinBox();
    d->setValue((int)(value));
    d->setMinimum(CHAR_MIN);
    d->setMaximum(CHAR_MAX);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::SignedCharDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(int)));
}