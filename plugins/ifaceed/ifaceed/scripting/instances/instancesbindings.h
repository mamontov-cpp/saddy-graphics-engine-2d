/*! \file instancesbindings.h
    

    A bindings for animation instances are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>
#include <maybe.h>

#include <QScriptEngine>
#include <QVector>

#include <animations/animationsinstance.h>
#include <animations/animationswayinstance.h>

namespace scripting
{

class Scripting;

namespace instances
{

/*! Lists all animation instances
    \param[in] ctx context
    \param[in] engine an enginge
    \return a ways list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

/*! Adds animation instance. Prefixed by underscore, since it will be mapped to _addInstance function and addInstance
    is reserved for call, which will take object, preprocess it's fields and call _addInstance using fields of this object.
    \param[in] scripting a scripting part
    \param[in] name a name for object
    \param[in] animationid major id of animation to be added
    \param[in] animationname name of animation if added as resource
    \param[in] object an object to be added
    \param[in] starttime a start time for instance
    \return major id for animation if added, 0 otherwise
 */
unsigned long long _addInstance(
    scripting::Scripting* scripting,
    sad::String name,
    unsigned long long animationid,
    sad::String animationname,
    QScriptValue object,
    double starttime
);

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
    QScriptValue way,
    QScriptValue object,
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

}

}
