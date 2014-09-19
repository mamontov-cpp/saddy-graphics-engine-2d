#include "scenesclear.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

#include <renderer.h>


history::scenes::Clear::Clear(sad::Scene * s) : m_scene(s)
{
	m_scene->addRef();
	m_nodes = m_scene->objects();
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->active() == false)
		{
			m_nodes.removeAt(i);
			--i;
		}
		else
		{
			m_nodes[i]->addRef();
		}
	}
}

history::scenes::Clear::~Clear()
{
	m_scene->delRef();
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->delRef();
	}
}	


void history::scenes::Clear::commit(core::Editor * ob)
{
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->setActive(false);
	}
	if (ob)
	{
		if (ob->panel()->currentScene() == m_scene)
		{
			ob->emitClosure( bind(ob->panel()->UI()->lstSceneObjects, &QListWidget::clear) );
		}
		for(size_t i = 0; i < m_nodes.size(); i++)
		{
			if (ob->shared()->selectedObject() == m_nodes[i])
			{
				ob->shared()->setSelectedObject(NULL);
				if (ob->machine()->isInState("selected"))
				{
					ob->machine()->enterState("idle");
				}
			}
		}
	}
}

void history::scenes::Clear::rollback(core::Editor * ob)
{
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->setActive(true);
	}
	if (ob)
	{
		if (ob->panel()->currentScene() == m_scene)
		{
			for(size_t i = 0; i < m_nodes.size(); i++)
			{
				ob->emitClosure( bind(ob->panel(), &MainPanel::addSceneNodeToSceneNodeList, m_nodes[i]) );
			}
		}
	}
}