#include "scenenodesremove.h"

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/sceneactions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/gridactions.h"

#include "../../closuremethodcall.h"

#include <renderer.h>

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

history::scenenodes::Remove::Remove(sad::SceneNode* d, int position)
: m_node(d), m_position(position), m_position_in_instance_combo(-1)
{
    m_node->addRef();
}

history::scenenodes::Remove::~Remove()
{
    m_node->delRef();
}

void history::scenenodes::Remove::set(
    int  position_in_instance_combo,
    const sad::Vector< sad::animations::Instance* >& instances
)
{
    m_position_in_instance_combo = position_in_instance_combo;
    m_dependent_instances = instances;
}

void history::scenenodes::Remove::set(const sad::Vector< gui::GridPosition >& grids)
{
    m_dependent_grids = grids;
}

void history::scenenodes::Remove::commit(core::Editor * ob)
{
    m_node->setActive(false);
    if (ob)
    {
        if (ob->actions()->sceneActions()->currentScene() == m_node->scene())
        {			
            ob->emitClosure( bind(ob->actions()->sceneNodeActions(), &gui::actions::SceneNodeActions::removeSceneNodeFromSceneNodeList, m_position) );
        }
        if (ob->shared()->selectedObject() == m_node)
        {
            ob->shared()->setSelectedObject(nullptr);
            if (ob->machine()->isInState("selected"))
            {
                ob->machine()->enterState("idle");
            }
        }
        QComboBox* ai_object = ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
        if (m_position_in_instance_combo > -1)
        {
            ob->emitClosure( bind(ai_object, &QComboBox::removeItem, m_position_in_instance_combo));
        }
        for(size_t i = 0; i < m_dependent_instances.size(); i++)
        {
            m_dependent_instances[i]->setObjectId(0);
            if (m_dependent_instances[i] == ob->shared()->selectedInstance())
            {
                ob->emitClosure( bind(ai_object, &QComboBox::setCurrentIndex, 0));
            }
        }
        gui::actions::GridActions* ga = ob->actions()->gridActions();
        for(size_t i = 0; i < m_dependent_grids.size(); i++)
        {
            ga->removeChildFromGrid(
                m_dependent_grids[i].Grid,
                m_dependent_grids[i].Row,
                m_dependent_grids[i].Col,
                m_dependent_grids[i].Pos
            );
        }
        ga->eraseNodeToGridEntry(m_node);
    }
}


void history::scenenodes::Remove::rollback(core::Editor * ob)
{
    m_node->setActive(true);
    if (ob)
    {
        QComboBox* ai_object = ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
            
        ob->emitClosure( bind(ob->actions()->sceneNodeActions(), &gui::actions::SceneNodeActions::insertSceneNodeToSceneNodeList, m_node, m_position) );
        if (m_position_in_instance_combo > -1)
        {
            QVariant v;
            v.setValue(static_cast<sad::db::Object*>(m_node));

            void (QComboBox::*f)(int, const QString&, const QVariant&) = &QComboBox::insertItem;
            ob->emitClosure( bind(
                ai_object, 
                f, 
                m_position_in_instance_combo,
                ob->actions()->sceneNodeActions()->fullNameForNode(m_node),
                v
            ));
        }
        for(size_t i = 0; i < m_dependent_instances.size(); i++)
        {
            m_dependent_instances[i]->setObjectId(m_node->MajorId);
            if (m_dependent_instances[i] == ob->shared()->selectedInstance())
            {
                ob->emitClosure( bind(ai_object, &QComboBox::setCurrentIndex, m_position_in_instance_combo));
            }
        }

        gui::actions::GridActions* ga = ob->actions()->gridActions();
        for(size_t i = 0; i < m_dependent_grids.size(); i++)
        {
            ga->insertChildToGrid(
                m_dependent_grids[i].Grid,
                m_dependent_grids[i].Row,
                m_dependent_grids[i].Col,
                m_dependent_grids[i].Pos,
                m_node
            );
            ga->insertNodeToGridEntry(m_node, m_dependent_grids[i].Grid);
        }
    }
}
