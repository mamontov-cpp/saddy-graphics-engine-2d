#include "scenesadd.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include <QListWidget>
#include <QListWidgetItem>

history::scenes::Add::Add(sad::Scene * s) : m_scene(s)
{
	m_scene->addRef();
}

history::scenes::Add::~Add()
{
	m_scene->delRef();
}

void history::scenes::Add::commit(core::Editor * ob)
{
	m_scene->setActive(true);
	m_scene->Active = true;

	if (ob)
	{
		ob->panel()->addSceneToSceneList(m_scene);
	}
}

void history::scenes::Add::rollback(core::Editor * ob)
{
	m_scene->setActive(false);
	m_scene->Active = false;

	if (ob)
	{
		ob->panel()->removeLastSceneFromSceneList();
	}
}
