#include "dialogueactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../history/dialogues/dialoguesnew.h"
#include "../history/dialogues/dialoguesremove.h"
#include "../history/dialogues/dialogueschangename.h"
#include "../history/dialogues/dialoguesphrasenew.h"
#include "../history/dialogues/dialoguesphraseremove.h"
#include "../history/dialogues/dialoguesphraseswap.h"
#include "../history/dialogues/dialoguesphrasechangeduration.h"
#include "../history/dialogues/dialoguesphrasechangephrase.h"
#include "../history/dialogues/dialoguesphrasechangelineeditbasedproperty.h"

#include <geometry2d.h>

Q_DECLARE_METATYPE(sad::dialogue::Dialogue*)

// ========================== PUBLIC METHODS ==========================

gui::DialogueActions::DialogueActions(QObject* parent) : QObject(parent)
{
	
}

gui::DialogueActions::~DialogueActions()
{
	
}

void gui::DialogueActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::DialogueActions::panel() const
{
	return m_panel;
}

void gui::DialogueActions::viewDialogue(sad::dialogue::Dialogue* d)
{
	Ui::MainPanelClass* ui = m_panel->UI();
	invoke_blocked(ui->txtDialogueName, &QLineEdit::setText, d->objectName().c_str());
	ui->lstPhrases->clear();
	for(size_t i = 0; i < d->phrases().size(); i++)
	{
		ui->lstPhrases->addItem(m_panel->nameForPhrase(*(d->phrases()[i])));
	}
}

void gui::DialogueActions::viewPhrase(sad::dialogue::Phrase* p)
{
	m_panel->editor()->emitClosure( blocked_bind(
			m_panel->UI()->txtPhraseActorName,
			&QLineEdit::setText,
			p->actorName().c_str()
	));
	m_panel->editor()->emitClosure( blocked_bind(
		m_panel->UI()->txtPhraseActorPortrait,
		&QLineEdit::setText,
		p->actorPortrait().c_str()
	));
	m_panel->editor()->emitClosure( blocked_bind(
		m_panel->UI()->txtPhrasePhrase,
		&QPlainTextEdit::setPlainText,
		p->phrase().c_str()
	));
	m_panel->editor()->emitClosure( blocked_bind(
		m_panel->UI()->dsbPhraseDuration,
		&QDoubleSpinBox::setValue,
		p->duration()
	));
	m_panel->editor()->emitClosure( blocked_bind(
		m_panel->UI()->txtPhraseViewHint,
		&QLineEdit::setText,
		p->viewHint().c_str()
	));
}

void gui::DialogueActions::changePhraseText(
	sad::dialogue::Dialogue* d, 
	int pos, 
	const sad::String& newvalue, 
	bool fromeditor
)
{
	sad::String oldvalue = d->phrases()[pos]->phrase();
	if (oldvalue != newvalue)
	{
		d->phrases()[pos]->setPhrase(newvalue);
		if (m_panel->editor()->shared()->selectedDialogue() == d)
		{
			m_panel->UI()->lstPhrases->item(pos)->setText(m_panel->nameForPhrase(*(d->phrases()[pos])));
		}
		history::dialogues::PhraseChangePhrase* c = new history::dialogues::PhraseChangePhrase(d, pos, oldvalue, newvalue);
		if (fromeditor)
		{
			m_panel->editor()->history()->add(c);
		} 
		else
		{
			m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
}

void gui::DialogueActions::changePhraseDuration(
	sad::dialogue::Dialogue* d, 
	int pos, 
	double newvalue, 
	bool fromeditor
)
{
	double oldvalue = d->phrases()[pos]->duration();
	if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
	{
		history::dialogues::PhraseChangeDuration* c = new history::dialogues::PhraseChangeDuration(
			d, 
			pos, 
			oldvalue, 
			newvalue
		);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			m_panel->editor()->history()->add(c);
		} 
		else
		{
			m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
}

void gui::DialogueActions::changePhraseActorName(
	sad::dialogue::Dialogue* d, 
	int pos, 
	const sad::String& newvalue, 
	bool fromeditor
)
{
	sad::String oldvalue = d->phrases()[pos]->actorName();
	if (oldvalue != newvalue)
	{
		history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
			m_panel->UI()->txtPhraseActorName,
			&sad::dialogue::Phrase::setActorName,
			true,
			d, 
			pos, 
			oldvalue, 
			newvalue
		);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			m_panel->editor()->history()->add(c);
		}
		else
		{
			m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
}

void gui::DialogueActions::changePhraseActorPortrait(
	sad::dialogue::Dialogue* d, 
	int pos, 
	const sad::String& newvalue, 
	bool fromeditor
)
{
	sad::String oldvalue = d->phrases()[pos]->actorPortrait();
	if (oldvalue != newvalue)
	{
		history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
			m_panel->UI()->txtPhraseActorPortrait,
			&sad::dialogue::Phrase::setActorPortrait,
			false,
			d, 
			pos, 
			oldvalue, 
			newvalue
		);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			m_panel->editor()->history()->add(c);
		}
		else
		{
			m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
}

void gui::DialogueActions::changePhraseViewHint(
	sad::dialogue::Dialogue* d, 
	int pos, 
	const sad::String& newvalue, 
	bool fromeditor
)
{
	sad::String oldvalue = d->phrases()[pos]->viewHint();
	if (oldvalue != newvalue)
	{
		history::Command* c = new history::dialogues::PhraseChangeLineEditBasedProperty(
			m_panel->UI()->txtPhraseViewHint,
			&sad::dialogue::Phrase::setViewHint,
			false,
			d, 
			pos, 
			oldvalue, 
			newvalue
		);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			m_panel->editor()->history()->add(c);
		}
		else
		{
			m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
}

// ========================== PUBLIC SLOTS ==========================

void gui::DialogueActions::addDialogue()
{
    sad::dialogue::Dialogue* w = new sad::dialogue::Dialogue();
    w->setObjectName(m_panel->UI()->txtDialogueName->text().toStdString());
    sad::Renderer::ref()->database("")->table("dialogues")->add(w);
    history::dialogues::New* c = new history::dialogues::New(w);
    c->commit(m_panel->editor());
    m_panel->editor()->history()->add(c);
    m_panel->editor()->shared()->setSelectedDialogue(w);
    m_panel->UI()->lstDialogues->setCurrentRow(m_panel->UI()->lstDialogues->count() - 1);
}


void gui::DialogueActions::removeDialogue()
{
    int row = m_panel->UI()->lstDialogues->currentRow();
    if (row > -1)
    {
        QVariant variant = m_panel->UI()->lstDialogues->item(row)->data(Qt::UserRole);
        sad::dialogue::Dialogue* w = variant.value<sad::dialogue::Dialogue*>();
        history::dialogues::Remove* c = new history::dialogues::Remove(w, row);
        c->commit(m_panel->editor());
        m_panel->editor()->history()->add(c);
    }
}

void gui::DialogueActions::addPhrase()
{
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (d)
	{
		sad::dialogue::Phrase p;
		p.setActorName(m_panel->UI()->txtPhraseActorName->text().toStdString());
		p.setActorPortrait(m_panel->UI()->txtPhraseActorPortrait->text().toStdString());
		p.setPhrase(m_panel->UI()->txtPhrasePhrase->toPlainText().toStdString());
		p.setDuration(m_panel->UI()->dsbPhraseDuration->value());
		p.setViewHint(m_panel->UI()->txtPhraseViewHint->text().toStdString());
		history::dialogues::PhraseNew* c = new history::dialogues::PhraseNew(d, p);
        c->commit(m_panel->editor());
        m_panel->editor()->history()->add(c);
	}
}

void gui::DialogueActions::removePhrase()
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		history::dialogues::PhraseRemove* c = new history::dialogues::PhraseRemove(d, row);
		c->commit(m_panel->editor());
		m_panel->editor()->history()->add(c);
	}
}

void gui::DialogueActions::movePhraseBack()
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row > 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		history::dialogues::PhraseSwap* c = new history::dialogues::PhraseSwap(d, row, row - 1);
		c->commit(m_panel->editor());
		m_panel->editor()->history()->add(c);
	}
}

void gui::DialogueActions::movePhraseFront()
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() - 1 && d)
	{
		history::dialogues::PhraseSwap* c = new history::dialogues::PhraseSwap(d, row, row + 1);
		c->commit(m_panel->editor());
		m_panel->editor()->history()->add(c);
	}
}

void gui::DialogueActions::nameEdited(const QString& name)
{
	sad::String newvalue = name.toStdString();
	sad::dialogue::Dialogue* w = m_panel->editor()->shared()->selectedDialogue();
    if (w)
    {
        sad::String oldvalue =  w->objectName();
        if (newvalue != oldvalue)
        {
            w->setObjectName(newvalue);
            m_panel->updateDialogueName(w);
            m_panel->editor()->history()->add(new history::dialogues::ChangeName(w, oldvalue, newvalue));
        }
    }
}

void gui::DialogueActions::dialogueChanged(int i)
{
	if (i >= 0)
    {
        QListWidgetItem* item = m_panel->UI()->lstDialogues->item(i);
        QVariant v = item->data(Qt::UserRole);
        sad::dialogue::Dialogue* w = v.value<sad::dialogue::Dialogue*>();
        m_panel->editor()->shared()->setSelectedDialogue(w);
		this->viewDialogue(w);
    }
    else
    {
        m_panel->editor()->shared()->setSelectedDialogue(NULL);
    }
}


void gui::DialogueActions::phraseChanged(int i)
{
	sad::dialogue::Dialogue* w =  m_panel->editor()->shared()->selectedDialogue();
	if (i >= 0)
    {
		sad::dialogue::Phrase* p = w->phrases()[i];
		this->viewPhrase(p);
	}
}

void gui::DialogueActions::phraseTextChanged()
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		sad::String newvalue = m_panel->UI()->txtPhrasePhrase->toPlainText().toStdString();
		changePhraseText(d, row, newvalue, true);
	}
}

void gui::DialogueActions::durationChanged(double newvalue)
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		changePhraseDuration(d, row, newvalue, true);
	}
}

void gui::DialogueActions::actorNameChanged(const QString& newvalue)
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		sad::String nv = newvalue.toStdString();
		changePhraseActorName(d, row, nv, true);
	}
}

void gui::DialogueActions::actorPortraitChanged(const QString& newvalue)
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{
		sad::String nv = newvalue.toStdString();
		changePhraseActorPortrait(d, row, nv, true);
	}
}

void gui::DialogueActions::viewHintChanged(const QString& newvalue)
{
	int row = m_panel->UI()->lstPhrases->currentRow();
	sad::dialogue::Dialogue* d = m_panel->editor()->shared()->selectedDialogue();
	if (row >= 0 && row < m_panel->UI()->lstPhrases->count() && d)
	{		
		sad::String nv = newvalue.toStdString();
		changePhraseViewHint(d, row, nv, true);
	}
}
