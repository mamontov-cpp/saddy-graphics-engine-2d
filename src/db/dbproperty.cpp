#include "db/dbproperty.h"

sad::db::Property::Property() : m_o(NULL)
{

}


sad::db::Property::~Property()
{

}


bool sad::db::Property::check(const sad::String& key, const picojson::value& v)
{
	return true;
}