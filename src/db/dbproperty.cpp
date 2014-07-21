#include "db/dbproperty.h"

sad::db::Property::Property() : m_type_is_kind_of_sad_object(false), m_pointer_stars_count(0)
{

}


sad::db::Property::~Property()
{

}

const sad::String & sad::db::Property::type() const
{
	return m_base_type;
}

bool sad::db::Property::typeIsKindOfSadObject() const
{
	return m_type_is_kind_of_sad_object;
}


bool sad::db::Property::check(const sad::String& key, const picojson::value& v)
{
	return true;
}