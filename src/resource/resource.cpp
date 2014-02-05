#include "resource/resource.h"
#include "resource/abstractlink.h"

#include <algorithm>

DECLARE_SOBJ(resource::Resource);


resource::Resource::Resource() : m_folder(NULL), m_store_links(true), m_file(NULL)
{
	
}

resource::Resource::~Resource()
{
	for(size_t i = 0; i < m_links.size(); i++)
	{
        m_links[i]->detach();
	}
}

bool resource::Resource::referenced() const
{
	return m_links.count() != 0;
}

void resource::Resource::replaceWith(resource::Resource* a)
{
	a->m_links << this->m_links;
	// Notify links, that we are gone
	for(size_t i = 0; i < m_links.size(); i++)
	{
        m_links[i]->attach(a);
	}
	// Clear self links
	m_links.clear();
}


void resource::Resource::setParentFolder(resource::Folder* folder)
{
	m_folder = folder;
}

resource::Folder* resource::Resource::parentFolder()
{
	return m_folder;
}

const sad::String& resource::Resource::name()
{
	return m_name;	
}

void resource::Resource::setName(const sad::String & name)
{
	m_name = name;
}

void resource::Resource::addLink(resource::AbstractLink* link)
{	
	if (m_store_links && link)
	{
		if (std::find(m_links.begin(), m_links.end(), link) == m_links.end())
		{
			m_links << link;
		}
	}
}

void resource::Resource::removeLink(resource::AbstractLink* link)
{
	if (m_store_links && link)
	{
		m_links.removeFirst(link);
	}		
}


void resource::Resource::enableStoringLinks()
{
	m_store_links = true;	
}

void resource::Resource::disableStoringLinks()
{
	m_store_links = false;
	m_links.clear();
}

void resource::Resource::setPhysicalFile(resource::PhysicalFile * file)
{
	m_file = file;	
}

resource::PhysicalFile * resource::Resource::file() const
{
	return m_file;	
}
