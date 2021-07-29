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

sad::db::Object::Object() : m_table(nullptr), MajorId(0), MinorId(0), Active(true)
{

}

sad::db::Object::~Object()
{

}

void sad::db::Object::save(picojson::value & v)
{
    sad::db::schema::Schema * schema = this->schema();
    if (schema)
    {
        schema->save(this, v);
    }
}

bool sad::db::Object::load(const picojson::value& v)
{
    sad::db::schema::Schema * schema = this->schema();
    if (schema)
    {
        return schema->load(this, v);
    }
    return false;
}

void sad::db::Object::reset()
{

}

sad::db::Table* sad::db::Object::table() const
{
    return m_table;
}

void sad::db::Object::setTable(sad::db::Table* t)
{
    m_table = t;
}


static sad::db::schema::Schema* DbObjectBasicSchema = nullptr;
static sad::Mutex DbObjectBasicSchemaInit;
sad::db::schema::Schema* sad::db::Object::basicSchema()
{
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
    return  sad::db::Object::basicSchema();
}

static sad::String DbObjectClassName = "sad::db::Object";

const sad::String& sad::db::Object::serializableName() const
{
    return DbObjectClassName;
}

const sad::String& sad::db::Object::objectName() const
{
    return this->m_name;
}

void sad::db::Object::setObjectName(const sad::String & newname)
{
    if (table())
    {
        this->table()->changeObjectName(this, this->m_name, newname);
    }
    this->m_name = newname;
}


void sad::db::Object::setTreeName(
    sad::Renderer* renderer,
    const sad::String& treename
)
{

}

sad::db::Property* sad::db::Object::getObjectProperty(const sad::String& s) const
{
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
    return this->serializableName() == name || name == "sad::db::Object";
}
