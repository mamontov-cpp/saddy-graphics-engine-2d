#include "resource/factory.h"

resource::Creator::~Creator()
{

}

resource::Factory::~Factory()
{

}

void resource::Factory::add(const sad::String & name, resource::Creator * c)
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

resource::Resource* resource::Factory::create(sad::String name)
{
	resource::Resource* result = NULL;
	if (m_creators.contains(name))
	{
		resource::Creator * c = m_creators[name];
		result = c->create();	
	}
	return result;	
}

