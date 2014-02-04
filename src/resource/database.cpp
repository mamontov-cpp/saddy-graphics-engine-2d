#include "resource/database.h"

resource::Database::~Database()
{

}

void resource::Database::initFromString(sad::String string)
{
			
}

void resource::Database::initFromFile(sad::String string)
{
			
}

bool resource::Database::load(sad::String file)
{
		return bool();	
}

bool resource::Database::unload(sad::String file)
{
		return bool();	
}

resource::Folder* resource::Database::root()
{
		return resource::Folder*();	
}

resource::PhysicalFile* resource::Database::file(sad::String name)
{
		return resource::PhysicalFile*();	
}

sad::Vector<resource::PhysicalFile*>& resource::Database::files()
{
		return sad::Vector<resource::PhysicalFile*>&();	
}

void resource::Database::addFormat(resource::FileFormat* format)
{
			
}

void resource::Database::clearFormats()
{
			
}

resource::ResourceFactory* resource::Database::resourceFactory()
{
		return resource::ResourceFactory*();	
}

void resource::Database::setResourceFactory(resource::ResourceFactory* factory)
{
			
}

