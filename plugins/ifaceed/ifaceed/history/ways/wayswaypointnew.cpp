#include "wayswaypointnew.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::ways::WayPointNew::WayPointNew(sad::p2d::app::Way* w) : m_way(w)
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
            MainPanel* p = ob->panel();
            void (QListWidget::*f)(const QString&) = &QListWidget::addItem;
            ob->emitClosure( bind(
                p->UI()->lstWayPoints,
                f,
                p->nameForPoint(m_point)
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
                ob->panel(),
                &MainPanel::removeRowInWayPointList,
                row
            ));
        }
    }
}

