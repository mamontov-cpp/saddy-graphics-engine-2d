#include "db/dbdatabase.h"

sad::db::Database::~Database()
{

}

void sad::db::Database::save(sad::String & output)
{

}

void sad::db::Database::saveToFile(const sad::String& s)
{
			
}

bool sad::db::Database::load(const sad::String& text)
{
			
}

bool sad::db::Database::loadFromFile(const sad::String& name)
{
			
}

bool sad::db::Database::addTable(const sad::String& name, db::Table* table)
{
	return false;		
}

void sad::db::Database::removeTable(const sad::String& name)
{
	
}

unsigned long long sad::db::Database::uniqueMajorId(sad::db::Table * t)
{
	return 0;	
}

sad::db::Table* sad::db::Database::table(const sad::String& name)
{
	return NULL;	
}

sad::db::ObjectFactory* sad::db::Database::factory()
{
	return m_factory;	
}

void sad::db::Database::setFactory(sad::db::ObjectFactory* f)
{
			
}

