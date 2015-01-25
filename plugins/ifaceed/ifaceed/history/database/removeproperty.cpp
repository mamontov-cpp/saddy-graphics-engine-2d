#include "removeproperty.h"

#include "../../mainpanel.h"

history::database::RemoveProperty::RemoveProperty(gui::table::Delegate* d, MainPanel* panel)
{
	m_delegate = d;
	m_delegate->addRef();

	m_property = sad::Renderer::ref()
	->database("")
	->propertyByName(d->propertyName().toStdString())
	->clone();

	m_panel = panel;

	m_row = -1;
	QTableWidget* props = m_panel->UI()->twDatabaseProperties;
	for(size_t i = 0; i < props->rowCount() && m_row == -1; i++)
	{
		if (props->item(i, 0)->text() == d->propertyName())
		{
			m_row = i;
		}
	}
}

history::database::RemoveProperty::~RemoveProperty()
{
	m_delegate->delRef();

	delete m_property;
}

void history::database::RemoveProperty::commit(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->removeProperty(m_delegate->propertyName().toStdString());
	
	m_delegate->remove();

	m_panel->delegatesByName().remove(m_delegate->propertyName().toStdString());
}

void history::database::RemoveProperty::rollback(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->addProperty(m_delegate->propertyName().toStdString(), m_property->clone());
	if (m_row == -1)
	{
		m_delegate->add();
	}
	else
	{
		m_delegate->insert(m_row);
	}
	m_panel->delegatesByName().insert(m_delegate->propertyName().toStdString(), m_delegate);
}
