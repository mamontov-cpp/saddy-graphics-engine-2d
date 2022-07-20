#include "gui/table/unsignedshortdelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::UnsignedShortDelegate::UnsignedShortDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedShortDelegate::~UnsignedShortDelegate()
{
    
}

void gui::table::UnsignedShortDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    unsigned short value = v.get<unsigned short>().value();
    this->setCurrentValue<unsigned short>(value);
    static_cast<QSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::UnsignedShortDelegate::widgetChanged(int i)
{
    unsigned short old_value = this->currentValue<unsigned short>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<unsigned short>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<unsigned short>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<unsigned short>(i);
}

void gui::table::UnsignedShortDelegate::makeEditor()
{
    unsigned short value = this->currentValue<unsigned short>();
    QSpinBox * d = new QSpinBox();
    d->setMinimum(0);
    d->setMaximum(USHRT_MAX);
    d->setValue((int)(value));
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::UnsignedShortDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(int)));
}