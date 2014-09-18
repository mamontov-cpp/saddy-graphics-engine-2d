#include "sadcolordelegate.h"

#include "../colorview/colorview.h"

#include "core/editor.h"

#include "history/database/changeproperty.h"

#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColorDialog>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


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
	QColor oldvalue = this->currentValue<QColor>();
	QColor i = QColorDialog::getColor(oldvalue, NULL);
	if (i.isValid())
	{
		m_editor->history()->add(new history::database::ChangeProperty<QColor>(oldvalue, i, this));
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
