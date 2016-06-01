#include "gridbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>


#include "../scripting.h"
#include "../querytable.h"
#include "../queryobject.h"
#include "scriptablegrid.h"

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

QScriptValue scripting::layouts::query(
    QScriptContext* ctx,
    QScriptEngine* engine	
)
{
	if (ctx->argumentCount() != 1)
    {
        ctx->throwError("list: accepts only 1 argument");
    }

	sad::Maybe<sad::layouts::Grid*> maybe_grid = scripting::query<sad::layouts::Grid*>(ctx->argument(0)); 
	scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
	if (maybe_grid.exists())
	{
		return engine->newQObject(new scripting::layouts::ScriptableGrid(maybe_grid.value()->MajorId, e));
	}
	return engine->nullValue();
}