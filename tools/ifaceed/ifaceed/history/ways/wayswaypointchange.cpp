#include "wayswaypointchange.h"

#include <QListWidget>
#include <QDoubleSpinBox>

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"

history::ways::WayPointChange::WayPointChange(
    sad::Way* w, 
    int pos, 
    const sad::Point2D& old_value, 
    const sad::Point2D& new_value
) : m_way(w), m_position(pos), m_old_point(old_value), m_new_point(new_value)
{
    m_way->addRef();
}

history::ways::WayPointChange::~WayPointChange()
{
    m_way->delRef();
}

void history::ways::WayPointChange::commit(core::Editor * ob)
{
    m_way->setPoint(m_position, m_new_point);
    if (ob)
    {
        ob->emitClosure(bind(this, &history::ways::WayPointChange::tryUpdateUI, ob, m_new_point));
    }
}

void history::ways::WayPointChange::rollback(core::Editor * ob)
{
    m_way->setPoint(m_position, m_old_point);
    if (ob)
    {
        ob->emitClosure(bind(this, &history::ways::WayPointChange::tryUpdateUI, ob, m_old_point));
    }
}

void history::ways::WayPointChange::tryUpdateUI(core::Editor* ob, const sad::Point2D& p)
{
    if (ob->shared()->selectedWay() == m_way)
    {
        gui::uiblocks::UIWayBlock* blk = ob->uiBlocks()->uiWayBlock();
        blk->lstWayPoints->item(m_position)->setText(ob->actions()->wayActions()->nameForPoint(p));
        if (m_position == blk->lstWayPoints->currentRow())
        {
            invoke_blocked(blk->dsbWayPointX, &QDoubleSpinBox::setValue, p.x());
            invoke_blocked(blk->dsbWayPointY, &QDoubleSpinBox::setValue, p.y());
        }
    }
}