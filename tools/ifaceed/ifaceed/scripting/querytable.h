/*! \file querytable.h
    

    Queries table for objects, returning list of major ids for objects
 */
#pragma once
#include <QVector>
#include <sadstring.h>

namespace scripting
{

/*! Queries table, returning list of major ids for active objects
    with specified types
    \param[in] table name of table
    \param[in] typeofobjects type, that most ob objects have
    \return list of major ids
 */
QVector<unsigned long long> query_table(
    const sad::String& table,
    const sad::String& typeofobjects
);

}
