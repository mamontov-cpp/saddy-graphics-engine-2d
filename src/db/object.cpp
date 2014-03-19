#include "db/object.h"

db::Object::~Object()
{

}

picojson::value db::Object::save()
{
		return picojson::value();	
}

void db::Object::load(picojson::value v)
{
			
}

db::Table* db::Object::table()
{
		return db::Table*();	
}

void db::Object::setTable(db::Table* t)
{
			
}

