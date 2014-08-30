#include "gui/table/unsignedlonglongdelegate.h"

#include "core/editor.h"

#include "history/database/propertychanged.h"

#include "qwwulongspinbox.h"

#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::UnsignedLongLongDelegate::UnsignedLongLongDelegate() : gui::table::Delegate()
{

}

gui::table::UnsignedLongLongDelegate::~UnsignedLongLongDelegate()
{
	
}

void gui::table::UnsignedLongLongDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	unsigned long long value = v.get<unsigned long long>().value();
	this->setCurrentValue<unsigned long long>(value);
	static_cast<QwwULongSpinBox*>(m_my_widget)->setValue(value);
	m_my_widget->blockSignals(b);
}

void gui::table::UnsignedLongLongDelegate::widgetChanged(qulonglong i)
{
	unsigned long long oldvalue = this->currentValue<unsigned long long>();
	m_editor->history()->add(new history::database::PropertyChanged<unsigned long long>(oldvalue, i, this));
	this->setCurrentValue<unsigned long long>(i);
}

void gui::table::UnsignedLongLongDelegate::makeEditor()
{
	unsigned long long value = this->currentValue<unsigned long long>();
	QwwULongSpinBox * d = new QwwULongSpinBox();
	d->setValue(value);
	d->setMinimum(0);
	d->setMaximum(ULONG_MAX);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(valueChanged(qulonglong)), this, SLOT(widgetChanged(qulonglong)));	
}

void gui::table::UnsignedLongLongDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged(qulonglong)));
}
