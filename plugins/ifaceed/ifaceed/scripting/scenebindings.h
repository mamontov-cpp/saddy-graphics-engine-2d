/*! \file scenebindings.h
	\author HiddenSeeker

	Describes bindings for editing scenes
 */
#include <QScriptContext>
#include <QScriptEngine>

namespace scripting
{
/*! Adds new scene with name from script
	\param[in] context a context name
	\param[in] engine an engine name
 */
QScriptValue addScene(QScriptContext *context, QScriptEngine *engine);

/*! Adds new scene with name from script
	\param[in] context a context name
	\param[in] engine an engine name
 */
QScriptValue removeScene(QScriptContext *context, QScriptEngine *engine);

/*! Fetches scene and tries to move it back
	\param[in] context a context name
	\param[in] engine an engine name
 */
QScriptValue sceneMoveBack(QScriptContext *context, QScriptEngine *engine);

/*! Fetches scene and tries to move it back
	\param[in] context a context name
	\param[in] engine an engine name
 */
QScriptValue sceneMoveFront(QScriptContext *context, QScriptEngine *engine);

}
