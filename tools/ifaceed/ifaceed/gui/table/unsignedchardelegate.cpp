#include "gui/table/unsignedchardelegate.h"

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


gui::table::UnsignedCharDelegate::UnsignedCharDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedCharDelegate::~UnsignedCharDelegate()
{
    
}

void gui::table::UnsignedCharDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    unsigned char value = v.get<unsigned char>().value();
    this->setCurrentValue<unsigned char>(value);
    static_cast<QSpinBox*>(m_my_widget)->setValue(value);
    m_my_widget->blockSignals(b);
}

void gui::table::UnsignedCharDelegate::widgetChanged(int i)
{
    unsigned char old_value = this->currentValue<unsigned char>();
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<unsigned char>(old_value, i, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<unsigned char>(m_object, Q2STDSTRING(m_property_name), old_value, i)
        );
    }
    this->setCurrentValue<unsigned char>(i);
}

void gui::table::UnsignedCharDelegate::makeEditor()
{
    unsigned char value = this->currentValue<unsigned char>();
    QSpinBox * d = new QSpinBox();
    d->setValue((int)(value));
    d->setMinimum(0);
    d->setMaximum(UCHAR_MAX);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(valueChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::UnsignedCharDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(int)));
}