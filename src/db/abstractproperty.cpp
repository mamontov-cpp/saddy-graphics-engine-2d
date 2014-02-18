#include "db/abstractproperty.h"

db::AbstractProperty::~AbstractProperty()
{

}

void db::AbstractProperty::set(db::Variant v)
{
			
}

db::Variant db::AbstractProperty::get()
{
		return db::Variant();	
}

picojson::value db::AbstractProperty::save()
{
		return picojson::value();	
}

void db::AbstractProperty::load(picojson::value a)
{
			
}

bool db::AbstractProperty::saveable()
{
		return bool();	
}

void db::AbstractProperty::call()
{
			
}

