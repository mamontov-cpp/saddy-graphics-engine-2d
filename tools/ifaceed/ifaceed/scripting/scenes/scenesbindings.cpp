#include "../scripting.h"
#include "../querytable.h"

#include "scenesbindings.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/sceneactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uisceneblock.h"

#include "../../history/scenes/sceneslayerswap.h"
#include "../../history/scenes/sceneschangename.h"

#include "../abstractgetter.h"
#include "../abstractsetter.h"

#include <renderer.h>

#include <db/dbdatabase.h>


unsigned long long scripting::scenes::add(scripting::Scripting* s, QString name)
{
    return  s->editor()->actions()->sceneActions()->addSceneWithName(name, false);
}

unsigned long long scripting::scenes::addNameless(scripting::Scripting* s)
{
    return  s->editor()->actions()->sceneActions()->addSceneWithName("", false);	
}

void scripting::scenes::remove(scripting::Scripting* s, sad::Scene* scene)
{
    s->editor()->actions()->sceneActions()->scriptableRemoveScene(scene, false);
}

void scripting::scenes::moveBack(scripting::Scripting* s, sad::Scene* scene)
{
    core::Editor* e = s->editor();	
    int row = s->editor()->actions()->sceneActions()->findSceneInList(scene);
    QListWidget* lst = e->uiBlocks()->uiSceneBlock()->lstScenes;	
    if (row > 0)
    {
        sad::Scene* previousscene = lst->item(row -1)->data(Qt::UserRole).value<sad::Scene*>();
        history::Command* c = new history::scenes::LayerSwap(scene, previousscene, row, row - 1);
        e->currentBatchCommand()->add(c);
        c->commit(e);
    }
}

void scripting::scenes::moveFront(scripting::Scripting* s, sad::Scene* scene)
{
    core::Editor* e = s->editor();
    int row = e->actions()->sceneActions()->findSceneInList(scene);
    QListWidget* lst = e->uiBlocks()->uiSceneBlock()->lstScenes;
    if (row < lst->count() - 1)
    {
        sad::Scene* nextscene = lst->item(row + 1)->data(Qt::UserRole).value<sad::Scene*>();

        history::Command* c = new history::scenes::LayerSwap(scene, nextscene, row, row + 1);
        e->currentBatchCommand()->add(c);
        c->commit(e);
    }
}

QVector<unsigned long long> scripting::scenes::list()
{
    return scripting::query_table("scenes", "sad::Scene");
}


dukpp03::qt::JSObject* scripting::scenes::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::JSObject* scenes = new dukpp03::qt::JSObject();
    scenes->setProperty("list", dukpp03::qt::make_function::from(scripting::scenes::list));

    dukpp03::qt::MultiMethod* add = new dukpp03::qt::MultiMethod();
    add->add(dukpp03::qt::curried1::from(s, scripting::scenes::add));
    add->add(dukpp03::qt::curried1::from(s, scripting::scenes::addNameless));
    scenes->setProperty("add", static_cast<dukpp03::qt::Callable*>(add)); // E.db.add

    scenes->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::scenes::remove));
    scenes->setProperty("moveBack", dukpp03::qt::curried1::from(s, scripting::scenes::moveBack));
    scenes->setProperty("moveFront", dukpp03::qt::curried1::from(s, scripting::scenes::moveFront));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();

    {
        scripting::AbstractSetter<sad::Scene*, sad::String>* name_setter = scripting::setterForProperty<sad::Scene*, sad::String>(s, "name");
        std::function<
            void(scripting::Scripting*, sad::Scene*, const sad::String&, sad::String oldvalue, sad::String newvalue)
        > set_name_action = [](scripting::Scripting* s, sad::Scene* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor = s->editor();

            history::Command* c = new history::scenes::ChangeName(obj, oldvalue, newvalue);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        name_setter->addAction(set_name_action);
        set->add(name_setter);
    }

    scenes->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.scenes.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    get->add(new scripting::AbstractGetter<sad::Scene*, sad::String>("name"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned int>("layer"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>("majorid"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>("minorid"));
    scenes->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.scenes.get

    e->setProperty("scenes", scenes);


    bool b = s->context()->eval(
        "E.scenes.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.scenes.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       E.scenes.set(arguments[0], arguments[1], arguments[2]); return E.scenes;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert(b);
    return scenes;
}