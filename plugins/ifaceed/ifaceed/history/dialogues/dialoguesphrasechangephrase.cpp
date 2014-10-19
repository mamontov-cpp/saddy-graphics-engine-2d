#include "dialoguesphrasechangephrase.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::dialogues::PhraseChangePhrase::PhraseChangePhrase(
	sad::dialogue::Dialogue* dialogue, 
	int i,
	const sad::String& oldvalue, 
	const sad::String& newvalue
)
: m_dialogue(dialogue), m_position(i), m_oldvalue(oldvalue), m_newvalue(newvalue)
{
	m_dialogue->addRef();	
}

history::dialogues::PhraseChangePhrase::~PhraseChangePhrase()
{
	m_dialogue->delRef();
}

void history::dialogues::PhraseChangePhrase::commit(core::Editor * ob)
{
	m_dialogue->phrases()[m_position]->setPhrase(m_newvalue);
	this->tryUpdateUI(ob, m_newvalue);
}

void history::dialogues::PhraseChangePhrase::rollback(core::Editor * ob)
{
	m_dialogue->phrases()[m_position]->setPhrase(m_oldvalue);
	this->tryUpdateUI(ob, m_oldvalue);
}

void history::dialogues::PhraseChangePhrase::tryUpdateUI(core::Editor* e, const sad::String& v)
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
					e->panel()->UI()->txtPhrasePhrase,
					&QPlainTextEdit::setPlainText,
					v.c_str()
				));
			}
		}
	}
}
