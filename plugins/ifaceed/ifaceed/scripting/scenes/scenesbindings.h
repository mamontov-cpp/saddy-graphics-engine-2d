/*! \file scenesbindings.h
	

	Describes bindings for editing scenes
 */
#pragma once
#include <QScriptContext>
#include <QScriptEngine>

#include <QString>

#include <scene.h>

#include "../tovalue.h"

namespace scripting
{

class Scripting;

namespace scenes
{

/*! Adds new named scene to a bindings
	\param[in] s scripting part
    \param[in] name a name of scene
 */
unsigned long long add(scripting::Scripting* s, QString name);
/*! Adds new nameless scene
	\param[in] s scripting part
 */
unsigned long long addNameless(scripting::Scripting* s);
/*! Adds new scene with name from script
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void remove(scripting::Scripting* s, sad::Scene* scene);
/*! Moves scene back in list
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void moveBack(scripting::Scripting* s, sad::Scene* scene);
/*! Moves scene front in list
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void moveFront(scripting::Scripting* s, sad::Scene* scene);
/*! Lists database properties
	\param[in] ctx context
	\param[in] engine an engine list
	\return list of strings with propeties of database
 */
QScriptValue list(QScriptContext* ctx, QScriptEngine* engine);

}

}
