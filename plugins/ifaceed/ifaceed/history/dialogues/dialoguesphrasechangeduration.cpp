#include "dialoguesphrasechangeduration.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::dialogues::PhraseChangeDuration::PhraseChangeDuration(
    sad::dialogue::Dialogue* dialogue, 
    int i,
    double oldvalue, 
    double newvalue
)
: m_dialogue(dialogue), m_position(i), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_dialogue->addRef();	
}

history::dialogues::PhraseChangeDuration::~PhraseChangeDuration()
{
    m_dialogue->delRef();
}

void history::dialogues::PhraseChangeDuration::commit(core::Editor * ob)
{
    m_dialogue->phrases()[m_position]->setDuration(m_newvalue);
    this->tryUpdateUI(ob, m_newvalue);
}

void history::dialogues::PhraseChangeDuration::rollback(core::Editor * ob)
{
    m_dialogue->phrases()[m_position]->setDuration(m_oldvalue);
    this->tryUpdateUI(ob, m_oldvalue);
}

void history::dialogues::PhraseChangeDuration::tryUpdateUI(core::Editor* e, double v)
{
    if (e)
    {
        if (e->shared()->selectedDialogue() == m_dialogue)
        {
            e->emitClosure(bind(
                e->panel()->UI()->lstPhrases->item(m_position),
                &QListWidgetItem::setText,
                e->panel()->nameForPhrase(*(m_dialogue->phrases()[m_position]))
            ));
            if (e->panel()->UI()->lstPhrases->currentRow() == m_position)
            {
                e->emitClosure(blocked_bind(
                    e->panel()->UI()->dsbPhraseDuration,
                    &QDoubleSpinBox::setValue,
                    v
                ));
            }
        }
    }
}
