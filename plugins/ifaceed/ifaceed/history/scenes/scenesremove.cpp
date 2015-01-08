#include "scenesremove.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

Q_DECLARE_METATYPE(sad::db::Object*)

history::scenes::Remove::Remove(sad::Scene * s, int position) 
: m_scene(s), 
m_position(position),
m_position_in_instance_combo(-1)
{
	m_scene->addRef();
}

history::scenes::Remove::~Remove()
{
	m_scene->delRef();
}

void history::scenes::Remove::set(
	int position_in_instance_combo,
	const sad::Vector< sad::Pair<sad::SceneNode*, int> >& positions,
	const sad::Vector<sad::animations::Instance*>& dependent_animations
)
{
	m_position_in_instance_combo = position_in_instance_combo;
	m_scenenodes_positions = positions;
	m_dependent_animations = dependent_animations;
}

void history::scenes::Remove::commit(core::Editor * ob)
{
	m_scene->setActive(false);
	for(size_t i = 0; i < m_dependent_animations.size(); i++)
	{
		m_dependent_animations[i]->setObjectId(0);
	}
	if (ob)
	{
		ob->panel()->removeSceneFromSceneList(m_position);
		if (m_position_in_instance_combo > -1)
		{
			ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::removeItem, m_position_in_instance_combo));
		}
		for(size_t i = 0; i < m_scenenodes_positions.size(); i++)
		{
			ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::removeItem, m_scenenodes_positions[i].p2()));
		}
		for(size_t i = 0; i < m_dependent_animations.size(); i++)
		{
			if (m_dependent_animations[i] == ob->shared()->selectedInstance())
			{
				ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, 0));
			}
		}
	}
}

void history::scenes::Remove::rollback(core::Editor * ob)
{
	m_scene->setActive(true);
	for(size_t i = 0; i < m_dependent_animations.size(); i++)
	{
		m_dependent_animations[i]->setObjectId(m_scene->MajorId);
	}
	if (ob)
	{
		ob->panel()->insertSceneToSceneList(m_scene, m_position);

		void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;

		if (m_position_in_instance_combo > -1)
		{
			QVariant v;
			v.setValue(static_cast<sad::db::Object*>(m_scene));

			ob->emitClosure( bind(
				ob->panel()->UI()->cmbAnimationInstanceObject, 
				f, 
				m_position_in_instance_combo,
				ob->panel()->nameForScene(m_scene),
				v
			));
		}
		for(size_t i = 0; i < m_scenenodes_positions.size(); i++)
		{
			sad::db::Object* object = m_scenenodes_positions[i].p1();
			int pos = m_scenenodes_positions[i].p2();

			QVariant v;
			v.setValue(object);

			ob->emitClosure( bind(
				ob->panel()->UI()->cmbAnimationInstanceObject, 
				f, 
				pos,
				ob->panel()->fullNameForNode(m_scenenodes_positions[i].p1()),
				v
			));
		}
		for(size_t i = 0; i < m_dependent_animations.size(); i++)
		{
			if (m_dependent_animations[i] == ob->shared()->selectedInstance())
			{
				ob->emitClosure( bind(ob->panel()->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, m_position_in_instance_combo));
			}
		}
	}
}
