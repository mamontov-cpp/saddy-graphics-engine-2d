/*! \file scenebindings.h
	\author HiddenSeeker

	Describes bindings for editing scenes
 */
#pragma once
#include <QScriptContext>
#include <QScriptEngine>

#include <QString>

#include "scene.h"

namespace scripting
{

class Scripting;

/*! Adds new named scene to a bindings
	\param[in] s scripting part
 */
unsigned long long addScene(scripting::Scripting* s, QString name);
/*! Adds new nameless scene
	\param[in] s scripting part
 */
unsigned long long addNamelessScene(scripting::Scripting* s);
/*! Adds new scene with name from script
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void removeScene(scripting::Scripting* s, sad::Scene* scene);
/*! Moves scene back in list
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void moveSceneBack(scripting::Scripting* s, sad::Scene* scene);
/*! Moves scene front in list
	\param[in] s scripting part
	\param[in] scene scene to be removed
 */
void moveSceneFront(scripting::Scripting* s, sad::Scene* scene);

}
