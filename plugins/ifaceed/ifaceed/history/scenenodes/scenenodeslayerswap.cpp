#include "scenenodeslayerswap.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/sceneactions.h"
#include "../../gui/actions/scenenodeactions.h"

history::scenenodes::LayerSwap::LayerSwap(sad::SceneNode* n1, sad::SceneNode* n2, int l1, int l2)
: m_n1(n1), m_n2(n2), m_list_position1(l1), m_list_position2(l2)
{

}

history::scenenodes::LayerSwap::~LayerSwap()
{

}

void history::scenenodes::LayerSwap::commit(core::Editor * ob)
{
    m_n1->scene()->swapLayers(m_n1, m_n2);
    if (ob)
    {
        if (ob->actions()->sceneActions()->currentScene() == m_n1->scene())
        {
            ob->actions()->sceneNodeActions()->setSceneNodesInList(m_n1, m_n2, m_list_position2, m_list_position1);
        }
    }
}

void history::scenenodes::LayerSwap::rollback(core::Editor * ob)
{
    m_n1->scene()->swapLayers(m_n1, m_n2);
    if (ob)
    {
        if (ob->actions()->sceneActions()->currentScene() == m_n1->scene())
        {
            ob->actions()->sceneNodeActions()->setSceneNodesInList(m_n1, m_n2, m_list_position1, m_list_position2);
        }
    }
}
