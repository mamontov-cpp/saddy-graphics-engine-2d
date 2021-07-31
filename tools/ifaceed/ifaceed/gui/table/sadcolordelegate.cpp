#include "sadcolordelegate.h"

#include "../colorview/colorview.h"

#include "core/editor.h"

#include "core/typeconverters/qcolortosadcolor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColorDialog>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../../qstdstring.h"


gui::table::SadColorDelegate::SadColorDelegate() : gui::table::Delegate()
{

}

gui::table::SadColorDelegate::~SadColorDelegate()
{
    
}

void gui::table::SadColorDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    QColor value = v.get<QColor>().value();
    this->setCurrentValue<QColor>(value);
    static_cast<gui::colorview::ColorView*>(m_my_widget)->setBackgroundColor(value);
    m_my_widget->blockSignals(b);
}

void gui::table::SadColorDelegate::clicked()
{
    QColor old_value = this->currentValue<QColor>();
    QColor i = QColorDialog::getColor(old_value, nullptr);
    if (i.isValid())
    {
        if (this->isLinkedToDatabase())
        {
            m_editor->history()->add(new history::database::ChangeProperty<QColor>(old_value, i, this));
        }
        else
        {
            sad::Color old_valueforprop;
            sad::Color new_valueforprop;
            core::typeconverters::QColorToSadColor::convert(old_value, old_valueforprop);
            core::typeconverters::QColorToSadColor::convert(i, new_valueforprop);
            m_editor->history()->add( 
                new history::customobject::ChangeProperty<sad::Color>(m_object, Q2STDSTRING(m_property_name), old_valueforprop, new_valueforprop)
            );
        }
        this->setCurrentValue<QColor>(i);
        static_cast<gui::colorview::ColorView*>(m_my_widget)->setBackgroundColor(i);
    }
}

void gui::table::SadColorDelegate::makeEditor()
{
    QColor value = this->currentValue<QColor>();
    gui::colorview::ColorView * d = new gui::colorview::ColorView();
    d->setBackgroundColor(value);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(clicked()), this, SLOT(clicked()));	
}

void  gui::table::SadColorDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(clicked()));
}
