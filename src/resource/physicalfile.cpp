#include "resource/physicalfile.h"

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

sad::Vector<sad::resource::Error*> sad::resource::PhysicalFile::reload()
{
	// TODO: Actually reload file
	return sad::Vector<sad::resource::Error*>();
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
