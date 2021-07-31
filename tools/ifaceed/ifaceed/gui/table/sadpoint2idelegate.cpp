#include "gui/table/sadpoint2idelegate.h"

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

gui::table::SadPoint2IDelegate::SadPoint2IDelegate() : gui::table::Delegate()
{

}

gui::table::SadPoint2IDelegate::~SadPoint2IDelegate()
{

}

void gui::table::SadPoint2IDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    sad::Point2I value = v.get<sad::Point2I>().value();
    this->setCurrentValue<sad::Point2I>(value);
    static_cast<gui::tuplewidget::Int64TupleWidget*>(m_my_widget)->setValue(value.x(), value.y());
    m_my_widget->blockSignals(b);
}

void gui::table::SadPoint2IDelegate::widgetChanged(qlonglong f, qlonglong s)
{
    sad::Point2I old_value = this->currentValue<sad::Point2I>();
    sad::Point2I new_value(f, s);
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<sad::Point2I>(old_value, new_value, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<sad::Point2I>(m_object, Q2STDSTRING(m_property_name), old_value, new_value)
        );
    }
    this->setCurrentValue<sad::Point2I>(new_value);
}

void gui::table::SadPoint2IDelegate::makeEditor()
{
    sad::Point2I value = this->currentValue<sad::Point2I>();
    gui::tuplewidget::Int64TupleWidget * d = new gui::tuplewidget::Int64TupleWidget();
    d->setLabel("x:", "y:");
    d->setValue(value.x(), value.y());
    d->setRange(INT_MIN, INT_MAX);
    m_my_widget = d;
    this->insertToTable();
    m_widget->setRowHeight(m_row, d->rowHeight(0) * 2 + 4);
    QObject::connect(d, SIGNAL(valueChanged(qlonglong, qlonglong)), this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

void gui::table::SadPoint2IDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(qlonglong, qlonglong)));
}

