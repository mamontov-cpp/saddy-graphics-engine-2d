#include "gui/table/delegate.h"

#include "core/editor.h"

#include "../../mainpanel.h"

#include "../../history/database/removeproperty.h"


#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPushButton>

#include <renderer.h>
#include <db/dbdatabase.h>

gui::table::Delegate::Delegate() 
: m_custom_object(false), 
m_widget(NULL), 
m_editor(NULL),
m_my_widget(NULL)
{
	
}

gui::table::Delegate::~Delegate()
{
	
}

void gui::table::Delegate::makeLinkedTo(QTableWidget* widget,core::Editor* editor)
{
	m_widget = widget;
	m_editor = editor;
}

void gui::table::Delegate::linkToDatabase()
{
	m_custom_object = false;
}

void  gui::table::Delegate::linkToCustomObject()
{
	m_custom_object = true;
}

bool gui::table::Delegate::isLinkedToDatabase() const
{
	return !m_custom_object;
}

bool gui::table::Delegate::isLinkedToCustomObject() const
{
	return m_custom_object;
}

void  gui::table::Delegate::setPropertyName(const QString & name)
{
	m_property_name = name;
}

const QString& gui::table::Delegate::propertyName() const
{
	return m_property_name;
}

void gui::table::Delegate::add()
{
	if (this->isLinkedToDatabase())
	{
		m_widget->insertRow(m_widget->rowCount());
		unsigned int lastrow = m_widget->rowCount() - 1;
		m_row = lastrow;

		QTableWidgetItem* item = new QTableWidgetItem(this->propertyName());
		item->setFlags(item->flags() 
					   & ~Qt::ItemIsEditable 
					   & ~Qt::ItemIsSelectable
					   & ~Qt::ItemIsEnabled
		);
		m_widget->setItem(lastrow, 0, item);
		
		this->makeEditor();

		QPushButton * button = new QPushButton();
		button->setText("X");
		QFont font = button->font();
		font.setPointSize(16);
		button->setFont(font);
		button->setStyleSheet("QPushButton {color: red}");
		m_widget->setCellWidget(lastrow, 2, button);
		QObject::connect(button, SIGNAL(clicked()), this, SLOT(removeWithCommand()));
	}
	else
	{
		// TODO: Implement for custom object
	}
}

void gui::table::Delegate::remove()
{
	if (this->isLinkedToDatabase())
	{
		// Find a row in database
		unsigned int row = this->findPropertyInDatabase();
		if (row != -1)
		{
			this->disconnect();
			QObject::disconnect(this, SLOT(removeWithCommand()));
			m_widget->removeRow(row);
			m_my_widget = NULL;
		}
	}
	else
	{
		// TODO: Implement for custom object
	}
}

void gui::table::Delegate::removeWithCommand()
{
	m_editor->panel()->takeDelegateByPropertyName(this->propertyName());
	history::database::RemoveProperty* p = new history::database::RemoveProperty(this);
	p->commit();
	m_editor->history()->add(p);
}

int gui::table::Delegate::findPropertyInDatabase()
{
	unsigned int findrow = -1;
	for(unsigned int i = 0; i < m_widget->rowCount(); i++)
	{
		if (m_widget->item(i, 0)->text() == this->propertyName())
		{
			findrow = i;
		}
	}
	return findrow;
}


void gui::table::Delegate::insertToTable()
{
	m_widget->setCellWidget(m_row, 1, m_my_widget);
	m_widget->resizeRowToContents(m_row);
}
