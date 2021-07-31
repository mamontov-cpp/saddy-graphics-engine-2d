#include "querytable.h"

#include <QVector>

#include <renderer.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>


QVector<unsigned long long> scripting::query_table(
    const sad::String& table,
    const sad::String& type_of_objects
)
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    sad::Vector<sad::db::Object*> objects;
    db->table(table)->objects(objects);

    QVector<unsigned long long> result;
    for(size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i]->Active && objects[i]->isInstanceOf(type_of_objects))
        {
            result << objects[i]->MajorId;
        }
    }

    return result;
}
