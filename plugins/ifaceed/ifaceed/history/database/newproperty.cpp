#include "newproperty.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"

history::database::NewProperty::NewProperty(gui::table::Delegate* d, MainPanel* p)
{
	m_delegate = d;
	m_delegate->addRef();

	m_property = sad::Renderer::ref()
	->database("")
	->propertyByName(Q2STDSTRING(d->propertyName()))
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
	->addProperty(Q2STDSTRING(m_delegate->propertyName()), m_property->clone());

	m_delegate->add();

	m_panel->delegatesByName().insert(Q2STDSTRING(m_delegate->propertyName()), m_delegate);
}

void history::database::NewProperty::rollback(core::Editor * ob)
{
	sad::Renderer::ref()
	->database("")
	->removeProperty(Q2STDSTRING(m_delegate->propertyName()));
	
	m_delegate->remove();

	m_panel->delegatesByName().remove(Q2STDSTRING(m_delegate->propertyName()));
}
