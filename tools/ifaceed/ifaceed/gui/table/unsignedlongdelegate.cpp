#include "gui/table/unsignedlongdelegate.h"

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

gui::table::UnsignedLongDelegate::UnsignedLongDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedLongDelegate::~UnsignedLongDelegate()
{
    
}

void gui::table::UnsignedLongDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    unsigned long value = v.get<unsigned long>().value();
    this->setCurrentValue<unsigned long>(value);
    static_cast<QwwULongSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::UnsignedLongDelegate::widgetChanged(qulonglong i)
{
    unsigned long old_value = this->currentValue<unsigned long>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<unsigned long>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<unsigned long>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<unsigned long>(i);
}

void gui::table::UnsignedLongDelegate::makeEditor()
{
    unsigned long value = this->currentValue<unsigned long>();
    QwwULongSpinBox * d = new QwwULongSpinBox();
    d->setValue(value);
    d->setMinimum(0);
    d->setMaximum(ULONG_MAX);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(qulonglong)), this, SLOT(widgetChanged(qulonglong)));	
}

void gui::table::UnsignedLongDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qulonglong)));
}
