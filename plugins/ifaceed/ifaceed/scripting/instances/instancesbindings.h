/*! \file instancesbindings.h
    \author HiddenSeeker

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

}

}
