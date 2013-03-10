#include "newcommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"


NewCommand::NewCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	m_container = container;
	m_object = object;
	object->addRef();
}

void NewCommand::commit(ActionContext *c, CommandChangeObserver * ob)
{
	m_container->add(m_object);
	c->pushAction("NewCommand::commit");
	m_object->getProperty("activity")->set(sad::Variant(true),c);
	ob->submitEvent("new", sad::Variant(0));
	c->popAction();
}

void NewCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_container->remove(m_object);
	c->pushAction("NewCommand::rollback");
	m_object->getProperty("activity")->set(sad::Variant(false),c);
	ob->submitEvent("new", sad::Variant(0));
	c->popAction();
}

NewCommand::~NewCommand()
{
	m_object->delRef();
}