#include "scenenodesremove.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

#include <renderer.h>

Q_DECLARE_METATYPE(sad::db::Object*)

history::scenenodes::Remove::Remove(sad::SceneNode* d, int position)
: m_node(d), m_position(position), m_position_in_instance_combo(-1)
{
	m_node->addRef();
}

history::scenenodes::Remove::~Remove()
{
	m_node->delRef();
}

void history::scenenodes::Remove::set(
	int  position_in_instance_combo,
	const sad::Vector< sad::animations::Instance* >& instances
)
{
	m_position_in_instance_combo = position_in_instance_combo;
	m_dependent_instances = instances;
}

void history::scenenodes::Remove::commit(core::Editor * ob)
{
	m_node->setActive(false);
	if (ob)
	{
		if (ob->panel()->currentScene() == m_node->scene())
		{			
			ob->emitClosure( bind(ob->panel(), &MainPanel::removeSceneNodeFromSceneNodeList, m_position) );
		}
		if (ob->shared()->selectedObject() == m_node)
		{
			ob->shared()->setSelectedObject(NULL);
			if (ob->machine()->isInState("selected"))
			{
				ob->machine()->enterState("idle");
			}
		}
		if (m_position_in_instance_combo > -1)
		{
			ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::removeItem, m_position_in_instance_combo));
		}
		for(size_t i = 0; i < m_dependent_instances.size(); i++)
		{
			m_dependent_instances[i]->setObjectId(0);
			if (m_dependent_instances[i] == ob->shared()->selectedInstance())
			{
				ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, 0));
			}
		}
	}
}


void history::scenenodes::Remove::rollback(core::Editor * ob)
{
	m_node->setActive(true);
	if (ob)
	{
		ob->emitClosure( bind(ob->panel(), &MainPanel::insertSceneNodeToSceneNodeList, m_node, m_position) );
		if (m_position_in_instance_combo > -1)
		{
			QVariant v;
			v.setValue(static_cast<sad::db::Object*>(m_node));

			void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;

			ob->emitClosure( bind(
				ob->panel()->UI()->cmbAnimationInstanceObject, 
				f, 
				m_position_in_instance_combo,
				ob->panel()->fullNameForNode(m_node),
				v
			));
		}
		for(size_t i = 0; i < m_dependent_instances.size(); i++)
		{
			m_dependent_instances[i]->setObjectId(m_node->MajorId);
			if (m_dependent_instances[i] == ob->shared()->selectedInstance())
			{
				ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, m_position_in_instance_combo));
			}
		}
	}
}
