#include "gridbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

QScriptValue scripting::layouts::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    return scripting::query_table("layouts", "sad::layouts::Grid", ctx, engine);
}
