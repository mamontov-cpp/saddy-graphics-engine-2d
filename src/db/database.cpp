#include "db/database.h"

db::Database::~Database()
{

}

sad::String db::Database::save()
{
		return sad::String();	
}

void db::Database::saveToFile(sad::String s)
{
			
}

void db::Database::loadFromFile(sad::String name)
{
			
}

void db::Database::load(sad::String text)
{
			
}

void db::Database::addTable(sad::String name, db::Table* table)
{
			
}

void db::Database::removeTable(sad::String name)
{
			
}

unsigned long long db::Database::addMajor()
{
		return unsigned long long();	
}

bool db::Database::save(unsigned long long id)
{
		return bool();	
}

bool db::Database::save(unsigned long long id, sad::String name)
{
		return bool();	
}

sad::Vector<db::Error*> db::Database::load(sad::String name, unsigned long long& id)
{
		return sad::Vector<db::Error*>();	
}

db::Table* db::Database::table(sad::String name)
{
		return db::Table*();	
}

sad::Hash<sad::String, db::Table*>::iterator db::Database::begin()
{
		return sad::Hash<sad::String, db::Table*>::iterator();	
}

sad::Hash<sad::String, db::Table*>::iterator db::Database::end()
{
		return sad::Hash<sad::String, db::Table*>::iterator();	
}

db::ObjectFactory* db::Database::factory()
{
		return db::ObjectFactory*();	
}

void db::Database::setFactory(db::ObjectFactory* f)
{
			
}

