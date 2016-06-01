/*! \file scripting/layouts/gridbindings.h

    A grid scripting bindings should be placed here
 */
#pragma once
#pragma once

#include <QScriptEngine>

namespace scripting
{

class Scripting;

namespace layouts
{

/*! Lists all ways
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