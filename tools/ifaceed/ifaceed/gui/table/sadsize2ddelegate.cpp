#include "sadsize2ddelegate.h"

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


gui::table::SadSize2DDelegate::SadSize2DDelegate() : gui::table::Delegate()
{

}

gui::table::SadSize2DDelegate::~SadSize2DDelegate()
{

}

void gui::table::SadSize2DDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    sad::Size2D value = v.get<sad::Size2D>().value();
    this->setCurrentValue<sad::Size2D>(value);
    static_cast<gui::tuplewidget::DoubleTupleWidget*>(m_my_widget)->setValue(value.Width, value.Height);
    m_my_widget->blockSignals(b);
}

void gui::table::SadSize2DDelegate::widgetChanged(double f, double s)
{
    sad::Size2D oldvalue = this->currentValue<sad::Size2D>();
    sad::Size2D newvalue(f, s);
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<sad::Size2D>(oldvalue, newvalue, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<sad::Size2D>(m_object, Q2STDSTRING(m_property_name), oldvalue, newvalue)
        );
    }
    this->setCurrentValue<sad::Size2D>(newvalue);
}

void gui::table::SadSize2DDelegate::makeEditor()
{
    sad::Size2D value = this->currentValue<sad::Size2D>();
    gui::tuplewidget::DoubleTupleWidget * d = new gui::tuplewidget::DoubleTupleWidget();
    d->setLabel("w:", "h:");
    d->setValue(value.Width, value.Height);
    m_my_widget = d;
    this->insertToTable();
    m_widget->setRowHeight(m_row, d->rowHeight(0) * 2 + 4);
    QObject::connect(d, SIGNAL(valueChanged(double, double)), this, SLOT(widgetChanged(double, double)));
}

void gui::table::SadSize2DDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(double, double)));
}


