#include "sceneschangename.h"

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include <db/dbobject.h>

#include ".././gui/uiblocks/uiblocks.h"
#include ".././gui/uiblocks/uianimationinstanceblock.h"

#include ".././gui/actions/actions.h"
#include ".././gui/actions/sceneactions.h"
#include ".././gui/actions/scenenodeactions.h"
#include ".././gui/actions/gridactions.h"

Q_DECLARE_METATYPE(sad::db::Object*) //-V566

history::scenes::ChangeName::ChangeName(
    sad::Scene * s, 
    const sad::String& old_name, 
    const sad::String& new_name
) : m_scene(s), m_old(old_name), m_new(new_name) 
{
    m_scene->addRef();
}

history::scenes::ChangeName::~ChangeName()
{
    m_scene->delRef();
}

void history::scenes::ChangeName::commitWithoutUpdatingUI(core::Editor * ob)
{
    this->commit(ob, false);
}

void history::scenes::ChangeName::commit(core::Editor * ob)
{
   this->commit(ob, true);
}

void history::scenes::ChangeName::rollback(core::Editor * ob)
{
    m_scene->setObjectName(m_old);
    if (ob)
    {
        gui::actions::SceneActions* s_actions = ob->actions()->sceneActions();

        s_actions->updateSceneName(m_scene);
        ob->emitClosure( bind(this, &history::scenes::ChangeName::updateDependent, ob));
    }
}

void history::scenes::ChangeName::commit(core::Editor* ob, bool update_ui)
{
    m_scene->setObjectName(m_new);
    if (ob)
    {
        gui::actions::SceneActions* s_actions = ob->actions()->sceneActions();

        s_actions->updateSceneName(m_scene, update_ui);
        ob->emitClosure( bind(this, &history::scenes::ChangeName::updateDependent, ob));
    }
}

void history::scenes::ChangeName::updateDependent(core::Editor* e)
{
    gui::uiblocks::UIAnimationInstanceBlock* ai_blk = e->uiBlocks()->uiAnimationInstanceBlock();
    
    gui::actions::SceneActions* s_actions = e->actions()->sceneActions();
    gui::actions::SceneNodeActions* sn_actions = e->actions()->sceneNodeActions();
    gui::actions::GridActions* ga_actions = e->actions()->gridActions();
    
    int pos = s_actions->findInComboBox<sad::db::Object*>(ai_blk->cmbAnimationInstanceObject, m_scene);
    if (pos > - 1)
    {
        ai_blk->cmbAnimationInstanceObject->setItemText(pos, s_actions->nameForScene(m_scene));
    }
    sad::Vector<sad::SceneNode*> nodes = m_scene->objects();
    for(size_t i = 0; i < nodes.size(); i++)
    {
        pos = s_actions->findInComboBox<sad::db::Object*>(ai_blk->cmbAnimationInstanceObject, nodes[i]);
        if (pos > - 1)
        {
            ai_blk->cmbAnimationInstanceObject->setItemText(pos, sn_actions->fullNameForNode(nodes[i]));
        }
        ga_actions->tryUpdateNodeNameInGrid(nodes[i]);
    }
}
