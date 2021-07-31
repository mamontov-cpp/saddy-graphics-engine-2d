#include "gui/table/sadpoint2ddelegate.h"

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

gui::table::SadPoint2DDelegate::SadPoint2DDelegate() : gui::table::Delegate()
{

}

gui::table::SadPoint2DDelegate::~SadPoint2DDelegate()
{
    
}

void gui::table::SadPoint2DDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    sad::Point2D value = v.get<sad::Point2D>().value();
    this->setCurrentValue<sad::Point2D>(value);
    static_cast<gui::tuplewidget::DoubleTupleWidget*>(m_my_widget)->setValue(value.x(), value.y());
    m_my_widget->blockSignals(b);
}

void gui::table::SadPoint2DDelegate::widgetChanged(double f, double s)
{
    sad::Point2D old_value = this->currentValue<sad::Point2D>();
    sad::Point2D new_value(f, s);
    if (this->isLinkedToDatabase())
    {
        m_editor->history()->add(new history::database::ChangeProperty<sad::Point2D>(old_value, new_value, this));
    }
    else
    {
        m_editor->history()->add( 
            new history::customobject::ChangeProperty<sad::Point2D>(m_object, Q2STDSTRING(m_property_name), old_value, new_value)
        );
    }
    this->setCurrentValue<sad::Point2D>(new_value);
}

void gui::table::SadPoint2DDelegate::makeEditor()
{
    sad::Point2D value = this->currentValue<sad::Point2D>();
    gui::tuplewidget::DoubleTupleWidget * d = new gui::tuplewidget::DoubleTupleWidget();
    d->setLabel("x:", "y:");
    d->setValue(value.x(), value.y());
    m_my_widget = d;
    this->insertToTable();
    m_widget->setRowHeight(m_row, d->rowHeight(0) * 2 + 4);
    QObject::connect(d, SIGNAL(valueChanged(double, double)), this, SLOT(widgetChanged(double, double)));	
}

void gui::table::SadPoint2DDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(widgetChanged(double, double)));
}

