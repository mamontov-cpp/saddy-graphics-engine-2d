#include "../scripting.h"
#include "../querytable.h"

#include "scenesbindings.h"

#include "../fromvalue.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/sceneactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uisceneblock.h"

#include "../../history/scenes/sceneslayerswap.h"

#include <renderer.h>

#include <db/dbdatabase.h>

Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::AColor)
Q_DECLARE_METATYPE(sad::Scene*) //-V566

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

QScriptValue scripting::scenes::list(QScriptContext* ctx, QScriptEngine* engine)
{
    return scripting::query_table("scenes", "sad::Scene", ctx, engine);
}
