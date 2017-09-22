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

QVector<unsigned long long>scripting::layouts::list()
{
    return scripting::query_table("layouts", "sad::layouts::Grid");
}

scripting::layouts::ScriptableGrid*  scripting::layouts::_query(
    scripting::Scripting* s,
    sad::layouts::Grid* grid
)
{
    return new scripting::layouts::ScriptableGrid(grid->MajorId, s);
}


scripting::layouts::ScriptableGrid*  scripting::layouts::add(
    scripting::Scripting* s,
    const sad::String& name
)
{
    sad::layouts::Grid* grid = new sad::layouts::Grid();
    grid->Active = true;
    grid->setTreeName(sad::Renderer::ref(), "");
    grid->setFixedWidth(false);
    grid->setFixedHeight(false);
    grid->setObjectName(name);
    grid->setRows(1);
    grid->setColumns(1);
    grid->setRenderColor(gui::RenderGrids::defaultColor());
    core::Editor* editor = s->editor();
    editor->actions()->gridActions()->addGridToGridList(grid);
    editor->renderGrids()->add(grid);
    sad::Renderer::ref()->database("")->table("layouts")->add(grid);

    editor->currentBatchCommand()->add(new history::layouts::New(grid));

    return new scripting::layouts::ScriptableGrid(grid->MajorId, s);
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
            result.setProperty(1, QScriptValue(static_cast<unsigned int>(g.Pos)));
            return result;
        }
    }
    else
    {
        ctx->throwError("parent: cannot find object to be searched in grids");
    }
    return engine->nullValue();
}

