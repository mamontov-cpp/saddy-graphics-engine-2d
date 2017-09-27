#include "querytable.h"

#include <QVector>

#include <renderer.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>


QVector<unsigned long long> scripting::query_table(
    const sad::String& table,
    const sad::String& typeofobjects
)
{
    sad::db::Database* db = sad::Renderer::ref()->database("");
    sad::Vector<sad::db::Object*> objs;
    db->table(table)->objects(objs);

    QVector<unsigned long long> result;
    for(size_t i = 0; i < objs.size(); i++)
    {
        if (objs[i]->Active && objs[i]->isInstanceOf(typeofobjects))
        {
            result << objs[i]->MajorId;
        }
    }

    return result;
}
