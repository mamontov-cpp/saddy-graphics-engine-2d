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
    const sad::String& oldvalue,
    const sad::String& newvalue
) : m_dialogue(dialogue), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
    m_dialogue->addRef();
}

history::dialogues::ChangeName::~ChangeName()
{
    m_dialogue->delRef();
}


void history::dialogues::ChangeName::commit(core::Editor* ob)
{
    m_dialogue->setObjectName(m_newvalue);
    if (ob)
    {
        ob->emitClosure( bind(this, &history::dialogues::ChangeName::updateUI, ob, m_newvalue) );
    }
}

void history::dialogues::ChangeName::rollback(core::Editor* ob)
{
    m_dialogue->setObjectName(m_oldvalue);
    if (ob)
    {
        ob->emitClosure( bind(this, &history::dialogues::ChangeName::updateUI, ob, m_oldvalue) );
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
