/*! \file scripting/layouts/gridbindings.h

    A grid scripting bindings should be placed here
 */
#pragma once
#pragma once

#include <QScriptEngine>
#include "scriptablegrid.h"

namespace scripting
{

class Scripting;

namespace layouts
{

/*! Lists all grids, present in database, returning list of their major id
    \param[in] ctx context
    \param[in] engine an engine
    \return list of major ids for all grids in database
 */
QVector<unsigned long long> list();

/*! Fetches a grid by it's name or major id. Takes name or major id of object
    \param[in] s scripting
    \param[in] grid a grid value
    \return result
 */
scripting::layouts::ScriptableGrid* _query(scripting::Scripting* s, sad::layouts::Grid* grid);

/*! Adds new grid to list. Takes a name, which can be empty.
    \param[in] s scripting value
    \param[in] name a name for object
    \return a scriptable reference to a new grid
 */
scripting::layouts::ScriptableGrid* add(scripting::Scripting* s, const sad::String& name);

/*!	Removes a grid from a list. Takes name or major id of node
    \param[in] ctx context
    \param[in] engine an engine
    \return a scriptable reference to a new grid
 */
QScriptValue remove(
    QScriptContext* ctx,
    QScriptEngine* engine	
);

/*!	Returns a position of object in grid if any grid is set as parent for an object
    \param[in] ctx context
    \param[in] engine an engine
    \return empty array if nothing is found, otherwise to elements - cell reference and position
 */
QScriptValue parent(
    QScriptContext* ctx,
    QScriptEngine* engine
);

}

}
