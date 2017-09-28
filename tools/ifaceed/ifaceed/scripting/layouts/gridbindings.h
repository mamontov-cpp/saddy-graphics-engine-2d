/*! \file scripting/layouts/gridbindings.h

    A grid scripting bindings should be placed here
 */
#pragma once
#pragma once

#include <QVariant>
#include "../dukqtcontext.h"
#include "scriptablegrid.h"

namespace scripting
{

class Scripting;

namespace layouts
{

/*! Lists all grids, present in database, returning list of their major id
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

/*!	Removes a grid from a list. When called, from script, takes name or major id of node
    \param[in] s scripting
    \param[in] grid a grid to be removed
    \return a scriptable reference to a new grid
 */
void remove(
    scripting::Scripting* s,
    sad::layouts::Grid* grid
);

/*!	Returns a position of object in grid if any grid is set as parent for an object
    \param[in] scripting a scripting context
    \param[in] node a node
    \return null if nothing is found, otherwise to elements - cell reference and position
 */
dukpp03::Maybe<QVector<QVariant> > parent(
    scripting::Scripting* scripting,
    sad::SceneNode* node
);

/*! Initializes bindings for grids
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created layout grid object
*/
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
