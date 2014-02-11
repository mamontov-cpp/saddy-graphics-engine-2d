#include "resource/resourcefactory.h"
#include "resource/physicalfile.h"
#include "resource/textureatlasfile.h"

sad::resource::Creator::~Creator()
{

}

sad::resource::Factory::~Factory()
{

}

void sad::resource::Factory::add(const sad::String & name, resource::Creator * c)
{
	if (c)
	{
		if (m_creators.contains(name))
		{
			delete m_creators[name];
		}
		m_creators.insert(name, c);
	}
}

sad::resource::Resource* sad::resource::Factory::create(const sad::String& name)
{
	sad::resource::Resource* result = NULL;
	if (m_creators.contains(name))
	{
		resource::Creator * c = m_creators[name];
		result = c->create();	
	}
	return result;	
}

sad::resource::PhysicalFile * sad::resource::Factory::fileByType(const sad::String & typehint)
{
	if (typehint == "sad::resource::TextureAtlasFile")
	{
		return new sad::resource::TextureAtlasFile();
	}
	return new sad::resource::PhysicalFile();
}

