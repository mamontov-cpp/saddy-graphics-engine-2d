#include "resource/abstractlink.h"
#include "resource/resource.h"

resource::AbstractLink::AbstractLink(const sad::String & resource_type)
: m_changed(false), m_resource(NULL),  m_path(""), m_tree(NULL)
{
	
}

resource::AbstractLink::~AbstractLink()
{
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
}

void resource::AbstractLink::attach(resource::Resource* r)
{
	m_resource->removeLink(this);
	m_resource = r;
	if (m_resource)
	{
		m_resource->addLink(this);			
	}
	m_changed = true;
}

void resource::AbstractLink::detach()
{
	m_changed = true;
	m_resource = NULL;
}

resource::Resource* resource::AbstractLink::resource() const
{
	if (m_resource == NULL && m_tree != NULL)
	{
		// TODO: Actually implement
	}
	return m_resource;
}

void resource::AbstractLink::setPath(sad::String path)
{
	m_path = path;
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
	m_changed = true;
	m_resource = NULL;
}

const sad::String&  resource::AbstractLink::path() const
{
	return m_path;	
}

void resource::AbstractLink::finishedChanging()
{
	m_changed = true;
}

bool resource::AbstractLink::changed() const
{
	return m_changed;
}

void  resource::AbstractLink::setTree(resource::Tree * tree)
{
	m_tree = tree;
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
	m_resource = NULL;
}

resource::Tree* resource::AbstractLink::tree() const
{
	return m_tree;	
}
