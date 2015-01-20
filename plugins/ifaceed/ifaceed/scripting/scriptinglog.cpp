#include "scriptinglog.h"

#include "scripting.h"

#include "classwrapper.h"

#include "../mainpanel.h"

#include "../core/editor.h"

QScriptValue scripting::scriptinglog(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	QTextEdit* edit = e->panel()->UI()->txtConsoleResults;
	for(size_t i = 0; i < context->argumentCount(); ++i)
	{
		QScriptValue arg = context->argument(i);
		bool handled = false;
		if (arg.isQObject())
		{
			QObject* o = arg.toQObject();
			scripting::ClassWrapper* wrapper = qobject_cast<scripting::ClassWrapper*>(o);
			if (wrapper)
			{
				handled = true;
				edit->append(wrapper->toString());
			}
		}
		if (handled == false)
		{
			edit->append(context->argument(i).toString());
		}
	}
	return QScriptValue();
}
