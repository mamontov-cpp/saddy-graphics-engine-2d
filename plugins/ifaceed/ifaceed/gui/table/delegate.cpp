#include "gui/table/delegate.h"

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
