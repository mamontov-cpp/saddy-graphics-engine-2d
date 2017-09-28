/*! \file instancesbindings.h
    

    A bindings for animation instances are listed here
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadrect.h>

#include <QVariant>
#include <QVector>

#include <animations/animationsinstance.h>
#include <animations/animationswayinstance.h>

namespace scripting
{

class Scripting;

namespace instances
{

/*! Lists all animation instances, returning their major ids as list
    \return a list of all major ids for animation instances in database
 */
QVector<unsigned long long> list();

/*! Adds way animation instance. Prefixed by underscore, since it will be mapped to _addWayInstance function and addWayInstance
    is reserved for call, which will take object, preprocess it's fields and call _addWayInstance using fields of this object.
    \param[in] scripting a scripting part
    \param[in] name a name for object
    \param[in] way a way to be bind to
    \param[in] object an object to be added
    \param[in] starttime a start time for instance
    \return major id for animation if added, 0 otherwise
 */
unsigned long long _addWayInstance(
    scripting::Scripting* scripting,
    sad::String name,
    const QVariant& way,
    const QVariant& object,
    double starttime
);

/*! Removes animation instance
    \param scripting a scripting part
    \param instance an instance
 */
void remove(
    scripting::Scripting* scripting,
    sad::animations::Instance* instance
);

/*! Sets readable or writable properties for animation instances
    \param[in] obj object
    \param[out] list a list of properties
    \param[in] readable a readable or writable properties
 */
void checkProperties(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool readable
);

/*! Initializes bindings for animation instances
    \param[in] s scripting object
    \param[in] animations a global binding object for animations
    \return created animations object
*/
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* animations);

}

}
