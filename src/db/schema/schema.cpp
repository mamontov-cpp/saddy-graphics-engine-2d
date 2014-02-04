#include "db/schema/schema.h"

db::schema::Schema::~Schema()
{

}

bool db::schema::Schema::check(picojson::value v)
{
		return bool();	
}

void db::schema::Schema::load(picojson::value v)
{
			
}

picojson::value db::schema::Schema::save()
{
		return picojson::value();	
}

