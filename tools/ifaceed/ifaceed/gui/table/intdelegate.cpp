#include "gui/table/intdelegate.h"

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


gui::table::IntDelegate::IntDelegate() : gui::table::Delegate()
{

}

gui::table::IntDelegate::~IntDelegate()
{
    
}

void gui::table::IntDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    int value = v.get<int>().value();
    this->setCurrentValue<int>(value);
    static_cast<QSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::IntDelegate::widgetChanged(int i)
{
    int old_value = this->currentValue<int>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<int>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<int>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<int>(i);
}

void gui::table::IntDelegate::makeEditor()
{
    int value = this->currentValue<int>();
    QSpinBox * d = new QSpinBox();
    d->setValue((int)(value));
    d->setMinimum(INT_MIN);
    d->setMaximum(INT_MAX);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::IntDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(int)));
}