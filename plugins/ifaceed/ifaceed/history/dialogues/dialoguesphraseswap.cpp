#include "dialoguesphraseswap.h"

#include "../mainpanel.h"

#include "../gui/dialogueactions.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

history::dialogues::PhraseSwap::PhraseSwap(
    sad::dialogue::Dialogue* dialogue, 
    int p1,
    int p2
) : m_dialogue(dialogue), m_position1(p1), m_position2(p2)
{
    m_dialogue->addRef();
}

history::dialogues::PhraseSwap::~PhraseSwap()
{
    m_dialogue->delRef();
}

void history::dialogues::PhraseSwap::commit(core::Editor * ob)
{
    sad::dialogue::Phrase p = *(m_dialogue->phrases()[m_position1]);
    *(m_dialogue->phrases()[m_position1]) = *(m_dialogue->phrases()[m_position2]);
    *(m_dialogue->phrases()[m_position2]) = p;
    if (ob)
    {
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->emitClosure(bind(
                ob->panel()->UI()->lstPhrases->item(m_position1),
                &QListWidgetItem::setText,
                ob->panel()->nameForPhrase(*(m_dialogue->phrases()[m_position1]))
            ));
            ob->emitClosure(bind(
                ob->panel()->UI()->lstPhrases->item(m_position2),
                &QListWidgetItem::setText,
                ob->panel()->nameForPhrase(*(m_dialogue->phrases()[m_position2]))
            ));

            if (m_position1 == ob->panel()->UI()->lstPhrases->currentRow())
            {
                ob->emitClosure(bind(
                    ob->panel()->dialogueActions(),
                    &gui::DialogueActions::viewPhrase,
                    m_dialogue->phrases()[m_position1]
                ));
            }
            if (m_position2 == ob->panel()->UI()->lstPhrases->currentRow())
            {
                ob->emitClosure(bind(
                    ob->panel()->dialogueActions(),
                    &gui::DialogueActions::viewPhrase,
                    m_dialogue->phrases()[m_position2]
                ));
            }
        }
    }
}

void history::dialogues::PhraseSwap::rollback(core::Editor * ob)
{
    this->commit(ob);
}
