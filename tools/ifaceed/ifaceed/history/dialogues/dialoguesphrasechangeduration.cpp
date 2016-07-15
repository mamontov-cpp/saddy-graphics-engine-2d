#include "dialoguesphrasechangeduration.h"

#include <QDoubleSpinBox>

#include "../../core/editor.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

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
            QListWidget* lst = e->uiBlocks()->uiPhraseBlock()->lstPhrases;
            e->emitClosure(bind(
                lst->item(m_position),
                &QListWidgetItem::setText,
                e->actions()->dialogueActions()->nameForPhrase(*(m_dialogue->phrases()[m_position]))
            ));
            if (lst->currentRow() == m_position)
            {
                e->emitClosure(blocked_bind(
                    e->uiBlocks()->uiPhraseBlock()->dsbPhraseDuration,
                    &QDoubleSpinBox::setValue,
                    v
                ));
            }
        }
    }
}
