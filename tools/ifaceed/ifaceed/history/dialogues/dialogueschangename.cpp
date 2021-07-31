#include "dialogueschangename.h"

#include <QLineEdit>

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../../qstdstring.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uidialogueblock.h"

// ========================= PUBLIC METHODS =========================

history::dialogues::ChangeName::ChangeName(
    sad::dialogue::Dialogue* dialogue,
    const sad::String& old_value,
    const sad::String& new_value
) : m_dialogue(dialogue), m_old_value(old_value), m_new_value(new_value)
{
    m_dialogue->addRef();
}

history::dialogues::ChangeName::~ChangeName()
{
    m_dialogue->delRef();
}


void history::dialogues::ChangeName::commit(core::Editor* ob)
{
    m_dialogue->setObjectName(m_new_value);
    if (ob)
    {
        ob->emitClosure( ::bind(this, &history::dialogues::ChangeName::updateUI, ob, m_new_value) );
    }
}

void history::dialogues::ChangeName::rollback(core::Editor* ob)
{
    m_dialogue->setObjectName(m_old_value);
    if (ob)
    {
        ob->emitClosure( ::bind(this, &history::dialogues::ChangeName::updateUI, ob, m_old_value) );
    }
}

// ========================= PROTECTED METHODS =========================

void history::dialogues::ChangeName::updateUI(core::Editor* e, const sad::String& str) const
{
    e->actions()->dialogueActions()->updateDialogueName(m_dialogue);
    if (e->shared()->selectedDialogue() == m_dialogue)
    {
        this->blockedSetLineEditText(
            e->uiBlocks()->uiDialogueBlock()->txtDialogueName,
            STD2QSTRING(str)
        );
    }
}
