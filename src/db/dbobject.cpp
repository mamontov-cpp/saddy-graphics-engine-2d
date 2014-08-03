#include "db/dbobject.h"
#include "db/schema/schema.h"

#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"

sad::db::Object::Object() : m_table(NULL), MajorId(0), MinorId(0)
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

sad::db::Table* sad::db::Object::table() const
{
	return m_table;
}

void sad::db::Object::setTable(sad::db::Table* t)
{
	m_table = t;
}


static sad::db::schema::Schema* DbObjectBasicSchema = NULL;
sad::db::schema::Schema* sad::db::Object::basicSchema()
{
	if (DbObjectBasicSchema == NULL)
	{
		DbObjectBasicSchema = new sad::db::schema::Schema();
		DbObjectBasicSchema->add("majorid", sad::db::define_field(&sad::db::Object::MajorId));
		DbObjectBasicSchema->add("minorid", sad::db::define_field(&sad::db::Object::MinorId));
		DbObjectBasicSchema->add("name"   , sad::db::define_field(&sad::db::Object::Name));
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

sad::db::Property* sad::db::Object::getObjectProperty(const sad::String& s) const
{
	sad::db::schema::Schema* schema = this->schema();
	sad::db::Property* result = NULL;
	if (schema)
	{
		result = schema->getProperty(s);
	}
	return result;
}
