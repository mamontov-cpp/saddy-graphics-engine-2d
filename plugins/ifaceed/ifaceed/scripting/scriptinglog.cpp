#include "scriptinglog.h"

#include "scripting.h"

#include "../mainpanel.h"

#include "../core/editor.h"

QScriptValue scripting::scriptinglog(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	QTextEdit* edit = e->panel()->UI()->txtConsoleResults;
	for(size_t i = 0; i < context->argumentCount(); ++i)
	{
		edit->append(context->argument(i).toString());
	}
	return QScriptValue();
}
