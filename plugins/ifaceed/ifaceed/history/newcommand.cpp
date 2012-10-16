#include "newcommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"


NewCommand::NewCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	m_container = container;
	m_object = object;
	object->addRef();
}

void NewCommand::commit()
{
	m_container->add(m_object);
	ActionContext c;
	c.pushAction("NewCommand::commit");
	m_object->getProperty("activity")->set(sad::Variant(true),&c);
	c.popAction();
}

void NewCommand::rollback()
{
	m_container->remove(m_object);
	ActionContext c;
	c.pushAction("NewCommand::rollback");
	m_object->getProperty("activity")->set(sad::Variant(false),&c);
	c.popAction();
}

NewCommand::~NewCommand()
{
	m_object->delRef();
}