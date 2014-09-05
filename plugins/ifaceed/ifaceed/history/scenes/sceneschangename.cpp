#include "sceneschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"


history::scenes::ChangeName::ChangeName(
	sad::Scene * s, 
	const sad::String& oldname, 
	const sad::String& newname
) : m_scene(s), m_old(oldname), m_new(newname) 
{
	m_scene->addRef();
}

history::scenes::ChangeName::~ChangeName()
{
	m_scene->delRef();
}

void history::scenes::ChangeName::commit(core::Editor * ob)
{
	m_scene->setObjectName(m_new);
	if (ob)
	{
		ob->panel()->updateSceneName(m_scene);
	}
}

void history::scenes::ChangeName::rollback(core::Editor * ob)
{
	m_scene->setObjectName(m_old);
	if (ob)
	{
		ob->panel()->updateSceneName(m_scene);
	}
}
