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

/*! Lists all grids
    \param[in] ctx context
    \param[in] engine an engine
    \return a ways list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

/*! Fetches a grid by it's name or major id
    \param[in] ctx context
    \param[in] engine an engine
	\return an object or null	
 */
QScriptValue query(
    QScriptContext* ctx,
    QScriptEngine* engine	
);

}

}