#include "dialoguesphraseswap.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

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
            QListWidget* lst = ob->uiBlocks()->uiPhraseBlock()->lstPhrases;	
			gui::actions::DialogueActions* d_actions = ob->actions()->dialogueActions(); 
            ob->emitClosure(bind(
                lst->item(m_position1),
                &QListWidgetItem::setText,
                d_actions->nameForPhrase(*(m_dialogue->phrases()[m_position1]))
            ));
            ob->emitClosure(bind(
                lst->item(m_position2),
                &QListWidgetItem::setText,
                d_actions->nameForPhrase(*(m_dialogue->phrases()[m_position2]))
            ));

            if (m_position1 == lst->currentRow())
            {
                ob->emitClosure(bind(
                    d_actions,
                    &gui::actions::DialogueActions::viewPhrase,
                    m_dialogue->phrases()[m_position1]
                ));
            }
            if (m_position2 == lst->currentRow())
            {
                ob->emitClosure(bind(
                    d_actions,
                    &gui::actions::DialogueActions::viewPhrase,
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
