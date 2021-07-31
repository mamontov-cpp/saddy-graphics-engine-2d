#include "sadacolordelegate.h"

#include "../colorview/colorview.h"
#include "../acolordialog.h"

#include "core/editor.h"

#include "core/typeconverters/qcolortosadacolor.h"

#include "history/database/changeproperty.h"

#include "history/customobject/customobjectchangeproperty.h"

#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColorDialog>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>

#include "../mainpanelproxy.h"

#include "../../qstdstring.h"


gui::table::SadAColorDelegate::SadAColorDelegate() : gui::table::Delegate()
{

}

gui::table::SadAColorDelegate::~SadAColorDelegate()
{
    
}

void gui::table::SadAColorDelegate::set(const sad::db::Variant& v)
{
    bool b = m_my_widget->blockSignals(true);
    QColor value = v.get<QColor>().value();
    this->setCurrentValue<QColor>(value);
    static_cast<gui::colorview::ColorView*>(m_my_widget)->setBackgroundColor(value);
    m_my_widget->blockSignals(b);
}

void gui::table::SadAColorDelegate::clicked()
{
    QColor old_value = this->currentValue<QColor>();
    AColorDialog dlg;
    dlg.setColorPalette(m_editor->panelProxy()->colorPalette());
    dlg.setSelectedColor(old_value);
    
    if (dlg.exec() == QDialog::Accepted)
    {
        QColor i = dlg.selectedColor();
        m_editor->panelProxy()->setColorPalette(dlg.colorPalette());
        if (this->isLinkedToDatabase())
        {
            m_editor->history()->add(new history::database::ChangeProperty<QColor>(old_value, i, this));
        }
        else
        {
            sad::AColor old_valueforprop;
            sad::AColor new_valueforprop;
            core::typeconverters::QColorToSadAColor::convert(old_value, old_valueforprop);
            core::typeconverters::QColorToSadAColor::convert(i, new_valueforprop);
            m_editor->history()->add( 
                new history::customobject::ChangeProperty<sad::AColor>(m_object, Q2STDSTRING(m_property_name), old_valueforprop, new_valueforprop)
            );
        }
        static_cast<gui::colorview::ColorView*>(m_my_widget)->setBackgroundColor(i);
        this->setCurrentValue<QColor>(i);
    }
}

void gui::table::SadAColorDelegate::makeEditor()
{
    // Take first color from palette
    QList<QList<QColor> > palette = m_editor->panelProxy()->colorPalette();
    QColor value = palette[0][0];
    this->setCurrentValue<QColor>(value);
    gui::colorview::ColorView * d = new gui::colorview::ColorView();
    d->setBackgroundColor(value);
    m_my_widget = d;
    this->insertToTable();
    QObject::connect(d, SIGNAL(clicked()), this, SLOT(clicked()));	
}

void  gui::table::SadAColorDelegate::disconnect()
{
    QObject::disconnect(this, SLOT(clicked()));
}
