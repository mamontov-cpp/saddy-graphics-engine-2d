#include "removeproperty.h"

history::database::RemoveProperty::RemoveProperty(gui::table::Delegate* d)
{
	m_delegate = d;
	m_delegate->addRef();

	m_property = sad::Renderer::ref()
	->database("")
	->propertyByName(d->propertyName().toStdString())
	->clone();
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
}

void history::database::RemoveProperty::rollback(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->addProperty(m_delegate->propertyName().toStdString(), m_property->clone());

	m_delegate->add();
}
