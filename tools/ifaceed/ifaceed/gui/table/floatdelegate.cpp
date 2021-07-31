#include "gui/table/floatdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>



gui::table::FloatDelegate::FloatDelegate() : gui::table::Delegate()
{

}

gui::table::FloatDelegate::~FloatDelegate()
{
    
}

void gui::table::FloatDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    float value = v.get<float>().value();
    this->setCurrentValue<float>(value);
    static_cast<QDoubleSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::FloatDelegate::widgetChanged(double i)
{
    float old_value = this->currentValue<float>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<float>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<float>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<float>(i);
}

void gui::table::FloatDelegate::makeEditor()
{
    float value = this->currentValue<float>();
    QDoubleSpinBox * d = new QDoubleSpinBox();
    d->setValue(value);
    d->setMinimum(-1.0E+6);
    d->setMaximum(1.0E+6);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(double)), this, SLOT(widgetChanged(double)));	
}

void gui::table::FloatDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(double)));
}
