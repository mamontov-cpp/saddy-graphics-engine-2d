/*! \file queryobject.h
    

    A function for querying object from database
 */
#pragma once
#include <QVariant>

#include <maybe.h>

#include <db/dbobject.h>
#include <db/dbtypename.h>

namespace scripting
{
/*! Queries object by name or by major id, depending from name
    \param[in] v value
    \return NULL if not found
 */
sad::db::Object* query_object(const QVariant& v);

template<
    typename T
>
sad::Maybe<T> query(const QVariant& v)
{
    sad::Maybe<T> result;
    sad::db::Object* object = scripting::query_object(v);
    if (object)
    {
        sad::db::TypeName<T>::init();
        if (object->isInstanceOf(sad::db::TypeName<T>::baseName()))
        {
            result.setValue(static_cast<T>(object));
        }
    }
    return result;
}

}
