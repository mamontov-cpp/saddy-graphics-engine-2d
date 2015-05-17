#include "dialoguesphrasechangelineeditbasedproperty.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::dialogues::PhraseChangeLineEditBasedProperty::PhraseChangeLineEditBasedProperty(
	QLineEdit* widget,
	void  (sad::dialogue::Phrase::*setter)(const sad::String&),
	bool update,
	sad::dialogue::Dialogue* dialogue, 
	int i,
	const sad::String& oldvalue, 
	const sad::String& newvalue
)
: m_widget(widget),  
m_setter(setter), 
m_update(update), 
m_dialogue(dialogue), 
m_position(i), 
m_oldvalue(oldvalue), 
m_newvalue(newvalue)
{
	m_dialogue->addRef();	
}

history::dialogues::PhraseChangeLineEditBasedProperty::~PhraseChangeLineEditBasedProperty()
{
	m_dialogue->delRef();
}

void history::dialogues::PhraseChangeLineEditBasedProperty::commit(core::Editor * ob)
{
	((m_dialogue->phrases()[m_position])->*m_setter)(m_newvalue);
	this->tryUpdateUI(ob, m_newvalue);
}

void history::dialogues::PhraseChangeLineEditBasedProperty::rollback(core::Editor * ob)
{
	((m_dialogue->phrases()[m_position])->*m_setter)(m_oldvalue);
	this->tryUpdateUI(ob, m_oldvalue);
}

void history::dialogues::PhraseChangeLineEditBasedProperty::tryUpdateUI(core::Editor* e, const sad::String& v)
{
	if (e)
	{
		if (e->shared()->selectedDialogue() == m_dialogue)
		{
			if (m_update)
			{
				e->emitClosure(bind(
					e->panel()->UI()->lstPhrases->item(m_position),
					&QListWidgetItem::setText,
					e->panel()->nameForPhrase(*(m_dialogue->phrases()[m_position]))
				));
			}
			if (e->panel()->UI()->lstPhrases->currentRow() == m_position)
			{
				e->emitClosure(blocked_bind(
					m_widget,
					&QLineEdit::setText,
					v.c_str()
				));
			}
		}
	}
}
