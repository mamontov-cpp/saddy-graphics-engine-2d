/*! \file groupsbindings.h
    \author HiddenSeeker

    A bindings for animation instances are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>
#include <maybe.h>

#include <QScriptEngine>
#include <QVector>

#include <animations/animationsgroup.h>

namespace scripting
{

class Scripting;

namespace groups
{

/*! Lists all animation groups
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
