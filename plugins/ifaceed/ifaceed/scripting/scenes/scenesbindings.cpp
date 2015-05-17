#include "../scripting.h"
#include "../querytable.h"

#include "scenesbindings.h"

#include "../fromvalue.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/scenes/sceneslayerswap.h"

#include <renderer.h>

#include <db/dbdatabase.h>

Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::AColor)
Q_DECLARE_METATYPE(sad::Scene*)

unsigned long long scripting::scenes::add(scripting::Scripting* s, QString name)
{
	return s->panel()->addSceneWithName(name, false);
}

unsigned long long scripting::scenes::addNameless(scripting::Scripting* s)
{
	return s->panel()->addSceneWithName("", false);	
}

void scripting::scenes::remove(scripting::Scripting* s, sad::Scene* scene)
{
	s->panel()->scriptableRemoveScene(scene, false);
}

void scripting::scenes::moveBack(scripting::Scripting* s, sad::Scene* scene)
{
	MainPanel* panel = s->panel();
	int row = panel->findSceneInList(scene);
	if (row > 0)
	{
		sad::Scene* previousscene = panel->UI()->lstScenes->item(row -1)->data(Qt::UserRole).value<sad::Scene*>();
		history::Command* c = new history::scenes::LayerSwap(scene, previousscene, row, row - 1);
		panel->editor()->currentBatchCommand()->add(c);
		c->commit(panel->editor());
	}
}

void scripting::scenes::moveFront(scripting::Scripting* s, sad::Scene* scene)
{
	MainPanel* panel = s->panel();
	int row = panel->findSceneInList(scene);
	if (row < panel->UI()->lstScenes->count() - 1)
	{
		sad::Scene* nextscene = panel->UI()->lstScenes->item(row + 1)->data(Qt::UserRole).value<sad::Scene*>();

		history::Command* c = new history::scenes::LayerSwap(scene, nextscene, row, row + 1);
		panel->editor()->currentBatchCommand()->add(c);
		c->commit(panel->editor());
	}
}

QScriptValue scripting::scenes::list(QScriptContext* ctx, QScriptEngine* engine)
{
    return scripting::query_table("scenes", "sad::Scene", ctx, engine);
}
