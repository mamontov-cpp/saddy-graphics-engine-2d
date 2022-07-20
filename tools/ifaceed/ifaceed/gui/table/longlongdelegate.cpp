#include "gui/table/longlongdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include "qwwlongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"


gui::table::LongLongDelegate::LongLongDelegate() : gui::table::Delegate()
{

}

gui::table::LongLongDelegate::~LongLongDelegate()
{
    
}

void gui::table::LongLongDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    long value = v.get<long long>().value();
    this->setCurrentValue<long long>(value);
    static_cast<QwwLongSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::LongLongDelegate::widgetChanged(qlonglong i)
{
    long long old_value = this->currentValue<long long>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<long long>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<long long>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<long long>(i);
}

void gui::table::LongLongDelegate::makeEditor()
{
    long long value = this->currentValue<long long>();
    QwwLongSpinBox * d = new QwwLongSpinBox();
    d->setMinimum(LLONG_MIN);
    d->setMaximum(LLONG_MAX);
    d->setValue(value);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(qlonglong)), this, SLOT(widgetChanged(qlonglong)));	
}

void gui::table::LongLongDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qlonglong)));
}
