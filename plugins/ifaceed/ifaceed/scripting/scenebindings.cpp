#include "scripting.h"
#include "scenebindings.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/scenes/sceneslayerswap.h"

Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::AColor)
Q_DECLARE_METATYPE(sad::Scene*)

QScriptValue scripting::addScene(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());

	if (context->argumentCount() != 1 && context->argumentCount() != 0)
    {
        context->throwError(QScriptContext::SyntaxError, "add() accepts only zero or one parameter");
        return static_cast<uint>(0);
    }

	QString name;
	if (context->argumentCount() == 1)
	{
		name = context->argument(0).toString();
	}

	return static_cast<uint>(e->panel()->addSceneWithName(name, false));
}


QScriptValue scripting::removeScene(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	sad::db::Object* result = NULL;
	if (context->argumentCount() != 1)
    {
        context->throwError(QScriptContext::SyntaxError, "remove() accepts only one argument");
        return main;
    }

	//	Querying object
	if (context->argument(0).isString())
	{
		sad::Vector<sad::db::Object*> os = sad::Renderer::ref()->database("")->queryByName(context->argument(0).toString().toStdString());
		if(os.size())
		{
			result  = os[0];
		}
	}
	else
	{
		if (context->argument(0).isNumber())
		{
			result = sad::Renderer::ref()->database("")->queryByMajorId(context->argument(0).toInt32());
		}
		else
		{
			 context->throwError(QScriptContext::SyntaxError, "remove() : first argument must be string or number");
			 return main;
		}
	}

	if (result)
	{
		if (result->isInstanceOf("sad::Scene"))
		{
			e->panel()->scriptableRemoveScene(static_cast<sad::Scene*>(result), false);
		}
	}
	return main;
}

QScriptValue scripting::sceneMoveBack(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	sad::db::Object* result = NULL;
	if (context->argumentCount() != 1)
    {
        context->throwError(QScriptContext::SyntaxError, "moveBack() accepts only one argument");
        return main;
    }

	//	Querying object
	if (context->argument(0).isString())
	{
		sad::Vector<sad::db::Object*> os = sad::Renderer::ref()->database("")->queryByName(context->argument(0).toString().toStdString());
		if(os.size())
		{
			result  = os[0];
		}
	}
	else
	{
		if (context->argument(0).isNumber())
		{
			result = sad::Renderer::ref()->database("")->queryByMajorId(context->argument(0).toInt32());
		}
		else
		{
			 context->throwError(QScriptContext::SyntaxError, "moveBack() : first argument must be string or number");
			 return main;
		}
	}

	if (result)
	{
		if (result->isInstanceOf("sad::Scene"))
		{
			sad::Scene* scene = static_cast<sad::Scene*>(result);
			MainPanel* panel = e->panel();
			int row = panel->findSceneInList(scene);
			if (row > 0)
			{
				sad::Scene* previousscene = panel->UI()->lstScenes->item(row -1)->data(Qt::UserRole).value<sad::Scene*>();
				history::Command* c = new history::scenes::LayerSwap(scene, previousscene, row, row - 1);
				panel->editor()->currentBatchCommand()->add(c);
				c->commit(panel->editor());
			}
		}
	}
	return main;
}


QScriptValue scripting::sceneMoveFront(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	sad::db::Object* result = NULL;
	if (context->argumentCount() != 1)
    {
        context->throwError(QScriptContext::SyntaxError, "moveFront() accepts only one argument");
        return main;
    }

	//	Querying object
	if (context->argument(0).isString())
	{
		sad::Vector<sad::db::Object*> os = sad::Renderer::ref()->database("")->queryByName(context->argument(0).toString().toStdString());
		if(os.size())
		{
			result  = os[0];
		}
	}
	else
	{
		if (context->argument(0).isNumber())
		{
			result = sad::Renderer::ref()->database("")->queryByMajorId(context->argument(0).toInt32());
		}
		else
		{
			 context->throwError(QScriptContext::SyntaxError, "moveFront() : first argument must be string or number");
			 return main;
		}
	}

	if (result)
	{
		if (result->isInstanceOf("sad::Scene"))
		{
			sad::Scene* scene = static_cast<sad::Scene*>(result);
			MainPanel* panel = e->panel();
			int row = panel->findSceneInList(scene);
			if (row < panel->UI()->lstScenes->count() - 1)
			{
				sad::Scene* nextscene = panel->UI()->lstScenes->item(row + 1)->data(Qt::UserRole).value<sad::Scene*>();

				history::Command* c = new history::scenes::LayerSwap(scene, nextscene, row, row + 1);
				panel->editor()->currentBatchCommand()->add(c);
				c->commit(panel->editor());
			}
		}
	}
	return main;
}

