#include "dialoguesphraseremove.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

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
                ob->panel(),
                &MainPanel::removePhraseFromPhraseList,
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
            void (QListWidget::*f)(int, const QString&) = &QListWidget::insertItem;
            MainPanel* p = ob->panel();            
            ob->emitClosure( bind(
                p->UI()->lstPhrases,
                f,
                m_position, 
                p->nameForPhrase(m_phrase)
            ));
        }
    }
}
