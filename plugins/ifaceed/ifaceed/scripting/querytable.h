/*! \file querytable.h
    \author HiddenSeeker

    Queries table for objects, returning list of major ids for objects
 */
#pragma once
#include <QScriptEngine>
#include <sadstring.h>

class QScriptContext;

namespace scripting
{

/*! Queries table, returning list of major ids for active objects
    with specified types
    \param[in] table name of table
    \param[in] typeofobjects type, that most ob objects have
    \param[in] ctx context
    \param[in] engine engine part
    \return list of major ids
 */
QScriptValue query_table(
    const sad::String& table,
    const sad::String& typeofobjects,
    QScriptContext* ctx,
    QScriptEngine* engine
);

}
