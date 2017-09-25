#include "dialoguesbindings.h"
#include "dialoguesphraseref.h"

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../abstractgetter.h"
#include "../abstractsetter.h"


#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/dialogueactions.h"

#include "../../history/dialogues/dialoguesnew.h"
#include "../../history/dialogues/dialoguesphrasenew.h"
#include "../../history/dialogues/dialoguesphraseremove.h"
#include "../../history/dialogues/dialogueschangename.h"


Q_DECLARE_METATYPE(scripting::dialogues::PhraseRef*)
Q_DECLARE_METATYPE(QVector<sad::dialogue::Phrase>)
Q_DECLARE_METATYPE(sad::dialogue::Phrase) //-V566


QVector<unsigned long long> scripting::dialogues::list()
{
    return scripting::query_table("dialogues", "sad::dialogue::Dialogue");
}

unsigned long long scripting::dialogues::_add(
    scripting::Scripting* scripting, 
    sad::String name,
    QVector<sad::dialogue::Phrase> phrases
)
{
    core::Editor* e = scripting->editor();

    sad::dialogue::Dialogue* d = new sad::dialogue::Dialogue();
    d->setObjectName(name);
    for(size_t i = 0; i < phrases.size(); i++)
    {
        d->phrases().add(new sad::dialogue::Phrase(phrases[i]));
    }


    sad::Renderer::ref()->database("")->table("dialogues")->add(d);
    history::dialogues::New* c = new history::dialogues::New(d);
    c->commit(e);
    e->currentBatchCommand()->add(c);

    return d->MajorId;
}


void scripting::dialogues::remove(scripting::Scripting* scripting, sad::dialogue::Dialogue* d)
{
    core::Editor* e = scripting->editor();
    e->actions()->dialogueActions()->removeDialogueFromDatabase(d, false);	
}


void scripting::dialogues::addPhrase(
    scripting::Scripting* scripting, 
    sad::dialogue::Dialogue* d,
    sad::dialogue::Phrase phrase
)
{
    core::Editor* e = scripting->editor();

    history::dialogues::PhraseNew* c = new history::dialogues::PhraseNew(d, phrase);
    c->commit(e);
    e->currentBatchCommand()->add(c);
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

    core::Editor* e = scripting->editor();
    command->commit(e);
    e->currentBatchCommand()->add(command);
    return true;
}

unsigned int scripting::dialogues::length(scripting::Scripting* scripting, sad::dialogue::Dialogue* d)
{
    return d->phrases().count();
}

dukpp03::Maybe<scripting::dialogues::PhraseRef*> scripting::dialogues::phrase(scripting::Scripting* scripting, sad::dialogue::Dialogue* d, unsigned int pos)
{
    if (pos >= d->phrases().count())
    {
        scripting->context()->throwError("phrase: position is out of dialogue");
        throw new dukpp03::ArgumentException();
    }

    return dukpp03::Maybe<scripting::dialogues::PhraseRef*>(new scripting::dialogues::PhraseRef(scripting, d, pos));
}



dukpp03::qt::JSObject* scripting::dialogues::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::registerMetaType<scripting::dialogues::PhraseRef>();

    dukpp03::qt::JSObject* dialogues = new dukpp03::qt::JSObject();
    dialogues->setProperty("list", dukpp03::qt::make_function::from(scripting::dialogues::list)); // E.dialogues.list
    dialogues->setProperty("_add", dukpp03::qt::curried1::from(s, scripting::dialogues::_add)); // E.dialogues._add
    dialogues->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::dialogues::remove)); // E.dialogues.remove
    dialogues->setProperty("addPhrase", dukpp03::qt::curried1::from(s, scripting::dialogues::addPhrase)); // E.dialogues.addPhrase
    dialogues->setProperty("removePhrase", dukpp03::qt::curried1::from(s, scripting::dialogues::removePhrase)); // E.dialogues.removePhrase
    dialogues->setProperty("length", dukpp03::qt::curried1::from(s, scripting::dialogues::length)); // E.dialogues.length
    dialogues->setProperty("phrase", dukpp03::qt::curried1::from(s, scripting::dialogues::phrase)); // E.dialogues.phrase


    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();

    {
        scripting::AbstractSetter<sad::dialogue::Dialogue*, sad::String>* name_setter = scripting::setterForProperty<sad::dialogue::Dialogue*, sad::String>(s, "name");
        std::function<
            void(scripting::Scripting*, sad::dialogue::Dialogue*, const sad::String&, sad::String oldvalue, sad::String newvalue)
        > set_name_action = [](scripting::Scripting* s, sad::dialogue::Dialogue* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor = s->editor();

            history::Command* c = new history::dialogues::ChangeName(obj, oldvalue, newvalue);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        name_setter->addAction(set_name_action);
        set->add(name_setter);
    }

    dialogues->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.dialogues.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>("minorid"));
    }
    dialogues->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.dialogues.get

    e->setProperty("dialogues", dialogues);
    bool b  = s->context()->eval(
        "var phrase = function(actorName, actorPortrait, text, duration, viewHint) {"
        "   return {\"actorName\" : actorName, \"actorPortrait\" : actorPortrait, \"text\": text, \"duration\": duration, \"viewHint\" : viewHint};"
        "};"
        "E.dialogues.add = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"phrases\" in o == false) "
        "   {                              "
        "      o[\"phrases\"] = [];        "
        "   }                              "
        "   return E.dialogues._add(o[\"name\"], o[\"phrases\"]);"
        "};"
        "E.dialogues.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.dialogues.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.dialogues.set(arguments[0], arguments[1], arguments[2]);"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert(b);
    return dialogues;
}
