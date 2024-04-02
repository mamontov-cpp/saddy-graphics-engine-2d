#include "wayswaypointnew.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/wayactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiwayblock.h"


history::ways::WayPointNew::WayPointNew(sad::Way* w) : m_way(w)
{
    m_way->addRef();

    const sad::Settings& s =  sad::Renderer::ref()->settings();
    double x =  s.width() / 2.0;
    double y =  s.height() / 2.0;
    m_point.setX(x);
    m_point.setY(y);
}

history::ways::WayPointNew::~WayPointNew()
{
    m_way->delRef();
}


void history::ways::WayPointNew::commit(core::Editor* ob)
{
    sad::Renderer::ref()->lockRendering();
    m_way->addPoint(m_point);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        if (ob->shared()->selectedWay() == m_way)
        {
            void (QListWidget::*f)(const QString&) = &QListWidget::addItem;
            ob->emitClosure( bind(
                ob->uiBlocks()->uiWayBlock()->lstWayPoints,
                f,
                ob->actions()->wayActions()->nameForPoint(m_point)
            ));
        }
    }
}

void history::ways::WayPointNew::rollback(core::Editor* ob)
{
    int row = m_way->wayPoints().size() - 1;
    sad::Renderer::ref()->lockRendering();
    m_way->removePoint(row);
    sad::Renderer::ref()->unlockRendering();
    if (ob)
    {
        if (ob->shared()->selectedWay() == m_way)
        {
            ob->emitClosure(bind(
                ob->actions()->wayActions(),
                &gui::actions::WayActions::removeRowInWayPointList,
                row
            ));
        }
    }
}

void history::ways::WayPointNew::setPoint(const sad::Point2D& point)
{
    m_point = point;
}
