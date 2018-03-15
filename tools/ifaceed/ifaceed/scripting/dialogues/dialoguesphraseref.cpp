#include "dialoguesphraseref.h"

#include "../scripting.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../history/dialogues/dialoguesphraseswap.h"


scripting::dialogues::PhraseRef::PhraseRef() : m_scripting(NULL), m_d(NULL), m_pos(0)
{
    
}

scripting::dialogues::PhraseRef::PhraseRef(scripting::Scripting* scripting, sad::dialogue::Dialogue* d, unsigned int pos)
: m_scripting(scripting), m_d(d), m_pos(pos)
{
    
}

scripting::dialogues::PhraseRef::~PhraseRef()
{
    
}

bool scripting::dialogues::PhraseRef::valid() const
{
    if (!m_d)
    {
        m_scripting->context()->throwError("Invalid dialogue for phrase reference");
        throw new dukpp03::ArgumentException();
    }
    if (!m_d->Active)
    {
        m_scripting->context()->throwError("Invalid dialogue for phrase reference");
        throw new dukpp03::ArgumentException();
    }
    if (m_pos >= m_d->phrases().count())
    {
        m_scripting->context()->throwError("Invalid position of phrase");
        throw new dukpp03::ArgumentException();
    }

    return true;
}

static sad::dialogue::Phrase default_phrase;

const sad::dialogue::Phrase& scripting::dialogues::PhraseRef::toPhrase() const
{
    if (!valid())
    {
        return default_phrase;
    }
    return *(m_d->phrases()[m_pos]);
}

gui::actions::DialogueActions* scripting::dialogues::PhraseRef::actions() const
{
    return m_scripting->editor()->actions()->dialogueActions();
}

void scripting::dialogues::PhraseRef::setActorName(QString name)
{
    if (!valid())
    {
        return;
    }
    this->actions()->changePhraseActorName(m_d, m_pos, Q2STDSTRING(name), false);
}

void scripting::dialogues::PhraseRef::setActorPortrait(QString portrait)
{
    if (!valid())
    {
        return;
    }
    this->actions()->changePhraseActorPortrait(m_d, m_pos, Q2STDSTRING(portrait), false);
}

void scripting::dialogues::PhraseRef::setText(QString phrase)
{
    if (!valid())
    {
        return;
    }
    this->actions()->changePhraseText(m_d, m_pos, Q2STDSTRING(phrase), false);
}

void scripting::dialogues::PhraseRef::setDuration(double duration)
{
    if (!valid())
    {
        return;
    }
    this->actions()->changePhraseDuration(m_d, m_pos, duration, false);
}

void scripting::dialogues::PhraseRef::setViewHint(QString viewhint)
{
    if (!valid())
    {
        return;
    }
    this->actions()->changePhraseViewHint(m_d, m_pos, Q2STDSTRING(viewhint), false);
}

QString scripting::dialogues::PhraseRef::actorName() const
{
    if (!valid())
    {
        return "";
    }
    return STD2QSTRING(toPhrase().actorName());
}

QString scripting::dialogues::PhraseRef::actorPortrait() const
{
    if (!valid())
    {
        return "";
    }
    return STD2QSTRING(toPhrase().actorPortrait());
}

QString scripting::dialogues::PhraseRef::text() const
{
    if (!valid())
    {
        return "";
    }
    return STD2QSTRING(toPhrase().phrase());
}

double scripting::dialogues::PhraseRef::duration() const
{
    if (!valid())
    {
        return 0;
    }
    return toPhrase().duration();
}

QString scripting::dialogues::PhraseRef::viewHint() const
{
    if (!valid())
    {
        return "";
    }
    return STD2QSTRING(toPhrase().viewHint());
}

unsigned int scripting::dialogues::PhraseRef::position() const
{
    return m_pos;
}

QString  scripting::dialogues::PhraseRef::toString() const
{
    if (!valid())
    {
        return "PhraseRef(<invalid>)";
    }
    QString result = QString("PhraseRef(dialogue : %1, pos: %2, actorName : %3, actorPortrait : %4, text: %5, duration: %6, viewHint: %7)")
        .arg(m_d->MajorId)
        .arg(m_pos)
        .arg(this->actorName())
        .arg(this->actorPortrait())
        .arg(this->text())
        .arg(this->duration())
        .arg(this->viewHint());
    return result;
}

void scripting::dialogues::PhraseRef::moveBack()
{
    if (!valid())
    {
        return;
    }
    if (m_pos > 0)
    {
        core::Editor* editor = m_scripting->editor();

        history::Command* c = new history::dialogues::PhraseSwap(m_d, m_pos - 1, m_pos);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
        
        m_pos--;
    }
}

void scripting::dialogues::PhraseRef::moveFront()
{
    if (!valid())
    {
        return;
    }
    if (m_pos < m_d->phrases().count() - 1)
    {
        core::Editor* editor = m_scripting->editor();

        history::Command* c = new history::dialogues::PhraseSwap(m_d, m_pos, m_pos + 1);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
        
        m_pos++;
    }
}
