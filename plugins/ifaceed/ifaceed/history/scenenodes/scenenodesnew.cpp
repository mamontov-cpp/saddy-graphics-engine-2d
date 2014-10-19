#include "scenenodesnew.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

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
}
