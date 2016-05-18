#include "scenesremove.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"
#include "../../blockedclosuremethodcall.h"

#include ".././gui/uiblocks/uiblocks.h"
#include ".././gui/uiblocks/uianimationinstanceblock.h"

#include ".././gui/actions/actions.h"
#include ".././gui/actions/sceneactions.h"
#include ".././gui/actions/scenenodeactions.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

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
    const sad::Vector<sad::animations::Instance*>& dependent_animations,
    const sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> >& dependent_from_nodes
)
{
    m_position_in_instance_combo = position_in_instance_combo;
    m_scenenodes_positions = positions;
    m_dependent_animations = dependent_animations;
    m_dependent_from_nodes = dependent_from_nodes;
}

void history::scenes::Remove::setAffectedGrids(
    const sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> >& affected_grids
)
{
    m_affected_grids = affected_grids;
}

void history::scenes::Remove::commit(core::Editor * ob)
{
    m_scene->setActive(false);
    m_scene->Active = false;

    for(size_t i = 0; i < m_dependent_animations.size(); i++)
    {
        m_dependent_animations[i]->setObjectId(0);
    }
    for(size_t i = 0; i < m_dependent_from_nodes.size(); i++)
    {
        m_dependent_from_nodes[i].p1()->setObjectId(0);
    }
    if (ob)
    {
        ob->actions()->sceneActions()->removeSceneFromSceneList(m_position);
        int offset = 0;
        QComboBox* ai_objects = ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
        if (m_position_in_instance_combo > -1)
        {			
            ob->emitClosure( bind(ai_objects, &QComboBox::removeItem, m_position_in_instance_combo));
            ++offset;
        }
        for(size_t i = 0; i < m_scenenodes_positions.size(); i++)
        {
            ob->emitClosure( bind(ai_objects, &QComboBox::removeItem, m_scenenodes_positions[i].p2() - offset));
            ++offset;
        }
        for(size_t i = 0; i < m_dependent_animations.size(); i++)
        {
            if (m_dependent_animations[i] == ob->shared()->selectedInstance())
            {
                ob->emitClosure( blocked_bind(ai_objects, &QComboBox::setCurrentIndex, 0));
            }
        }
        for(size_t i = 0; i < m_dependent_from_nodes.size(); i++)
        {
            if (m_dependent_from_nodes[i].p1() == ob->shared()->selectedInstance())
            {
                ob->emitClosure( blocked_bind(ai_objects, &QComboBox::setCurrentIndex, 0));
            }
        }
        
        ob->actions()->gridActions()->eraseNodesFromGrids(m_affected_grids);
    }
}

void history::scenes::Remove::rollback(core::Editor * ob)
{
    m_scene->setActive(true);
    m_scene->Active = true;

    for(size_t i = 0; i < m_dependent_animations.size(); i++)
    {
        m_dependent_animations[i]->setObjectId(m_scene->MajorId);
    }
    for(size_t i = 0; i < m_dependent_from_nodes.size(); i++)
    {
        m_dependent_from_nodes[i].p1()->setObjectId(m_dependent_from_nodes[i].p2());
    }
    if (ob)
    {
        ob->actions()->sceneActions()->insertSceneToSceneList(m_scene, m_position);

        void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;

        if (m_position_in_instance_combo > -1)
        {
            QVariant v;
            v.setValue(static_cast<sad::db::Object*>(m_scene));

            ob->emitClosure( bind(
                ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject, 
                f, 
                m_position_in_instance_combo,
                ob->actions()->sceneActions()->nameForScene(m_scene),
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
                ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject,
                f, 
                pos,
                ob->actions()->sceneNodeActions()->fullNameForNode(m_scenenodes_positions[i].p1()),
                v
            ));
        }
        for(size_t i = 0; i < m_dependent_animations.size(); i++)
        {
            if (m_dependent_animations[i] == ob->shared()->selectedInstance())
            {
                ob->emitClosure( blocked_bind(ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, m_position_in_instance_combo));
            }
        }
        for(int i = 0; i < m_dependent_from_nodes.size(); i++)
        {
            if (m_dependent_from_nodes[i].p1() == ob->shared()->selectedInstance())
            {
                int pos = -1;
                for(size_t j = 0; (j < m_scenenodes_positions.size()) && pos == -1; j++)
                {
                    if (m_scenenodes_positions[j].p1()->MajorId == m_dependent_from_nodes[i].p2())
                    {
                        pos = m_scenenodes_positions[j].p2();
                    }
                }
                if (pos != -1)
                {
                    ob->emitClosure( blocked_bind(ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, pos));
                }
            }
        }

        ob->actions()->gridActions()->insertNodesToGrids(m_affected_grids);
    }
}
