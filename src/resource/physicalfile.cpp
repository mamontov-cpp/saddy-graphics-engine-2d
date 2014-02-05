#include "resource/physicalfile.h"

#include <algorithm>

resource::PhysicalFile::PhysicalFile(const sad::String & name) 
: m_name(name), m_tree(NULL)
{
			
}


resource::PhysicalFile::~PhysicalFile()
{

}


bool resource::PhysicalFile::isAnonymous() const
{
	return m_name.length() != 0;	
}

const sad::String & resource::PhysicalFile::name() const
{
	return m_name;	
}

void resource::PhysicalFile::setName(const sad::String & name)
{
	m_name = name;
}

sad::Vector<resource::Error*> resource::PhysicalFile::reload()
{
	// TODO: Actually reload file
	return sad::Vector<resource::Error*>();
}

void resource::PhysicalFile::add(resource::Resource * r)
{
	if (r && std::find(m_resources.begin(), m_resources.end(), r) == m_resources.end())
	{
		m_resources << r;
	}	
}

void resource::PhysicalFile::remove(resource::Resource * r)
{
	m_resources.removeAll(r);
}

void resource::PhysicalFile::setTree(resource::Tree * tree)
{
	m_tree = tree;
}

resource::Tree * resource::PhysicalFile::tree() const
{
	return m_tree;
}
