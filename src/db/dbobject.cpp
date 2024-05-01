#include "db/dbobject.h"
#include "db/dbtable.h"
#include "db/schema/schema.h"

#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

#include "sadmutex.h"
#include "classmetadatacontainer.h"
#include "opticksupport.h"

sad::db::Object::Object() : m_table(nullptr), MajorId(0), MinorId(0), Active(true)
{
    PROFILER_EVENT;

}

sad::db::Object::~Object()
{
    PROFILER_EVENT;

}

void sad::db::Object::save(picojson::value & v)
{
    PROFILER_EVENT;
    sad::db::schema::Schema * schema = this->schema();
    if (schema)
    {
        schema->save(this, v);
    }
}

bool sad::db::Object::load(const picojson::value& v)
{
    PROFILER_EVENT;
    sad::db::schema::Schema * schema = this->schema();
    if (schema)
    {
        return schema->load(this, v);
    }
    return false;
}

void sad::db::Object::reset()
{
    PROFILER_EVENT;

}

sad::db::Table* sad::db::Object::table() const
{
    PROFILER_EVENT;
    return m_table;
}

void sad::db::Object::setTable(sad::db::Table* t)
{
    PROFILER_EVENT;
    m_table = t;
}


static sad::db::schema::Schema* DbObjectBasicSchema = nullptr;
static sad::Mutex DbObjectBasicSchemaInit;
sad::db::schema::Schema* sad::db::Object::basicSchema()
{
    PROFILER_EVENT;
    if (DbObjectBasicSchema == nullptr)
    {
        DbObjectBasicSchemaInit.lock();
        if (DbObjectBasicSchema == nullptr)
        {
            DbObjectBasicSchema = new sad::db::schema::Schema();
            DbObjectBasicSchema->add("majorid", sad::db::define_field(&sad::db::Object::MajorId));
            DbObjectBasicSchema->add("minorid", sad::db::define_field(&sad::db::Object::MinorId));
            DbObjectBasicSchema->add(
                "name",
                new sad::db::MethodPair<sad::db::Object, sad::String>(
                    &sad::db::Object::objectName,
                    &sad::db::Object::setObjectName
                )
            );
            DbObjectBasicSchema->add("active" , sad::db::define_field(&sad::db::Object::Active));

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(DbObjectBasicSchema);
        }
        DbObjectBasicSchemaInit.unlock();
    }
    return DbObjectBasicSchema;
}

sad::db::schema::Schema * sad::db::Object::schema() const
{
    PROFILER_EVENT;
    return  sad::db::Object::basicSchema();
}

static sad::String DbObjectClassName = "sad::db::Object";

const sad::String& sad::db::Object::serializableName() const
{
    PROFILER_EVENT;
    return DbObjectClassName;
}

const sad::String& sad::db::Object::objectName() const
{
    PROFILER_EVENT;
    return this->m_name;
}

void sad::db::Object::setObjectName(const sad::String & new_name)
{
    PROFILER_EVENT;
    if (table())
    {
        this->table()->changeObjectName(this, this->m_name, new_name);
    }
    this->m_name = new_name;
}


void sad::db::Object::setTreeName(
    sad::Renderer* renderer,
    const sad::String& tree_name
)
{
    PROFILER_EVENT;

}

sad::db::Property* sad::db::Object::getObjectProperty(const sad::String& s) const
{
    PROFILER_EVENT;
    sad::db::schema::Schema* schema = this->schema();
    sad::db::Property* result = nullptr;
    if (schema)
    {
        result = schema->getProperty(s);
    }
    return result;
}

bool sad::db::Object::isInstanceOf(const sad::String& name)
{
    PROFILER_EVENT;
    return this->serializableName() == name || name == "sad::db::Object";
}
