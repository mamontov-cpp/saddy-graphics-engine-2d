#include <resource/textureatlasfile.h>

sad::resource::TextureAtlasFile::TextureAtlasFile(const sad::String& name) 
: sad::resource::PhysicalFile(name)
{
	
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::load(
	sad::resource::Folder * parent
)
{
	// TODO: Implement
	sad::Vector<sad::resource::Error*> errors;

	return errors;
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::reload()
{
	// TODO: Implement
	return sad::Vector<sad::resource::Error*>();	
}
