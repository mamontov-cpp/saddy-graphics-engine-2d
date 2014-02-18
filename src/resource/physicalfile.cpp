#include "resource/physicalfile.h"

#include "resource/tree.h"

#include <algorithm>

sad::resource::PhysicalFile::PhysicalFile(const sad::String & name) 
: m_name(name), m_tree(NULL)
{
			
}


sad::resource::PhysicalFile::~PhysicalFile()
{

}


bool sad::resource::PhysicalFile::isAnonymous() const
{
	return m_name.length() != 0;	
}

const sad::String & sad::resource::PhysicalFile::name() const
{
	return m_name;	
}

void sad::resource::PhysicalFile::setName(const sad::String & name)
{
	m_name = name;
}

sad::Vector<sad::resource::Error*> sad::resource::PhysicalFile::load(sad::resource::Folder * parent)
{
	sad::Vector<sad::resource::Error*>  result;
	result << new sad::resource::FileLoadingNotImplemented(m_name);
	return result;
}

sad::Vector<sad::resource::Error*> sad::resource::PhysicalFile::reload()
{
	sad::Vector<sad::resource::Error*> errors;

	sad::resource::ResourceEntryList list;
	for(size_t i = 0; i < m_resources.size() && errors.size() == 0; i++)
	{
		sad::resource::Resource * res = m_tree->factory()->create(
			m_resources[i]->factoryName()
		);
		if (res)
		{
			res->setFactoryName(m_resources[i]->factoryName());
			sad::Maybe<sad::String> path = m_tree->root()->find(m_resources[i]); 
			if (path.exists())
			{
				bool ok = res->tryLoad(
					*this, 
					m_tree->renderer(), 
					m_resources[i]->options(), 
					m_resources[i]->shouldStoreLinks()
				);
				if (ok)
				{				
					list << sad::resource::ResourceEntry(path.value(), res);
				}
				else
				{
					errors << new sad::resource::ResourceLoadError(path.value());
				}
			}
			else
			{
				delete res;
				errors << new sad::resource::AnonymousResource(m_resources[i]->factoryName());
			}
		}
		else
		{
			errors << new sad::resource::UnregisteredResourceType(m_resources[i]->factoryName());
		}
	}

	if (errors.size() == 0)
	{
		for(size_t i = 0; i < m_resources.size(); i++)
		{
			m_tree->root()->replaceResource(list[i].p1(), list[i].p2());
			m_resources[i] = list[i].p2();
			m_resources[i]->setName(list[i].p1());
			list[i].p2()->setPhysicalFile(this);
		}
	}
	else
	{
		sad::resource::free(list);
	}
	return errors;
}

void sad::resource::PhysicalFile::add(sad::resource::Resource * r)
{
	if (r && std::find(m_resources.begin(), m_resources.end(), r) == m_resources.end())
	{
		m_resources << r;
	}	
}

void sad::resource::PhysicalFile::remove(sad::resource::Resource * r)
{
	m_resources.removeAll(r);
}

void sad::resource::PhysicalFile::setTree(sad::resource::Tree * tree)
{
	m_tree = tree;
}

sad::resource::Tree * sad::resource::PhysicalFile::tree() const
{
	return m_tree;
}

const sad::Vector<sad::resource::Resource*> & sad::resource::PhysicalFile::resources() const
{
	return m_resources;	
}

void sad::resource::PhysicalFile::replace(
	sad::resource::Resource * from, 
	sad::resource::Resource * to
)
{
	for(size_t i = 0; i < m_resources.size(); i++)
	{
		if (m_resources[i] == from)
		{
			m_resources[i] = to;
		}
	}
}
