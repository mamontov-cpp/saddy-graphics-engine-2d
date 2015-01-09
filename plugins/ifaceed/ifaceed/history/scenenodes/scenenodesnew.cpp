#include "scenenodesnew.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::db::Object*)

history::scenenodes::New::New(sad::SceneNode* d) : m_node(d)
{
	m_node->addRef();
}

history::scenenodes::New::~New()
{
	m_node->delRef();
}

void history::scenenodes::New::commit(core::Editor * ob)
{
	m_node->setActive(true);
	if (m_node->scene() == ob->panel()->currentScene())
	{
		ob->panel()->addSceneNodeToSceneNodeList(m_node);
	}

	QVariant v;
	v.setValue(static_cast<sad::db::Object*>(m_node));
	if(ob)
	{
		void (QComboBox::*f)(const QString&, const QVariant&) = &QComboBox::addItem;
		ob->emitClosure( bind(
			ob->panel()->UI()->cmbAnimationInstanceObject,
			f,
			ob->panel()->fullNameForNode(m_node),
			v
		));
	}
}

void history::scenenodes::New::rollback(core::Editor * ob)
{
	m_node->setActive(false);
	if (m_node->scene() == ob->panel()->currentScene())
	{
		ob->panel()->removeLastSceneNodeFromSceneNodeList();
	}
	if (ob->shared()->selectedObject() == m_node)
	{
		ob->shared()->setSelectedObject(NULL);
		if (ob->machine()->isInState("selected"))
		{
			ob->machine()->enterState("idle");
		}
	}
	ob->emitClosure( bind(
			this,
			&history::scenenodes::New::eraseFromAnimationObjectsCombo,
			ob
	));
}

void history::scenenodes::New::eraseFromAnimationObjectsCombo(core::Editor* e)
{
	int pos  = e->panel()->findInComboBox<sad::db::Object*>(e->panel()->UI()->cmbAnimationInstanceObject, m_node);
	if (pos > -1)
	{
		e->panel()->UI()->cmbAnimationInstanceObject->removeItem(pos);
	}
}

