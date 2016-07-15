#include "dialoguesphraseremove.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

history::dialogues::PhraseRemove::PhraseRemove(
    sad::dialogue::Dialogue* dialogue, 
    int position
) : m_dialogue(dialogue), m_position(position)
{
    m_dialogue->addRef();
    m_phrase = *(m_dialogue->phrases()[position]);
}

history::dialogues::PhraseRemove::~PhraseRemove()
{
    m_dialogue->delRef();
}

void history::dialogues::PhraseRemove::commit(core::Editor* ob)
{
    delete m_dialogue->phrases()[m_position];
    m_dialogue->phrases().removeAt(m_position);
    if (ob)
    {
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->emitClosure(bind(
                ob->actions()->dialogueActions(),
                &gui::actions::DialogueActions::removePhraseFromPhraseList,
                m_position
            ));
        }
    }
}


void history::dialogues::PhraseRemove::rollback(core::Editor* ob)
{
    m_dialogue->phrases().insert(new sad::dialogue::Phrase(m_phrase), m_position);
    if (ob)
    {
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            QListWidget* lst = ob->uiBlocks()->uiPhraseBlock()->lstPhrases;			
            void (QListWidget::*f)(int, const QString&) = &QListWidget::insertItem;
            ob->emitClosure( bind(
                lst,
                f,
                m_position, 
                ob->actions()->dialogueActions()->nameForPhrase(m_phrase)
            ));
        }
    }
}
