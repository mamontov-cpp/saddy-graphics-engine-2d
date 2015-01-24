#include "newproperty.h"

#include "../../mainpanel.h"

history::database::NewProperty::NewProperty(gui::table::Delegate* d, MainPanel* p)
{
	m_delegate = d;
	m_delegate->addRef();

	m_property = sad::Renderer::ref()
	->database("")
	->propertyByName(d->propertyName().toStdString())
	->clone();

	m_panel =  p;
}

history::database::NewProperty::~NewProperty()
{
	m_delegate->delRef();

	delete m_property;
}

void history::database::NewProperty::commit(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->addProperty(m_delegate->propertyName().toStdString(), m_property->clone());

	m_delegate->add();

	m_panel->delegatesByName().insert(m_delegate->propertyName().toStdString(), m_delegate);
}

void history::database::NewProperty::rollback(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->removeProperty(m_delegate->propertyName().toStdString());
	
	m_delegate->remove();

	m_panel->delegatesByName().remove(m_delegate->propertyName().toStdString());
}
