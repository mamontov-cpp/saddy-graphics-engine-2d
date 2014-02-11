#include "resource/spritesheetfile.h"

sad::resource::SpriteSheetFile::SpriteSheetFile(const sad::String& name) 
: sad::resource::PhysicalFile(name)
{
	
}

sad::Vector<sad::resource::Error*> sad::resource::SpriteSheetFile::load(
	sad::resource::Folder * parent
)
{
	// TODO: Implement
	return sad::Vector<sad::resource::Error*>();
}

sad::Vector<sad::resource::Error*> sad::resource::SpriteSheetFile::reload()
{
	// TODO: Implement
	return sad::Vector<sad::resource::Error*>();	
}
