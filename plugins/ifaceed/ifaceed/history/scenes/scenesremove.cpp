#include "scenesremove.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"


history::scenes::Remove::Remove(sad::Scene * s, int position) : m_scene(s), m_position(position)
{
	m_scene->addRef();
}

history::scenes::Remove::~Remove()
{
	m_scene->delRef();
}

void history::scenes::Remove::commit(core::Editor * ob)
{
	m_scene->setActive(false);
	if (ob)
	{
		ob->panel()->removeSceneFromSceneList(m_position);
	}
}

void history::scenes::Remove::rollback(core::Editor * ob)
{
	m_scene->setActive(true);
	if (ob)
	{
		ob->panel()->insertSceneToSceneList(m_scene, m_position);
	}
}
