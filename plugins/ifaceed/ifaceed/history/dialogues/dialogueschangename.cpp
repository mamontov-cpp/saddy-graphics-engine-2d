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
        ob->emitClosure( bind(ob->actions()->dialogueActions(), &gui::actions::DialogueActions::updateDialogueName, m_dialogue) );
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->emitClosure(blocked_bind(
                ob->uiBlocks()->uiDialogueBlock()->txtDialogueName,
                &QLineEdit::setText,
                STD2QSTRING(m_newvalue.c_str())
            ));
        }
    }
}

void history::dialogues::ChangeName::rollback(core::Editor* ob)
{
    m_dialogue->setObjectName(m_oldvalue);
    if (ob)
    {
        ob->emitClosure( bind(ob->actions()->dialogueActions(), &gui::actions::DialogueActions::updateDialogueName, m_dialogue) );
        if (ob->shared()->selectedDialogue() == m_dialogue)
        {
            ob->emitClosure(blocked_bind(
                ob->uiBlocks()->uiDialogueBlock()->txtDialogueName,
                &QLineEdit::setText,
                STD2QSTRING(m_oldvalue.c_str())
            ));
        }
    }
}
