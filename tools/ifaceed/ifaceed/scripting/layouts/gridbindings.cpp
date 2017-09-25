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

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGrid*)
Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell*)


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

void scripting::layouts::remove(
    scripting::Scripting* s,
    sad::layouts::Grid* grid
)
{
    core::Editor* editor = s->editor();
    editor->actions()->gridActions()->scriptableRemoveGrid(grid, false);
}


dukpp03::Maybe<QVector<QVariant> > scripting::layouts::parent(
    scripting::Scripting* scripting,
    sad::SceneNode* node
)
{
    dukpp03::Maybe<QVector<QVariant> > result;
    core::Editor* editor = scripting->editor();
    gui::actions::GridActions* ga = editor->actions()->gridActions();
    sad::Vector<gui::GridPosition> v = ga->findRelatedGrids(node);
    if (v.size())
    {
        gui::GridPosition g = v[0];

        QVector<QVariant> result_vector;
        QVariant first_result;
        first_result.setValue(new scripting::layouts::ScriptableGridCell(g.Grid->MajorId, g.Row, g.Col, scripting));

        QVariant second_result;
        second_result.setValue(static_cast<unsigned int>(g.Pos));

        result_vector << first_result << second_result;
        result.setValue(result_vector);
    }
    return result;
}

