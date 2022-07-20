#include "gui/table/doubledelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::DoubleDelegate::DoubleDelegate() : gui::table::Delegate()
{

}

gui::table::DoubleDelegate::~DoubleDelegate()
{
    
}

void gui::table::DoubleDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    double value = v.get<double>().value();
    this->setCurrentValue<double>(value);
    static_cast<QDoubleSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::DoubleDelegate::widgetChanged(double i)
{
    double old_value = this->currentValue<double>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<double>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<double>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<double>(i);
}

void gui::table::DoubleDelegate::makeEditor()
{
    double value = this->currentValue<double>();
    QDoubleSpinBox * d = new QDoubleSpinBox();
    d->setMinimum(-1.0E+6);
    d->setMaximum(1.0E+6);
    d->setValue(value);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(double)), this, SLOT(widgetChanged(double)));	
}

void gui::table::DoubleDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(double)));
}
