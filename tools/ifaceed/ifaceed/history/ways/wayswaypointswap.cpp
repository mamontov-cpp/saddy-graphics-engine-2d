#include "wayswaypointswap.h"

#include <QDoubleSpinBox>

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

history::ways::WayPointSwap::WayPointSwap(
    sad::Way* w,
    int pos1,
    int pos2
) : m_way(w), m_pos1(pos1), m_pos2(pos2)
{
    m_way->addRef();
}

history::ways::WayPointSwap::~WayPointSwap()
{
    m_way->delRef();
}


void history::ways::WayPointSwap::commit(core::Editor* ob)
{
    sad::Renderer::ref()->lockRendering();
    sad::Point2D p = m_way->wayPoints()[m_pos1];
    m_way->setPoint(m_pos1, m_way->wayPoints()[m_pos2]);
    m_way->setPoint(m_pos2, p);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        ob->emitClosure(bind(this, &history::ways::WayPointSwap::tryUpdateUI, ob));
    }
}

void history::ways::WayPointSwap::rollback(core::Editor* ob) //-V524
{
    sad::Renderer::ref()->lockRendering();
    sad::Point2D p = m_way->wayPoints()[m_pos1];
    m_way->setPoint(m_pos1, m_way->wayPoints()[m_pos2]);
    m_way->setPoint(m_pos2, p);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        ob->emitClosure(bind(this, &history::ways::WayPointSwap::tryUpdateUI, ob));
    }
}

void history::ways::WayPointSwap::tryUpdateUI(core::Editor* ob)
{
    if (ob->shared()->selectedWay() == m_way)
    {
        gui::uiblocks::UIWayBlock* blk = ob->uiBlocks()->uiWayBlock();				
        
        gui::actions::WayActions* wa = ob->actions()->wayActions();				

            
        QListWidget* list = blk->lstWayPoints;
        int currentrow = list->currentRow();
        const sad::Point2D& p1 = m_way->wayPoints()[m_pos1];
        const sad::Point2D& p2 = m_way->wayPoints()[m_pos2];
        list->item(m_pos1)->setText(wa->nameForPoint(p1));
        list->item(m_pos2)->setText(wa->nameForPoint(p2));
        if (currentrow == m_pos1)
        {
            invoke_blocked(blk->dsbWayPointX, &QDoubleSpinBox::setValue, p1.x());
            invoke_blocked(blk->dsbWayPointY, &QDoubleSpinBox::setValue, p1.y());
        }
        if (currentrow == m_pos2)
        {
            invoke_blocked(blk->dsbWayPointX, &QDoubleSpinBox::setValue, p2.x());
            invoke_blocked(blk->dsbWayPointY, &QDoubleSpinBox::setValue, p2.y());
        }
    }
}
