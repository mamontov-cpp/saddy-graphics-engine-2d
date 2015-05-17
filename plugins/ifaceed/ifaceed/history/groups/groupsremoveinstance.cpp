#include "groupsremoveinstance.h"

#include "../../gui/instanceactions.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::animations::Group*)
Q_DECLARE_METATYPE(sad::animations::Instance*)

history::groups::RemoveInstance::RemoveInstance( sad::animations::Group* d,
		 sad::animations::Instance* i,
		 int pos
) : m_node(d), m_instance(i), m_position(pos)
{
	m_node->addRef();
	m_instance->addRef();
}

history::groups::RemoveInstance::~RemoveInstance()
{
	m_node->delRef();
	m_instance->delRef();
}

void history::groups::RemoveInstance::commit(core::Editor * ob)
{
	m_node->removeAsLink(m_instance);

	if (ob)
	{
		if (ob->shared()->selectedGroup() == m_node)
		{
			ob->emitClosure(bind(
				ob->panel()->instanceActions(),
				&gui::InstanceActions::updateGroupInstanceList
			));
		}
	}
}

void history::groups::RemoveInstance::rollback(core::Editor * ob)
{
	m_node->insertAsLink(m_position, m_instance);

	if (ob)
	{
		if (ob->shared()->selectedGroup() == m_node)
		{
			ob->emitClosure(bind(
				ob->panel()->instanceActions(),
				&gui::InstanceActions::updateGroupInstanceList
			));
		}
	}
}
