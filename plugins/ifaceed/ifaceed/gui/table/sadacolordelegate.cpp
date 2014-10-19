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
	QColor oldvalue = this->currentValue<QColor>();
	AColorDialog dlg;
	dlg.setColorPalette(m_editor->panel()->colorPalette());
	dlg.setSelectedColor(oldvalue);
	
	if (dlg.exec() == QDialog::Accepted)
	{
		QColor i = dlg.selectedColor();
		m_editor->panel()->setColorPalette(dlg.colorPalette());
		if (this->isLinkedToDatabase())
		{
			m_editor->history()->add(new history::database::ChangeProperty<QColor>(oldvalue, i, this));
		}
		else
		{
			sad::AColor oldvalueforprop;
			sad::AColor newvalueforprop;
			core::typeconverters::QColorToSadAColor::convert(oldvalue, oldvalueforprop);
			core::typeconverters::QColorToSadAColor::convert(i, newvalueforprop);
			m_editor->history()->add( 
				new history::customobject::ChangeProperty<sad::AColor>(m_object, m_property_name.toStdString(), oldvalueforprop, newvalueforprop)
			);
		}
		static_cast<gui::colorview::ColorView*>(m_my_widget)->setBackgroundColor(i);
		this->setCurrentValue<QColor>(i);
	}
}

void gui::table::SadAColorDelegate::makeEditor()
{
	// Take first color from palette
	QList<QList<QColor> > palette = m_editor->panel()->colorPalette();
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
