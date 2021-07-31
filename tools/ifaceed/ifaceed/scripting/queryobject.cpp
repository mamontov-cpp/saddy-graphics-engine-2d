#include "queryobject.h"

#include <renderer.h>

#include <db/dbdatabase.h>

sad::db::Object* scripting::query_object(const QVariant& v)
{
    
    sad::db::Object* result = nullptr;
    if (v.canConvert(QVariant::String))
    {
        sad::String name = v.value<QString>().toStdString();
        sad::Vector<sad::db::Object*> vector = sad::Renderer::ref()->database("")->queryByName(name);
        if (vector.size())
        {
            for(size_t i = 0; i < vector.size() && !result; i++)
            {
                if (vector[i]->Active) {
                    result = vector[i];
                }
            }
        }
    }

    if (result == nullptr)
    {
        if (v.canConvert(QVariant::ULongLong)) 
        {
            sad::Maybe<unsigned long long> maybemajorid = v.value<unsigned long long>();
            if (maybemajorid.exists())
            {
                sad::db::Object* object = sad::Renderer::ref()->database("")->queryByMajorId(maybemajorid.value());
                if (object)
                {
                    if (object->Active == false)
                    {
                        object = nullptr;
                    }
                }
                result = object;
            }
        }
    }

    return result;
}
