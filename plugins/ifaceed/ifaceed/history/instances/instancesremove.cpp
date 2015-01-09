#include "instancesremove.h"

#include "../../core/editor.h"

#include "../../gui/instanceactions.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::animations::Instance*)

history::instances::Remove::Remove(
	sad::animations::Instance* d,
	int position,
	const sad::Vector<sad::Pair<sad::animations::Group*, int> >& dependent_groups
) : m_node(d), m_position_in_main_list(position), m_dependent_groups(dependent_groups)
{
	m_node->addRef();
}

history::instances::Remove::~Remove()
{
	m_node->delRef();
}

void history::instances::Remove::commit(core::Editor * ob)
{
	m_node->Active = false;
	for(size_t i = 0 ; i < m_dependent_groups.size(); i++)
	{
		m_dependent_groups[i]._1()->removeAsLink(m_node);
	}

	if (ob)
	{
		ob->emitClosure(bind(
			this,
			&history::instances::Remove::removeItem,			
			ob->panel()->UI()->lstAnimationInstances	
		));

		ob->emitClosure(bind(
			ob->panel()->instanceActions(),
			&gui::InstanceActions::updateGroupInstanceList
		));
	}
}

void history::instances::Remove::rollback(core::Editor * ob)
{
	m_node->Active = false;
	for(size_t i = 0 ; i < m_dependent_groups.size(); i++)
	{
		m_dependent_groups[i]._1()->insertAsLink(m_dependent_groups[i].p2(), m_node);
	}

	if (ob)
	{
		ob->emitClosure(bind(
			this,
			&history::instances::Remove::insertItem,			
			ob->panel()->UI()->lstAnimationInstances,
			ob->panel()->nameForInstance(m_node)
		));

		ob->emitClosure(bind(
			ob->panel()->instanceActions(),
			&gui::InstanceActions::updateGroupInstanceList
		));
	}
}

void history::instances::Remove::removeItem(QListWidget* widget)
{
	delete widget->takeItem(m_position_in_main_list);
}

void history::instances::Remove::insertItem(QListWidget* widget, const QString& name)
{
	QVariant v;
	v.setValue(m_node);

	QListWidgetItem* item = new QListWidgetItem(name);
	item->setData(Qt::UserRole, v);
	widget->insertItem(m_position_in_main_list, item);
}