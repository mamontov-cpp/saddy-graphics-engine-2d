/*! \file queryresource.h
    \author HiddenSeeker

    Defines a most of resource querying options, thus allowing
    to perform lookup for most of object stuff
  */
#pragma once
#include <sadstring.h>

#include <QScriptEngine>

namespace scripting
{

class Scripting;

/*! Returns resource type for scripting (empty string if not found)
    \param[in] scripting a scripting part
    \param[in] name name of resource
    \return resource type
 */
sad::String resource_type(scripting::Scripting* scripting, sad::String& name);

/*! Returns object wrapper for sad::Sprite2D::Options. Note, that this
    will return just a simple object, not a mutable thing, because we should
    not change stuff in tree. If resource is not found or not
    sad::Sprite2D::Options, null is returned
    \param[in] scripting a scripting part
    \param[in] name name of resource
    \return object
 */
QScriptValue resource_options(scripting::Scripting* scripting, sad::String& name);

}
