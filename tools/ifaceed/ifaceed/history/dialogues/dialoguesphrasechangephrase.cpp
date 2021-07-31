#include "dialoguesphrasechangephrase.h"

#include <QPlainTextEdit>

#include "../../core/editor.h"

#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

history::dialogues::PhraseChangePhrase::PhraseChangePhrase(
    sad::dialogue::Dialogue* dialogue, 
    int i,
    const sad::String& old_value, 
    const sad::String& new_value
)
: m_dialogue(dialogue), m_position(i), m_old_value(old_value), m_new_value(new_value)
{
    m_dialogue->addRef();	
}

history::dialogues::PhraseChangePhrase::~PhraseChangePhrase()
{
    m_dialogue->delRef();
}

void history::dialogues::PhraseChangePhrase::commit(core::Editor * ob)
{
    m_dialogue->phrases()[m_position]->setPhrase(m_new_value);
    this->tryUpdateUI(ob, m_new_value);
}

void history::dialogues::PhraseChangePhrase::rollback(core::Editor * ob)
{
    m_dialogue->phrases()[m_position]->setPhrase(m_old_value);
    this->tryUpdateUI(ob, m_old_value);
}

void history::dialogues::PhraseChangePhrase::tryUpdateUI(core::Editor* e, const sad::String& v)
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
                e->emitClosure(::bind(
                    this,
                    &history::Command::blockedSetPlainTextEditText,
                    e->uiBlocks()->uiPhraseBlock()->txtPhrasePhrase,
                    STD2QSTRING(v.c_str())
                ));
            }
        }
    }
}
