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

/*! Fetches a grid by it's name or major id. Takes name or major id of object
    \param[in] ctx context
    \param[in] engine an engine
	\return an object or null	
 */
QScriptValue query(
    QScriptContext* ctx,
    QScriptEngine* engine	
);

/*! Adds new grid to list. Takes one parameter as name or nothing.
    \param[in] ctx context
    \param[in] engine an engine
	\return a scriptable reference to a new grid
 */
QScriptValue add(
    QScriptContext* ctx,
    QScriptEngine* engine	
);

/*!	Removes a grid from a list. Takes name or major id of node
    \param[in] ctx context
    \param[in] engine an engine
	\return a scriptable reference to a new grid
 */
QScriptValue remove(
    QScriptContext* ctx,
    QScriptEngine* engine	
);


}

}
