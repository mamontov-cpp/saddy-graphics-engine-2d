/*! \file queryresource.h
    

    Defines a most of resource querying options, thus allowing
    to perform lookup for most of object stuff
  */
#pragma once
#include <QScriptEngine>
#include <QString>

namespace scripting
{

class Scripting;

/*! Returns resource type for scripting (empty string if not found)
    \param[in] scripting a scripting part
    \param[in] name name of resource
    \return resource type
 */
QString resource_type(scripting::Scripting* scripting, QString name);

/*! Returns object wrapper for sad::Sprite2D::Options. Note, that this
    will return just a simple object, not a mutable thing, because we should
    not change stuff in tree. If resource is not found or not
    sad::Sprite2D::Options, null is returned
    \param[in] scripting a scripting part
    \param[in] name name of resource
    \return object
 */
QScriptValue resource_options(scripting::Scripting* scripting, QString name);

/*! Returns object wrapper for sad::db::custom::Schema. Note, that this
    will return just a simple object, not a mutable thing, because we should
    not change stuff in tree. If resource is not found or not
    sad::Sprite2D::Options, null is returned
    \param[in] scripting a scripting part
    \param[in] name name of resource
    \return object
 */
QScriptValue resource_schema(scripting::Scripting* scripting, QString name);

}
