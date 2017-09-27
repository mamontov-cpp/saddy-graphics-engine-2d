#include "gridbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>


#include "../scripting.h"
#include "../querytable.h"
#include "../queryobject.h"

#include "scriptablegrid.h"
#include "scriptablegridcell.h"
#include "scriptablelengthvalue.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

#include "../../gui/rendergrids.h"

#include "../../history/layouts/layoutsnew.h"

#include "../abstractgetter.h"
#include "../abstractsetter.h"

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGrid*)
Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell*)
Q_DECLARE_METATYPE(sad::Vector<unsigned long long>) //-V566
Q_DUKPP03_DECLARE_METATYPE(scripting::layouts::ScriptableLengthValue)  //-V566

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

dukpp03::qt::JSObject* scripting::layouts::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableLengthValue>();
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableGrid>();
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableGridCell>();

    dukpp03::qt::JSObject* layouts = new dukpp03::qt::JSObject();

    e->setProperty("layouts", layouts);

    dukpp03::qt::JSObject* unit = new dukpp03::qt::JSObject();
    unit->setProperty("LU_Auto", static_cast<unsigned int>(sad::layouts::LU_Auto));
    unit->setProperty("LU_Pixels", static_cast<unsigned int>(sad::layouts::LU_Pixels));
    unit->setProperty("LU_Percents", static_cast<unsigned int>(sad::layouts::LU_Percents));
    layouts->setProperty("Unit", unit); // E.layouts.Unit
    layouts->setEvaluatedProperty("LU_Auto", "E.layouts.Unit.LU_Auto");
    layouts->setEvaluatedProperty("LU_Pixels", "E.layouts.Unit.LU_Pixels");
    layouts->setEvaluatedProperty("LU_Percents", "E.layouts.Unit.LU_Percents");

    dukpp03::qt::JSObject* horizontal_alignment = new dukpp03::qt::JSObject();
    horizontal_alignment->setProperty("LHA_Left", static_cast<unsigned int>(sad::layouts::LHA_Left));
    horizontal_alignment->setProperty("LHA_Middle", static_cast<unsigned int>(sad::layouts::LHA_Middle));
    horizontal_alignment->setProperty("LHA_Right", static_cast<unsigned int>(sad::layouts::LHA_Right));
    layouts->setProperty("HorizontalAlignment", horizontal_alignment); // E.layouts.HorizontalAlignment
    layouts->setEvaluatedProperty("LHA_Left", "E.layouts.HorizontalAlignment.LHA_Left");
    layouts->setEvaluatedProperty("LHA_Middle", "E.layouts.HorizontalAlignment.LHA_Middle");
    layouts->setEvaluatedProperty("LHA_Right", "E.layouts.HorizontalAlignment.LHA_Right");

    dukpp03::qt::JSObject* vertical_alignment = new dukpp03::qt::JSObject();
    vertical_alignment->setProperty("LVA_Top", static_cast<unsigned int>(sad::layouts::LVA_Top));
    vertical_alignment->setProperty("LVA_Middle", static_cast<unsigned int>(sad::layouts::LVA_Middle));
    vertical_alignment->setProperty("LVA_Bottom", static_cast<unsigned int>(sad::layouts::LVA_Bottom));
    layouts->setProperty("VerticalAlignment", vertical_alignment); // E.layouts.VerticalAlignment
    layouts->setEvaluatedProperty("LVA_Top", "E.layouts.VerticalAlignment.LVA_Top");
    layouts->setEvaluatedProperty("LVA_Middle", "E.layouts.VerticalAlignment.LVA_Middle");
    layouts->setEvaluatedProperty("LVA_Bottom", "E.layouts.VerticalAlignment.LVA_Bottom");

    dukpp03::qt::JSObject* stacking_type = new dukpp03::qt::JSObject();
    stacking_type->setProperty("LST_Horizontal", static_cast<unsigned int>(sad::layouts::LST_Horizontal));
    stacking_type->setProperty("LST_Vertical", static_cast<unsigned int>(sad::layouts::LST_Vertical));
    stacking_type->setProperty("LST_NoStacking", static_cast<unsigned int>(sad::layouts::LST_NoStacking));
    layouts->setProperty("StackingType", stacking_type); // E.layouts.StackingType
    layouts->setEvaluatedProperty("LST_Horizontal", "E.layouts.StackingType.LST_Horizontal");
    layouts->setEvaluatedProperty("LST_Vertical", "E.layouts.StackingType.LST_Vertical");
    layouts->setEvaluatedProperty("LST_NoStacking", "E.layouts.StackingType.LST_NoStacking");

    scripting::Scripting* scriptable_me = s;
    std::function<scripting::layouts::ScriptableLengthValue*(unsigned int, double)> length_value = [scriptable_me](unsigned int unit, double value)
    {
        if ((unit != sad::layouts::LU_Auto) && (unit != sad::layouts::LU_Percents) && (unit != sad::layouts::LU_Pixels))
        {
            scriptable_me->context()->throwError("Argument 1 must have sad::layouts::Unit type");
            throw new dukpp03::ArgumentException();
        }
        return new scripting::layouts::ScriptableLengthValue(static_cast<sad::layouts::Unit>(unit), value, scriptable_me);
    };
    layouts->setProperty("LengthValue", static_cast<dukpp03::qt::Callable*>(dukpp03::qt::make_lambda::from(length_value)));

    bool b = s->context()->eval(
        "E.layouts.Auto = function() { return E.layouts.LengthValue(E.layouts.Unit.LU_Auto, 0); };"
        "E.layouts.Pixels = function(a) { if (typeof a != \"number\") throw \"E.layouts.Pixels: first argument should be numeric\"; return E.layouts.LengthValue(E.layouts.Unit.LU_Pixels, a); };"
        "E.layouts.Percents = function(a) { if (typeof a != \"number\") throw \"E.layouts.Percents: first argument should be numeric\"; return E.layouts.LengthValue(E.layouts.Unit.LU_Percents, a); };"
    );
    assert(b);

    layouts->setProperty("list", dukpp03::qt::make_function::from(scripting::layouts::list)); // E.layouts.list
    layouts->setProperty("_query", dukpp03::qt::curried1::from(s, scripting::layouts::_query)); // E.layouts._query

    dukpp03::qt::MultiMethod* add = new dukpp03::qt::MultiMethod();
    {
        add->add(dukpp03::qt::curried1::from(s, scripting::layouts::add));
        scripting::Scripting* local_me = s;
        std::function<scripting::layouts::ScriptableGrid*()> add_no_args = [local_me]() {
            return scripting::layouts::add(local_me, "");
        };
        add->add(dukpp03::qt::make_lambda::from(add_no_args));
    }
    layouts->setProperty("add", static_cast<dukpp03::qt::Callable*>(add)); // E.scenenodes.add
    layouts->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::layouts::remove)); // E.layouts.remove
    layouts->setProperty("parent", dukpp03::qt::curried1::from(s, scripting::layouts::parent)); // E.layouts.parent

    {
        dukpp03::qt::ClassBinding* binding = new dukpp03::qt::ClassBinding();
        binding->addMethod("setArea", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::setArea));
        binding->addMethod("findChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::findChild));
        binding->addMethod("cell", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::cell));
        binding->addMethod("children", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::children));
        binding->registerMetaObject<scripting::layouts::ScriptableGrid>();
        s->context()->addClassBinding("scripting::layouts::ScriptableGrid", binding);
    }

    {
        dukpp03::qt::ClassBinding* binding = new dukpp03::qt::ClassBinding();
        binding->addMethod("children", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::children));
        binding->addMethod("findChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::findChild));
        binding->addMethod("addChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::addChild));
        binding->registerMetaObject<scripting::layouts::ScriptableGridCell>();
        s->context()->addClassBinding("scripting::layouts::ScriptableGridCell", binding);
    }

    b = s->context()->eval(
        "E.layouts.query = function(a) {  try { return E.layouts._query(a); } catch(e) { return null; } };"
    );
    assert(b);

    return layouts;
}