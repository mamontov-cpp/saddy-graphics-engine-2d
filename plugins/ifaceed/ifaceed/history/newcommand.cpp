#include "newcommand.h"

#include "../objects/abstractscreenobject.h"
#include "../objects/screentemplate.h"

#include "../core/editor.h"

#include <db/load.h>
#include <db/save.h>

#include <unused.h>


NewCommand::NewCommand(ScreenTemplate * container, AbstractScreenObject * object)
{
	SL_SCOPE("NewCommand::NewCommand");
	m_container = container;
	m_object = object;
	object->addRef();
}

void NewCommand::commit(core::Editor * ob)
{
	SL_SCOPE("NewCommand::commit");
	m_container->add(m_object);
	m_object->SerializableObject::getProperty("activity")->set(m_object, sad::db::Variant(true));
	ob->submitEvent("NewCommand::commit", sad::db::Variant(0));
}

void NewCommand::rollback(core::Editor * ob)
{
	SL_SCOPE("NewCommand::rollback");
	m_container->remove(m_object);
	m_object->SerializableObject::getProperty("activity")->set(m_object, sad::db::Variant(false));
	ob->submitEvent("NewCommand::rollback", sad::db::Variant(0));
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


void ScreenClearCommand::commit(core::Editor * ob)
{
	SL_SCOPE("ScreenClearCommand::commit");
    for (unsigned int i = 0 ; i < m_objects.count(); i++)
	{
		m_container->remove(m_objects[i]);
		m_objects[i]->setProp<bool>("activity", false, sad::log::Log::ref());
	}
	ob->submitEvent("ScreenClearCommand::commit", sad::db::Variant(0));

}

void ScreenClearCommand::rollback(core::Editor * ob)
{
	SL_SCOPE("ScreenClearCommand::rollback");
    for (unsigned int i = 0 ; i < m_objects.count(); i++)
	{
		m_container->add(m_objects[i]);
		m_objects[i]->setProp<bool>("activity", true, sad::log::Log::ref());
	}
	ob->submitEvent("ScreenClearCommand::rollback", sad::db::Variant(0));

}
