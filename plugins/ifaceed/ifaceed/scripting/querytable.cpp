#include "querytable.h"
#include "fromvalue.h"

#include <QVector>
#include <QScriptContext>

#include <renderer.h>
#include <db/dbdatabase.h>
#include <db/dbtable.h>


QScriptValue scripting::query_table(
    const sad::String& table,
    const sad::String& typeofobjects,
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
    if (ctx->argumentCount() != 0)
    {
        ctx->throwError("list: accepts only 0 arguments");
    }

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

    return scripting::FromValue<QVector<unsigned long long> >::perform(result, engine);
}
