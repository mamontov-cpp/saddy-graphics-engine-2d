/*! \file scenesbindings.h
    

    Describes bindings for editing scenes
 */
#pragma once

#include <QString>

#include <scene.h>

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
/*! Returns all major ids for scenes in database
    \return list of major ids for scenes in database
 */
QVector<unsigned long long> list();

/*! Initializes bindings for scenes
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created scenes object
*/
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
