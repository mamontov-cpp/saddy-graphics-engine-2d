#include "gui/table/sadstringdelegate.h"

#include "core/editor.h"

#include "history/database/propertychanged.h"

#include <QTableWidgetItem>
#include <QPlainTextEdit>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

#include <climits>


gui::table::SadStringDelegate::SadStringDelegate() : gui::table::Delegate()
{

}

gui::table::SadStringDelegate::~SadStringDelegate()
{
	
}

void gui::table::SadStringDelegate::set(const sad::db::Variant& v)
{
	bool b = m_my_widget->blockSignals(true);
	QString value = v.get<QString>().value();
	this->setCurrentValue<QString>(value);
	static_cast<QPlainTextEdit*>(m_my_widget)->setPlainText(value);
	m_my_widget->blockSignals(b);
}

void gui::table::SadStringDelegate::widgetChanged()
{
	QString oldvalue = this->currentValue<QString>();
	QString i = static_cast<QPlainTextEdit*>(m_my_widget)->toPlainText();
	m_editor->history()->add(new history::database::PropertyChanged<QString>(oldvalue, i, this));
	this->setCurrentValue<QString>(i);
}

void gui::table::SadStringDelegate::makeEditor()
{
	QString value = this->currentValue<QString>();
	QPlainTextEdit * d = new QPlainTextEdit();
	d->setPlainText(value);
	m_my_widget = d;
	this->insertToTable();
	QObject::connect(d, SIGNAL(textChanged()), this, SLOT(widgetChanged()));	
}

void gui::table::SadStringDelegate::disconnect()
{
	QObject::disconnect(this, SLOT(widgetChanged()));
}
