#include "wayswaypointremove.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"


history::ways::WayPointRemove::WayPointRemove(
    sad::Way* w,
    int position
) : m_way(w), m_position(position)
{
    m_way->addRef();
    m_point = m_way->wayPoints()[position];
}

history::ways::WayPointRemove::~WayPointRemove()
{
    m_way->delRef();
}


void history::ways::WayPointRemove::commit(core::Editor* ob)
{
    sad::Renderer::ref()->lockRendering();
    m_way->removePoint(m_position);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        if (ob->shared()->selectedWay() == m_way)
        {
             ob->emitClosure(bind(
                ob->actions()->wayActions(),
                &gui::actions::WayActions::removeRowInWayPointList,
                m_position
            ));
        }
    }
}

void history::ways::WayPointRemove::rollback(core::Editor* ob)
{
    sad::Renderer::ref()->lockRendering();
    m_way->insertPoint(m_position, m_point);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        if (ob->shared()->selectedWay() == m_way)
        {
            gui::uiblocks::UIWayBlock* blk = ob->uiBlocks()->uiWayBlock();			
            void (QListWidget::*f)(int, QListWidgetItem*) = &QListWidget::insertItem;
            ob->emitClosure( bind(
                blk->lstWayPoints,
                f,
                m_position,
                new QListWidgetItem(ob->actions()->wayActions()->nameForPoint(m_point))
            ));
        }
    }
}

