#include "dialoguesbindings.h"
#include "dialoguesphraseref.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/dialogueactions.h"

#include "../../history/dialogues/dialoguesnew.h"
#include "../../history/dialogues/dialoguesphrasenew.h"
#include "../../history/dialogues/dialoguesphraseremove.h"

QScriptValue scripting::dialogues::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("dialogues", "sad::dialogue::Dialogue", ctx, engine);
}

unsigned long long scripting::dialogues::_add(
	scripting::Scripting* scripting, 
	sad::String name,
	sad::Vector<sad::dialogue::Phrase> phrases
)
{
	MainPanel* panel = scripting->panel();

	sad::dialogue::Dialogue* d = new sad::dialogue::Dialogue();
    d->setObjectName(name);
	for(size_t i = 0; i < phrases.size(); i++)
	{
		d->phrases().add(new sad::dialogue::Phrase(phrases[i]));
	}


	sad::Renderer::ref()->database("")->table("dialogues")->add(d);
    history::dialogues::New* c = new history::dialogues::New(d);
    c->commit(panel->editor());
    panel->editor()->currentBatchCommand()->add(c);

	return d->MajorId;
}


void scripting::dialogues::remove(scripting::Scripting* scripting, sad::dialogue::Dialogue* d)
{
	MainPanel* panel = scripting->panel();
	panel->dialogueActions()->removeDialogueFromDatabase(d, false);	
}


void scripting::dialogues::addPhrase(
	scripting::Scripting* scripting, 
	sad::dialogue::Dialogue* d,
	sad::dialogue::Phrase phrase
)
{
	MainPanel* panel = scripting->panel();

	history::dialogues::PhraseNew* c = new history::dialogues::PhraseNew(d, phrase);
    c->commit(panel->editor());
    panel->editor()->currentBatchCommand()->add(c);
}

bool scripting::dialogues::removePhrase(
	scripting::Scripting* scripting, 
	sad::dialogue::Dialogue* d,
	unsigned int pos
)
{
	if (pos >= d->phrases().count())
	{
		return false;
	}

	history::dialogues::PhraseRemove* command = new history::dialogues::PhraseRemove(d, pos);

	core::Editor* e = scripting->panel()->editor();
	command->commit(e);
	e->currentBatchCommand()->add(command);
	return true;
}

unsigned int scripting::dialogues::length(scripting::Scripting* scripting, sad::dialogue::Dialogue* d)
{
	return d->phrases().count();
}

QScriptValue scripting::dialogues::phrase(scripting::Scripting* scripting, sad::dialogue::Dialogue* d, unsigned int pos)
{
	if (pos >= d->phrases().count())
	{
		scripting->engine()->currentContext()->throwError(QScriptContext::SyntaxError, "phrase: position is out of dialogue");
		return scripting->engine()->undefinedValue();
	}

	return scripting->engine()->newQObject(new scripting::dialogues::PhraseRef(d, pos));
}
