#include "gui/table/longdelegate.h"

#include "core/editor.h"

#include "history/database/propertychanged.h"

#include "qwwlongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::LongDelegate::LongDelegate() : gui::table::Delegate()
{

}

gui::table::LongDelegate::~LongDelegate()
{
	
}

void gui::table::LongDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	long value = v.get<long>().value();
	this->setCurrentValue<long>(value);
	static_cast<QwwLongSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::LongDelegate::widgetChanged(qlonglong i)
{
	long oldvalue = this->currentValue<long>();
	m_editor->history()->add(new history::database::PropertyChanged<long>(oldvalue, i, this));
	this->setCurrentValue<long>(i);
}

void gui::table::LongDelegate::makeEditor()
{
	long value = this->currentValue<long>();
	QwwLongSpinBox * d = new QwwLongSpinBox();
	d->setValue(value);
	d->setMinimum(LONG_MIN);
	d->setMaximum(LONG_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(qlonglong)), this, SLOT(widgetChanged(qlonglong)));	
}

void gui::table::LongDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(qlonglong)));
}