#include "../scripting.h"
#include "databasebindings.h"

#include <renderer.h>

#include <db/dbdatabase.h>

#include "../tovalue.h"
#include "../fromvalue.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../gui/table/delegate.h"

bool scripting::database::addProperty(scripting::Scripting* s, sad::String type, sad::String name)
{
	return s->panel()->scriptableAddProperty(type, name, false);	
}

bool scripting::database::removeProperty(scripting::Scripting* s, sad::String name)
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

QScriptValue scripting::database::list(QScriptContext* ctx, QScriptEngine* engine)
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

    return scripting::FromValue<QStringList>::perform(list, engine);
}

sad::String scripting::database::type(scripting::Scripting* s, sad::db::Object* o)
{
	return o->serializableName();
}

QScriptValue scripting::database::readableProperties(QScriptContext* ctx, QScriptEngine* engine)
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

	if (obj.value()->isInstanceOf("sad::SceneNode"))
	{
		list << "layer";
		list << "scene";
		list << "visible";
		list << "area";
		list << "angle";
		list << "color";
	}

	if (obj.value()->isInstanceOf("sad::Label")
		|| obj.value()->isInstanceOf("sad::db::custom::Object"))
	{
		list << "fontsize";
		list << "text";
		list << "linespacing";
		list << "font";
	}

	if (obj.value()->isInstanceOf("sad::Sprite2D")
		|| obj.value()->isInstanceOf("sad::db::custom::Object"))
	{
		list << "flipx";
		list << "flipy";
		list << "options";
	}

	if (obj.value()->isInstanceOf("sad::db::custom::Object"))
	{
		list << "schema";
		sad::db::custom::Object* o = static_cast<sad::db::custom::Object*>(obj.value());
		const sad::Hash<sad::String, sad::db::Property*> & sprops =  o->schemaProperties();
		for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = sprops.const_begin();
			it != sprops.const_end();
			++it)
		{
			list << it.key().c_str();
		}
	}

    return scripting::FromValue<QStringList>::perform(list, engine);
}

QScriptValue scripting::database::writableProperties(QScriptContext* ctx, QScriptEngine* engine)
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


    return scripting::FromValue<QStringList>::perform(list, engine);
}
