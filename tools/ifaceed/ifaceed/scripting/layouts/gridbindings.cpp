#include "gridbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>


#include "../scripting.h"
#include "../querytable.h"
#include "../queryobject.h"
#include "../tovalue.h"

#include "scriptablegrid.h"
#include "scriptablegridcell.h"
#include "scriptablelengthvalue.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

#include "../../gui/rendergrids.h"

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
    grid->setRenderColor(gui::RenderGrids::defaultColor());
    scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
    core::Editor* editor = e->editor();
    editor->actions()->gridActions()->addGridToGridList(grid);
    editor->renderGrids()->add(grid);
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


QScriptValue scripting::layouts::parent(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    if (ctx->argumentCount() != 1)
    {
        ctx->throwError("parent: accepts only 1 argument");
    }

    sad::Maybe<sad::SceneNode*> maybe_node = scripting::query<sad::SceneNode*>(ctx->argument(0));
    scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
    if (maybe_node.exists())
    {
        core::Editor* editor = e->editor();
        gui::actions::GridActions* ga = editor->actions()->gridActions();
        sad::Vector<gui::GridPosition> v = ga->findRelatedGrids(maybe_node.value());
        if (v.size())
        {
            gui::GridPosition g = v[0];
            QScriptValue result = engine->newArray(2);
            QScriptValue source = engine->newQObject(new scripting::layouts::ScriptableGridCell(g.Grid->MajorId, g.Row, g.Col, e));
            result.setProperty(0, source);
            result.setProperty(1, QScriptValue(g.Pos));
            return result;
        }
    }
    else
    {
        ctx->throwError("parent: cannot find object to be searched in grids");
    }
    return engine->nullValue();
}

QScriptValue scripting::layouts::length_value(
    QScriptContext* ctx,
    QScriptEngine* engine	
)
{
    if (ctx->argumentCount() != 2)
    {
        ctx->throwError("LengthValue: accepts only 2 arguments");
    }
    sad::Maybe<sad::layouts::Unit> mu_maybe = scripting::ToValue<sad::layouts::Unit>::perform(ctx->argument(0));
    sad::Maybe<double> mv_maybe = scripting::ToValue<double>::perform(ctx->argument(1));
    scripting::Scripting* e = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject());
    if (mu_maybe.exists() == false)
    {
        ctx->throwError("LengthValue: first argument is not a valid unit. Please, use one of E.layouts.Unit.LU_Auto, E.layouts.Unit.LU_Pixels, E.layouts.Unit.LU_Percents values");
        return engine->nullValue();
    }
    if (mv_maybe.exists() == false)
    {
        ctx->throwError("LengthValue: second argument is not a valid value.");
        return engine->nullValue();
    }
    return engine->newQObject(new scripting::layouts::ScriptableLengthValue(mu_maybe.value(), mv_maybe.value(), e));
}
