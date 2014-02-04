#include "resource/resource.h"

resource::Resource::~Resource()
{

}

sad::String resource::Resource::resourceType()
{
		return sad::String();	
}

void resource::Resource::load(resource::PhysicalFile file)
{
			
}

void resource::Resource::replaceWith(resource::Resource* a)
{
			
}

sad::Vector<resource::Error*> resource::Resource::reload()
{
		return sad::Vector<resource::Error*>();	
}

void resource::Resource::setParentFolder(resource::Folder* folder)
{
			
}

resource::Folder* resource::Resource::parentFolder()
{
		return resource::Folder*();	
}

sad::String resource::Resource::name()
{
		return sad::String();	
}

