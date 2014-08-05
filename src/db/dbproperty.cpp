#include "db/dbproperty.h"

sad::db::Property::Property() : m_type_is_kind_of_sad_object(false), m_pointer_stars_count(0)
{

}


sad::db::Property::~Property()
{

}

const sad::String & sad::db::Property::baseType() const
{
	return m_base_type;
}

bool sad::db::Property::typeIsKindOfSadObject() const
{
	return m_type_is_kind_of_sad_object;
}

int sad::db::Property::pointerStarsCount() const
{
	return m_pointer_stars_count;
}


bool sad::db::Property::check(const sad::String& key, const picojson::value& v)
{
	return true;
}

bool  sad::db::Property::hasEqualTypeAs(sad::db::Property * o) const
{
	return baseType() == o->baseType() 
		&& typeIsKindOfSadObject() == o->typeIsKindOfSadObject()	
		&& pointerStarsCount() == o->pointerStarsCount();
}
