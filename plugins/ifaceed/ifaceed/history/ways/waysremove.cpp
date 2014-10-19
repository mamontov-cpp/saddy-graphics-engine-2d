#include "waysremove.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::ways::Remove::Remove(sad::p2d::app::Way* w, int pos) : m_way(w), m_position(pos)
{
    m_way->addRef();
}

history::ways::Remove::~Remove()
{
    m_way->delRef();
}


void history::ways::Remove::commit(core::Editor* ob)
{
    m_way->Active = false;
    if (ob)
    {
        void (MainPanel::*f)(int) = &MainPanel::removeWayFromWayList;
        ob->emitClosure( bind(ob->panel(), f, m_position) );
        if (ob->shared()->selectedWay() == m_way)
        {
            ob->machine()->enterState("ways/idle");
            ob->shared()->setSelectedWay(NULL);
        }
    }
}

void history::ways::Remove::rollback(core::Editor* ob)
{
    m_way->Active = true;
    if (ob)
    {
        ob->emitClosure( bind(ob->panel(), &MainPanel::insertWayToWayList, m_way, m_position) );
    }
}

