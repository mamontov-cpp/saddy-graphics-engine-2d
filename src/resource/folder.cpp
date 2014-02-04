#include "resource/folder.h"

resource::Folder::Folder()
{

}

resource::Folder::~Folder()
{

}

bool resource::Folder::addFolder(sad::String name, resource::Folder* folder)
{
		return bool();	
}

bool resource::Folder::addResource(sad::String name, resource::Folder* folder)
{
		return bool();	
}

void resource::Folder::removeFolder(sad::String name)
{
			
}

void resource::Folder::removeResource(sad::String name)
{
			
}

resource::Folder* resource::Folder::folder(sad::String name)
{
		return resource::Folder*();	
}

resource::Resource* resource::Folder::resource(sad::String name)
{
		return resource::Resource*();	
}

void resource::Folder::setName(sad::String name)
{
			
}

sad::String resource::Folder::name()
{
		return sad::String();	
}

void resource::Folder::replaceResource(sad::String name, resource::Resource* r)
{
			
}

sad::Hash<sad::String, resource::Folder*>::iterator resource::Folder::foldersBegin()
{
		return sad::Hash<sad::String, resource::Folder*>::iterator();	
}

sad::Hash<sad::String, resource::Folder*>::iterator resource::Folder::foldersEnd()
{
		return sad::Hash<sad::String, resource::Folder*>::iterator();	
}

sad::Hash<sad::String, resource::Resource*>::iterator resource::Folder::resourceBegin()
{
		return sad::Hash<sad::String, resource::Resource*>::iterator();	
}

sad::Hash<sad::String, resource::Resource*>::iterator resource::Folder::resourceEnd()
{
		return sad::Hash<sad::String, resource::Resource*>::iterator();	
}

