#include "gridbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>


#include "../scripting.h"
#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"

#include "scriptablegrid.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

#include "../../history/layouts/layoutsnew.h"

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


QScriptValue scripting::layouts::add(
    QScriptContext* ctx,
    QScriptEngine* engine	
)
{
    if (ctx->argumentCount() > 1)
    {
        ctx->throwError("add: accepts only 0 or 1 arguments");
    }
    sad::String name = "";
    if (ctx->argumentCount() == 1)
    {
        sad::Maybe<sad::String> name_maybe = scripting::ToValue<sad::String>::perform(ctx->argument(0));
        if (name_maybe.exists())
        {
            name = name_maybe.value();
        }
        else
        {
            ctx->throwError("add: name should be a string");
            return engine->nullValue();
        }
    }
    sad::layouts::Grid* grid = new sad::layouts::Grid();
    grid->Active = true;
    grid->setTreeName(sad::Renderer::ref(), "");
    grid->setFixedWidth(false);
    grid->setFixedHeight(false);
    grid->setObjectName(name);
    grid->setRows(1);
    grid->setColumns(1);
    scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
    core::Editor* editor = e->editor();
    editor->actions()->gridActions()->addGridToGridList(grid);

    sad::Renderer::ref()->database("")->table("layouts")->add(grid);

    editor->currentBatchCommand()->add(new history::layouts::New(grid));

    return engine->newQObject(new scripting::layouts::ScriptableGrid(grid->MajorId, e));
}

QScriptValue scripting::layouts::remove(
    QScriptContext* ctx,
    QScriptEngine* engine	
)
{
    if (ctx->argumentCount() != 1)
    {
        ctx->throwError("remove: accepts only 1 argument");
    }

    sad::Maybe<sad::layouts::Grid*> maybe_grid = scripting::query<sad::layouts::Grid*>(ctx->argument(0)); 
    scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
    if (maybe_grid.exists())
    {
        core::Editor* editor = e->editor();
        editor->actions()->gridActions()->scriptableRemoveGrid(maybe_grid.value(), false);
    }
    else
    {
        ctx->throwError("remove: cannot find grid to be removed");
    }
    return engine->nullValue();
}