#include "gui/table/unsignedlonglongdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include "qwwulongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"

gui::table::UnsignedLongLongDelegate::UnsignedLongLongDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedLongLongDelegate::~UnsignedLongLongDelegate()
{
    
}

void gui::table::UnsignedLongLongDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    unsigned long long value = v.get<unsigned long long>().value();
    this->setCurrentValue<unsigned long long>(value);
    static_cast<QwwULongSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::UnsignedLongLongDelegate::widgetChanged(qulonglong i)
{
    unsigned long long old_value = this->currentValue<unsigned long long>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<unsigned long long>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<unsigned long long>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<unsigned long long>(i);
}

void gui::table::UnsignedLongLongDelegate::makeEditor()
{
    unsigned long long value = this->currentValue<unsigned long long>();
    QwwULongSpinBox * d = new QwwULongSpinBox();
    d->setValue(value);
    d->setMinimum(0);
    d->setMaximum(ULONG_MAX);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(qulonglong)), this, SLOT(widgetChanged(qulonglong)));	
}

void gui::table::UnsignedLongLongDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qulonglong)));
}
