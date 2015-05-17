#include "wayswaypointremove.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::ways::WayPointRemove::WayPointRemove(
	sad::p2d::app::Way* w,
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
                ob->panel(),
                &MainPanel::removeRowInWayPointList,
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
            MainPanel* p = ob->panel();
			void (QListWidget::*f)(int, QListWidgetItem*) = &QListWidget::insertItem;
            ob->emitClosure( bind(
                p->UI()->lstWayPoints,
				f,
				m_position,
                new QListWidgetItem(p->nameForPoint(m_point))
            ));
        }
    }
}

