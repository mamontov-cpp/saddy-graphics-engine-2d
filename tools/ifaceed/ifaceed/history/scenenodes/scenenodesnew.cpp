#include "scenenodesnew.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include ".././gui/uiblocks/uiblocks.h"
#include ".././gui/uiblocks/uianimationinstanceblock.h"

#include ".././gui/actions/actions.h"
#include ".././gui/actions/sceneactions.h"
#include ".././gui/actions/scenenodeactions.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

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
    if (!ob)
        return;
    
    gui::actions::SceneNodeActions* sn_actions = ob->actions()->sceneNodeActions();
    gui::actions::SceneActions* s_actions = ob->actions()->sceneActions();
    
    m_node->setActive(true);
    if (m_node->scene() == s_actions->currentScene())
    {
        sn_actions->addSceneNodeToSceneNodeList(m_node);
    }

    QVariant v;
    v.setValue(static_cast<sad::db::Object*>(m_node));
    if(ob)
    {
        void (QComboBox::*f)(const QString&, const QVariant&) = &QComboBox::addItem;
        ob->emitClosure( bind(
            ob->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject,
            f,
            sn_actions->fullNameForNode(m_node),
            v
        ));
    }
}

void history::scenenodes::New::rollback(core::Editor * ob)
{
    m_node->setActive(false);
    
    gui::actions::SceneNodeActions* sn_actions = ob->actions()->sceneNodeActions();
    gui::actions::SceneActions* s_actions = ob->actions()->sceneActions();
    
    
    if (m_node->scene() == s_actions->currentScene())
    {
        sn_actions->removeLastSceneNodeFromSceneNodeList();
    }
    if (ob->shared()->selectedObject() == m_node)
    {
        ob->shared()->setSelectedObject(nullptr);
        if (ob->machine()->isInState("selected"))
        {
            ob->machine()->enterState("idle");
        }
    }
    ob->emitClosure( bind(
            this,
            &history::scenenodes::New::eraseFromAnimationObjectsCombo,
            ob
    ));
}

void history::scenenodes::New::eraseFromAnimationObjectsCombo(core::Editor* e)
{
    QComboBox* ai_objects = e->uiBlocks()->uiAnimationInstanceBlock()->cmbAnimationInstanceObject;
    gui::actions::SceneNodeActions* sn_actions = e->actions()->sceneNodeActions();
    int pos  = sn_actions->findInComboBox<sad::db::Object*>(ai_objects, m_node);
    if (pos > -1)
    {
        ai_objects->removeItem(pos);
    }
}

