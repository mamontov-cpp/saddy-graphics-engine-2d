#include "resource/abstractlink.h"
#include "resource/resource.h"
#include "resource/tree.h"
#include "resource/folder.h"

sad::resource::AbstractLink::AbstractLink(const sad::String & resource_type)
: 
m_changed(false), 
m_resource(NULL),  
m_path(""), 
m_tree(NULL),
m_resource_type(resource_type)
{
	
}

sad::resource::AbstractLink::~AbstractLink()
{
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
}

void sad::resource::AbstractLink::attach(resource::Resource* r)
{
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
	m_resource = r;
	if (m_resource)
	{
		m_resource->addLink(this);			
	}
	m_changed = true;
}

void sad::resource::AbstractLink::detach()
{
	m_changed = true;
	m_resource = NULL;
}

sad::resource::Resource* sad::resource::AbstractLink::resource() const
{
	if (m_resource == NULL && m_tree != NULL)
	{
		sad::resource::Resource * res = m_tree->root()->resource(m_path);
		if (res)
		{
			if (res->metaData()->canBeCastedTo(m_resource_type))
			{
				res = static_cast<sad::resource::Resource*>(res->metaData()->castTo(res, m_resource_type));
				const_cast<sad::resource::AbstractLink*>(this)->attach(res);
			}
		}
	}
	return m_resource;
}

void sad::resource::AbstractLink::setPath(sad::String path)
{
	m_path = path;
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
	m_changed = true;
	m_resource = NULL;
}

const sad::String&  sad::resource::AbstractLink::path() const
{
	return m_path;	
}

void sad::resource::AbstractLink::finishedChanging()
{
	m_changed = true;
}

bool sad::resource::AbstractLink::changed() const
{
	return m_changed;
}

void  sad::resource::AbstractLink::setTree(resource::Tree * tree)
{
	m_tree = tree;
	if (m_resource)
	{
		m_resource->removeLink(this);
	}
	m_resource = NULL;
}

sad::resource::Tree* sad::resource::AbstractLink::tree() const
{
	return m_tree;	
}
