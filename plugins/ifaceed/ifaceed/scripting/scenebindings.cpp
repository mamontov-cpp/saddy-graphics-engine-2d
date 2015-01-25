#include "scripting.h"
#include "scenebindings.h"
#include "fromvalue.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/scenes/sceneslayerswap.h"

#include <renderer.h>

#include <db/dbdatabase.h>

Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::AColor)
Q_DECLARE_METATYPE(sad::Scene*)

unsigned long long scripting::addScene(scripting::Scripting* s, QString name)
{
	return s->panel()->addSceneWithName(name, false);
}

unsigned long long scripting::addNamelessScene(scripting::Scripting* s)
{
	return s->panel()->addSceneWithName("", false);	
}

void scripting::removeScene(scripting::Scripting* s, sad::Scene* scene)
{
	s->panel()->scriptableRemoveScene(scene, false);
}

void scripting::moveSceneBack(scripting::Scripting* s, sad::Scene* scene)
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

void scripting::moveSceneFront(scripting::Scripting* s, sad::Scene* scene)
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

QScriptValue scripting::listScenes(QScriptContext* ctx, QScriptEngine* engine)
{
	if (ctx->argumentCount() != 0)
	{
		ctx->throwError("list: accepts only 0 arguments");
	}

	sad::Vector<unsigned long long> list;
	sad::db::Database* db = sad::Renderer::ref()->database("");
	sad::Vector<sad::db::Object*> objs;
	db->table("scenes")->objects(objs);
	for(size_t i = 0; i < objs.size(); i++)
	{
		if (objs[i]->Active == false)
		{
			objs.removeAt(i);
			--i;
		}
	}
	QScriptValue v = engine->newArray(objs.size());
	for(size_t i = 0; i < objs.size(); i++)
	{
		v.setProperty(i,scripting::FromValue<unsigned long long>::perform(objs[i]->MajorId, engine));
	}

	return v;
}
