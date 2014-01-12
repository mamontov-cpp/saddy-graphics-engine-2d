#include "classmetadatacontainer.h"
#include <cstdlib>

sad::ClassMetaDataContainer::ClassMetaDataContainer()
{

}

sad::ClassMetaDataContainer::ClassMetaDataContainer(const ClassMetaDataContainer & c)
{

}

sad::ClassMetaDataContainer * sad::ClassMetaDataContainer::m_instance = NULL;

void sad::ClassMetaDataContainer::destroyInstance()
{
	delete sad::ClassMetaDataContainer::m_instance;
}

sad::ClassMetaDataContainer * sad::ClassMetaDataContainer::ref()
{
	if (sad::ClassMetaDataContainer::m_instance == NULL)
	{
		sad::ClassMetaDataContainer::m_instance = new sad::ClassMetaDataContainer();
		atexit(sad::ClassMetaDataContainer::destroyInstance);
	}
	return sad::ClassMetaDataContainer::m_instance;
}

sad::ClassMetaDataContainer::~ClassMetaDataContainer()
{
	for (ClassMetaDataHash::iterator it = m_container.begin() ; it != m_container.end(); it++)
	{
		delete it.value();
	}
}


sad::ClassMetaData * sad::ClassMetaDataContainer::get(const sad::String & name, bool & created)
{
	sad::ClassMetaData * result = NULL;
	m_lock.lock();
	if (m_container.contains(name))
	{
		result = m_container[name];
		created = false;
	}
	else
	{
		result = new sad::ClassMetaData();
		result->setName(name);
		m_container.insert(name, result);
		created = true;
	}
	m_lock.unlock();
	return result;
}

