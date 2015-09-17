#include "wayswaypointswap.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

history::ways::WayPointSwap::WayPointSwap(
    sad::p2d::app::Way* w,
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
        MainPanel* panel = ob->panel();
        QListWidget* list = panel->UI()->lstWayPoints;
        int currentrow = list->currentRow();
        const sad::Point2D& p1 = m_way->wayPoints()[m_pos1];
        const sad::Point2D& p2 = m_way->wayPoints()[m_pos2];
        list->item(m_pos1)->setText(panel->nameForPoint(p1));
        list->item(m_pos2)->setText(panel->nameForPoint(p2));
        if (currentrow == m_pos1)
        {
            invoke_blocked(panel->UI()->dsbWayPointX, &QDoubleSpinBox::setValue, p1.x());
            invoke_blocked(panel->UI()->dsbWayPointY, &QDoubleSpinBox::setValue, p1.y());
        }
        if (currentrow == m_pos2)
        {
            invoke_blocked(panel->UI()->dsbWayPointX, &QDoubleSpinBox::setValue, p2.x());
            invoke_blocked(panel->UI()->dsbWayPointY, &QDoubleSpinBox::setValue, p2.y());
        }
    }
}
