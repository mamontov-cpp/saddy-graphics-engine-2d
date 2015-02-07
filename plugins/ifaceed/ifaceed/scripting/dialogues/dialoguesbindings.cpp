#include "dialoguesbindings.h"
#include "dialoguesphraseref.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/dialogueactions.h"

QScriptValue scripting::dialogues::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("dialogues", "sad::dialogue::Dialogue", ctx, engine);
}