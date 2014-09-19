#include "scenenodesremove.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

#include <renderer.h>

history::scenenodes::Remove::Remove(sad::SceneNode* d, int position)
: m_position(position), m_node(d)
{
	m_node->addRef();
}

history::scenenodes::Remove::~Remove()
{
	m_node->delRef();
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
	}
}


void history::scenenodes::Remove::rollback(core::Editor * ob)
{
	m_node->setActive(true);
	if (ob)
	{
		ob->emitClosure( bind(ob->panel(), &MainPanel::insertSceneNodeToSceneNodeList, m_node, m_position) );
	}
}
