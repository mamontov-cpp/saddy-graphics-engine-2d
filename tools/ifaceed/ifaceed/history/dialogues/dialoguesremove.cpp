#include "dialoguesremove.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

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
        void (gui::actions::DialogueActions::*f)(int) = &gui::actions::DialogueActions::removeDialogueFromDialogueList;
        ob->emitClosure( bind(ob->actions()->dialogueActions(), f, m_position) );
        if (ob->shared()->selectedDialogue() == m_d)
        {
            ob->shared()->setSelectedDialogue(nullptr);
        }
    }
}

void history::dialogues::Remove::rollback(core::Editor* ob)
{
    m_d->Active = true;
    if (ob)
    {
        ob->emitClosure( bind(ob->actions()->dialogueActions(), &gui::actions::DialogueActions::insertDialogueToDialogueList, m_d, m_position) );
    }
}

