#include "history/dialogues/dialoguesphrasenew.h"

#include "../mainpanel.h"
#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

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
            MainPanel* p = ob->panel();
            void (QListWidget::*f)(const QString&) = &QListWidget::addItem;
            ob->emitClosure( bind(
                p->UI()->lstPhrases,
                f,
                p->nameForPhrase(m_phrase)
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
                ob->panel(),
				&MainPanel::removePhraseFromPhraseList,
                row
            ));
        }
    }
}

