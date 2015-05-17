#include "dialoguesnew.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"

history::dialogues::New::New(sad::dialogue::Dialogue* dialogue) : m_dialogue(dialogue)
{
    m_dialogue->addRef();
}

history::dialogues::New::~New()
{
    m_dialogue->delRef();
}


void history::dialogues::New::commit(core::Editor* ob)
{
    m_dialogue->Active = true;
    if (ob)
    {
		ob->emitClosure( bind(ob->panel(), &MainPanel::addDialogueToDialogueList, m_dialogue) );
    }
}

void history::dialogues::New::rollback(core::Editor* ob)
{
    m_dialogue->Active = false;
    if (ob)
    {
		ob->emitClosure( bind(ob->panel(), &MainPanel::removeLastDialogueFromDialogueList) );
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->shared()->setSelectedDialogue(NULL);
        }
    }
}
