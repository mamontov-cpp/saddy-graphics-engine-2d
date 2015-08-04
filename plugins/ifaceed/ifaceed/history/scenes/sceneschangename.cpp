#include "sceneschangename.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../closuremethodcall.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

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
		ob->emitClosure( bind(this, &history::scenes::ChangeName::updateDependent, ob));		
	}
}

void history::scenes::ChangeName::rollback(core::Editor * ob)
{
	m_scene->setObjectName(m_old);
	if (ob)
	{
		ob->panel()->updateSceneName(m_scene);
		ob->emitClosure( bind(this, &history::scenes::ChangeName::updateDependent, ob));
	}
}

void history::scenes::ChangeName::updateDependent(core::Editor* e)
{
	MainPanel* p = e->panel();
	int pos = p->findInComboBox<sad::db::Object*>(p->UI()->cmbAnimationInstanceObject, m_scene);
	if (pos > - 1)
	{
		p->UI()->cmbAnimationInstanceObject->setItemText(pos, p->nameForScene(m_scene));
	}
	sad::Vector<sad::SceneNode*> nodes = m_scene->objects();
	for(size_t i = 0; i < nodes.size(); i++)
	{
		pos = p->findInComboBox<sad::db::Object*>(p->UI()->cmbAnimationInstanceObject, nodes[i]);
		if (pos > - 1)
		{
			p->UI()->cmbAnimationInstanceObject->setItemText(pos, p->fullNameForNode(nodes[i]));
		}
	}
}
