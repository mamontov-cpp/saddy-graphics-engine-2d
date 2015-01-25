#include "scripting.h"
#include "databasebindings.h"
#include "../mainpanel.h"

#include "../core/editor.h"

#include "../gui/table/delegate.h"

bool scripting::addProperty(scripting::Scripting* s, sad::String type, sad::String name)
{
	return s->panel()->scriptableAddProperty(type, name, false);	
}

bool scripting::removeProperty(scripting::Scripting* s, sad::String name)
{
	MainPanel* panel = s->panel();
	bool result = false;
	if(sad::Renderer::ref()->database("")->propertyByName(name) != NULL)
	{
		if (panel->delegatesByName().contains(name))
		{
			result = true;
			panel->delegatesByName()[name]->removeWithCommand();
		}
	}
	return result;
}

QScriptValue scripting::listToValue(const QStringList& list, QScriptEngine* engine)
{
	QScriptValue v = engine->newArray(list.size());
	for(size_t i = 0; i < list.size(); i++)
	{
		v.setProperty(i, list[i]);
	}

	return v;
}

QScriptValue scripting::listProperties(QScriptContext* ctx, QScriptEngine* engine)
{
	if (ctx->argumentCount() != 0)
	{
		ctx->throwError("list: accepts only 0 arguments");
		return QScriptValue();
	}
	QStringList list;
	sad::db::Database* db = sad::Renderer::ref()->database("");
	sad::db::Database::Properties::const_iterator it = db->begin();
	for(; it != db->end(); ++it)
	{
		if (it.key() != "palette")
		{
			list << it.key().c_str();
		}
	}

	return scripting::listToValue(list, engine);
}

sad::String scripting::objectType(scripting::Scripting* s, sad::db::Object* o)
{
	return o->serializableName();
}

QScriptValue scripting::readableProperties(QScriptContext* ctx, QScriptEngine* engine)
{
	if (ctx->argumentCount() != 1)
	{
		ctx->throwError("readableProperties(): accepts only 1 argument");
		return QScriptValue();
	}

	sad::Maybe<sad::db::Object*> obj = scripting::ToValue<sad::db::Object*>::perform(ctx->argument(0));
	if (obj.exists() == false)
	{
		ctx->throwError("readableProperties(): first argument must be sad::db::Object");
		return QScriptValue();
	}

	QStringList list;
	list << "majorid";
	list << "minorid";
	list << "name";

	if (obj.value()->isInstanceOf("sad::Scene"))
	{
		list << "layer";
	}

	return scripting::listToValue(list, engine);
}

QScriptValue scripting::writableProperties(QScriptContext* ctx, QScriptEngine* engine)
{
	if (ctx->argumentCount() != 1)
	{
		ctx->throwError("writableProperties(): accepts only 1 argument");
		return QScriptValue();
	}

	sad::Maybe<sad::db::Object*> obj = scripting::ToValue<sad::db::Object*>::perform(ctx->argument(0));
	if (obj.exists() == false)
	{
		ctx->throwError("writableProperties(): first argument must be sad::db::Object");
		return QScriptValue();
	}

	QStringList list;
	list << "name";


	return scripting::listToValue(list, engine);
}
