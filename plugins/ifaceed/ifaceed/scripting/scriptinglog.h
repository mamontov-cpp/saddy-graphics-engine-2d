/*! \file scriptinglog.h
	

	Contains function, which performs log to a value
 */
#pragma once
#include <QScriptContext>

namespace scripting
{

/*! Logs object to a string
	\param[in] v value
	\param[in] engine an engine to be object
	\return string
 */
QString  scripting_log_object(const QScriptValue& v, QScriptEngine *engine);

/*! Performs logging action, when scripting scripting
	\param[in] context a context, which contains arguments
	\param[in] engine an engine
	\return value
 */
QScriptValue scripting_log(QScriptContext *context, QScriptEngine *engine);

}
