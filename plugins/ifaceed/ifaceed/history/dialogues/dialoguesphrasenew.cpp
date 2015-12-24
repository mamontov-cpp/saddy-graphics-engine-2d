#include "history/dialogues/dialoguesphrasenew.h"

#include <QListWidget>

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

history::dialogues::PhraseNew::PhraseNew(
    sad::dialogue::Dialogue* dialogue, 
    const sad::dialogue::Phrase& p
) : m_dialogue(dialogue), m_phrase(p)
{
    m_dialogue->addRef();
}

history::dialogues::PhraseNew::~PhraseNew()
{
    m_dialogue->delRef();
}


void history::dialogues::PhraseNew::commit(core::Editor* ob)
{
    m_dialogue->phrases() << new sad::dialogue::Phrase(m_phrase);
    if (ob)
    {
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            QListWidget* lst = ob->uiBlocks()->uiPhraseBlock()->lstPhrases;
            void (QListWidget::*f)(const QString&) = &QListWidget::addItem;
            ob->emitClosure( bind(
                lst,
                f,
                ob->actions()->dialogueActions()->nameForPhrase(m_phrase)
            ));
        }
    }
}

void history::dialogues::PhraseNew::rollback(core::Editor* ob)
{
    int row = m_dialogue->phrases().size() - 1;
    delete m_dialogue->phrases()[row];
    m_dialogue->phrases().removeAt(row);
    if (ob)
    {
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->emitClosure(bind(
                ob->actions()->dialogueActions(),
                &gui::actions::DialogueActions::removePhraseFromPhraseList,
                row
            ));
        }
    }
}

