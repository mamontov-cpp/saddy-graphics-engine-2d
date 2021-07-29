#include "classmetadatacontainer.h"
#include "sadmutex.h"

#include "db/schema/schema.h"

#include <cstdlib>
#include <stdexcept>


sad::ClassMetaDataContainer::ClassMetaDataContainer()
{

}

sad::ClassMetaDataContainer::ClassMetaDataContainer(const ClassMetaDataContainer & c)
{
    throw std::runtime_error("Not implemented!");
}

sad::ClassMetaDataContainer& sad::ClassMetaDataContainer::operator=(const sad::ClassMetaDataContainer& c)
{
    throw std::runtime_error("Not implemented!");
    return *this;
}

sad::ClassMetaDataContainer * sad::ClassMetaDataContainer::m_instance = nullptr;



static sad::Mutex creationlock;

void sad::ClassMetaDataContainer::destroyInstance()
{
    creationlock.lock();
    delete sad::ClassMetaDataContainer::m_instance;
    sad::ClassMetaDataContainer::m_instance = nullptr;
    creationlock.unlock();
}

sad::ClassMetaDataContainer * sad::ClassMetaDataContainer::ref()
{
    creationlock.lock();
    if (sad::ClassMetaDataContainer::m_instance == nullptr)
    {
        sad::ClassMetaDataContainer::m_instance = new sad::ClassMetaDataContainer();
        atexit(sad::ClassMetaDataContainer::destroyInstance);
    }
    creationlock.unlock();

    return sad::ClassMetaDataContainer::m_instance;
}

sad::ClassMetaDataContainer::~ClassMetaDataContainer()
{
    for (ClassMetaDataHash::iterator it = m_container.begin() ; it != m_container.end(); it++)
    {
        delete it.value();
    }
    for(size_t i = 0; i < m_global_schemas.size(); ++i)
    {
        delete m_global_schemas[i];
    }
}


sad::ClassMetaData * sad::ClassMetaDataContainer::get(const sad::String & name, bool & created, bool lock)
{
    sad::ClassMetaData * result = nullptr;
    if (lock)
    {
        m_lock.lock();
    }
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
    if (lock)
    {
        m_lock.unlock();
    }
    return result;
}

bool sad::ClassMetaDataContainer::contains(const sad::String & name) const
{
    return m_container.contains(name);
}

void sad::ClassMetaDataContainer::pushGlobalSchema(sad::db::schema::Schema* s)
{
    m_schemas_lock.lock();
    m_global_schemas << s;
    m_schemas_lock.unlock();
}
