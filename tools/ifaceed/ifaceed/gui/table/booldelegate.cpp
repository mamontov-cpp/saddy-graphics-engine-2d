#include "gui/table/booldelegate.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"


gui::table::BoolDelegate::BoolDelegate() : gui::table::Delegate()
{

}

gui::table::BoolDelegate::~BoolDelegate()
{
    
}

void gui::table::BoolDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    bool value = v.get<bool>().value();
    this->setCurrentValue<bool>(value);
    Qt::CheckState state = (value) ? Qt::Checked : Qt::Unchecked;
    static_cast<QCheckBox*>(m_my_widget)->setCheckState(state);
    m_my_widget->blockSignals(b);
}

void gui::table::BoolDelegate::widgetChanged(int i)
{
    bool old_value = this->currentValue<bool>();
    bool new_value = (i == Qt::Checked);
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<bool>(old_value, new_value, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<bool>(m_object, Q2STDSTRING(m_property_name), old_value, new_value)
        );
    }
    this->setCurrentValue<bool>(i);
}

void gui::table::BoolDelegate::makeEditor()
{
    bool value = this->currentValue<bool>();
    QCheckBox * d = new QCheckBox();
    Qt::CheckState state = (value) ? Qt::Checked : Qt::Unchecked;
    d->setTristate(false);
    d->setCheckState(state);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(stateChanged(int)), this, SLOT(widgetChanged(int)));	
}

void gui::table::BoolDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(int)));
}
