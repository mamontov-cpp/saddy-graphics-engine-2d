#include "dialoguesremove.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::dialogues::Remove::Remove(sad::dialogue::Dialogue* w, int pos) : m_d(w), m_position(pos)
{
    m_d->addRef();
}

history::dialogues::Remove::~Remove()
{
    m_d->delRef();
}


void history::dialogues::Remove::commit(core::Editor* ob)
{
    m_d->Active = false;
    if (ob)
    {
        void (MainPanel::*f)(int) = &MainPanel::removeDialogueFromDialogueList;
        ob->emitClosure( bind(ob->panel(), f, m_position) );
        if (ob->shared()->selectedDialogue() == m_d)
        {
            ob->shared()->setSelectedDialogue(NULL);
        }
    }
}

void history::dialogues::Remove::rollback(core::Editor* ob)
{
    m_d->Active = true;
    if (ob)
    {
        ob->emitClosure( bind(ob->panel(), &MainPanel::insertDialogueToDialogueList, m_d, m_position) );
    }
}

