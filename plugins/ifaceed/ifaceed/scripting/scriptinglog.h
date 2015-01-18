/*! \file scriptinglog.h
	\author HiddenSeeker

	Contains function, which performs log to a value
 */
#pragma once
#include <QScriptContext>

namespace scripting
{

/*! Performs logging action, when scripting scripting
	\param[in] context a context, which contains arguments
	\param[in] engine an engine
	\return value
 */
QScriptValue scriptinglog(QScriptContext *context, QScriptEngine *engine);

}