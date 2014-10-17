#include "dialogueactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"

#include "../history/dialogues/dialoguesnew.h"

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

void gui::DialogueActions::dialogueChanged(int i)
{
	if (i >= 0)
    {
        QListWidgetItem* item = m_panel->UI()->lstDialogues->item(i);
        QVariant v = item->data(Qt::UserRole);
        sad::dialogue::Dialogue* w = v.value<sad::dialogue::Dialogue*>();
        m_panel->editor()->shared()->setSelectedDialogue(w);
        m_panel->editor()->emitClosure( blocked_bind(
			m_panel->UI()->txtDialogueName,
			&QLineEdit::setText,
			w->objectName().c_str()
		));
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
}