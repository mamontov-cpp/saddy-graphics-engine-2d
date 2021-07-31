#include "dialoguesphrasechangelineeditbasedproperty.h"

#include <QLineEdit>

#include "../../core/editor.h"
#include "../../qstdstring.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiphraseblock.h"

history::dialogues::PhraseChangeLineEditBasedProperty::PhraseChangeLineEditBasedProperty(
    QLineEdit* widget,
    void  (sad::dialogue::Phrase::*setter)(const sad::String&),
    bool update,
    sad::dialogue::Dialogue* dialogue, 
    int i,
    const sad::String& old_value, 
    const sad::String& new_value
)
: m_widget(widget),  
m_setter(setter), 
m_update(update), 
m_dialogue(dialogue), 
m_position(i), 
m_old_value(old_value), 
m_new_value(new_value)
{
    m_dialogue->addRef();	
}

history::dialogues::PhraseChangeLineEditBasedProperty::~PhraseChangeLineEditBasedProperty()
{
    m_dialogue->delRef();
}

void history::dialogues::PhraseChangeLineEditBasedProperty::commitWithoutUpdatingUI(core::Editor * ob)
{
    ((m_dialogue->phrases()[m_position])->*m_setter)(m_new_value);
    this->tryUpdateUI(ob, m_new_value, false);
}

void history::dialogues::PhraseChangeLineEditBasedProperty::commit(core::Editor * ob)
{
    ((m_dialogue->phrases()[m_position])->*m_setter)(m_new_value);
    this->tryUpdateUI(ob, m_new_value, true);
}

void history::dialogues::PhraseChangeLineEditBasedProperty::rollback(core::Editor * ob)
{
    ((m_dialogue->phrases()[m_position])->*m_setter)(m_old_value);
    this->tryUpdateUI(ob, m_old_value, true);
}

void history::dialogues::PhraseChangeLineEditBasedProperty::tryUpdateUI(core::Editor* e, const sad::String& v, bool update_ui) const
{
    if (e)
    {
        if (e->shared()->selectedDialogue() == m_dialogue)
        {
            QListWidget* lst = e->uiBlocks()->uiPhraseBlock()->lstPhrases;
            
            if (m_update)
            {
                e->emitClosure(::bind(
                    lst->item(m_position),
                    &QListWidgetItem::setText,
                    e->actions()->dialogueActions()->nameForPhrase(*(m_dialogue->phrases()[m_position]))
                ));
            }
            if ((lst->currentRow() == m_position) && update_ui)
            {
                e->emitClosure(::bind(
                    this,
                    &history::Command::blockedSetLineEditText,
                    m_widget,
                    STD2QSTRING(v)
                ));
            }
        }
    }
}
