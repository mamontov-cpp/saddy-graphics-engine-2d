#include "waysnew.h"

#include "../mainpanel.h"
#include "../core/editor.h"

history::ways::New::New(sad::p2d::app::Way* w) : m_way(w)
{
    m_way->addRef();
}

history::ways::New::~New()
{
    m_way->delRef();
}


void history::ways::New::commit(core::Editor* ob)
{
    m_way->Active = true;
    if (ob)
    {
        ob->panel()->addLastWayToEnd(m_way);
    }
}

void history::ways::New::rollback(core::Editor* ob)
{
    m_way->Active = false;
    if (ob)
    {
        ob->panel()->removeLastWayFromWayList();
        if (ob->shared()->selectedWay() == m_way)
        {
            ob->machine()->enterState("ways/idle");
            ob->shared()->setSelectedWay(NULL);
        }
    }
}
