#include "newcommand.h"
#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"
#include <unused.h>


NewCommand::NewCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	SL_SCOPE("NewCommand::NewCommand");
	m_container = container;
	m_object = object;
	object->addRef();
}

void NewCommand::commit(ActionContext *c, CommandChangeObserver * ob)
{
	m_container->add(m_object);
	c->pushAction("NewCommand::commit");
	m_object->getProperty("activity")->set(sad::Variant(true),c);
	ob->submitEvent("NewCommand::commit", sad::Variant(0));
	c->popAction();
}

void NewCommand::rollback(ActionContext *c, CommandChangeObserver * ob)
{
	m_container->remove(m_object);
	c->pushAction("NewCommand::rollback");
	m_object->getProperty("activity")->set(sad::Variant(false),c);
	ob->submitEvent("NewCommand::rollback", sad::Variant(0));
	c->popAction();
}

NewCommand::~NewCommand()
{
	m_object->delRef();
}


ScreenClearCommand::ScreenClearCommand(ScreenTemplate * container)
{
	SL_SCOPE("ScreenClearCommand::ScreenClearCommand");
	m_objects.clear();
	AbstractScreenObject * o = container->templateBegin();
	while(o)
	{
		o->addRef();
		m_objects << o;
		 o = container->templateNext();
	}
	m_container = container;
}


ScreenClearCommand::~ScreenClearCommand()
{
    for (unsigned int i = 0; i < m_objects.count() ; i++)
	{
		m_objects[i]->delRef();
	}
}


void ScreenClearCommand::commit(UNUSED ActionContext *c, CommandChangeObserver * ob)
{
	SL_SCOPE("ScreenClearCommand::commit");
    for (unsigned int i = 0 ; i < m_objects.count(); i++)
	{
		m_container->remove(m_objects[i]);
		m_objects[i]->setProp<bool>("activity", false, sad::Log::ref());
	}
	ob->submitEvent("ScreenClearCommand::commit", sad::Variant(0));

}

void ScreenClearCommand::rollback(UNUSED ActionContext *c, CommandChangeObserver * ob)
{
	SL_SCOPE("ScreenClearCommand::rollback");
    for (unsigned int i = 0 ; i < m_objects.count(); i++)
	{
		m_container->add(m_objects[i]);
		m_objects[i]->setProp<bool>("activity", true, sad::Log::ref());
	}
	ob->submitEvent("ScreenClearCommand::rollback", sad::Variant(0));

}
