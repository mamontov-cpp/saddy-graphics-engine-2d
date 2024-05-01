#include "db/dbstoredpropertyfactory.h"

#include "db/save.h"
#include "db/load.h"

#include "sadcolor.h"
#include "sadrect.h"
#include "sadpoint.h"
#include "sadsize.h"
#include "opticksupport.h"

sad::db::StoredPropertyFactory::AbstractDelegate::~AbstractDelegate()
{
    PROFILER_EVENT;
    
}

sad::db::StoredPropertyFactory::StoredPropertyFactory()
{
    PROFILER_EVENT;
    add<bool>("bool");
    add<char>("char");
    add<unsigned char>("unsigned char");
    add<signed char>("signed char");
    add<short>("short");
    add<unsigned short>("unsigned short");
    add<int>("int");
    add<unsigned int>("unsigned int");
    add<long>("long");
    add<unsigned long>("unsigned long");
    add<long long>("long long");
    add<unsigned long long>("unsigned long long");
    add<float>("float");
    add<double>("double");
    add<sad::String>("sad::String");
    add<std::string>("std::string");
    add<sad::Color>("sad::Color");
    add<sad::AColor>("sad::AColor");
    add<sad::Point2D>("sad::Point2D");
    add<sad::Point2I>("sad::Point2I");
    add<sad::Point3D>("sad::Point3D");
    add<sad::Point3I>("sad::Point3I");
    add<sad::Rect2D>("sad::Rect2D");
    add<sad::Rect2I>("sad::Rect2I");
    add<sad::Size2D>("sad::Size2D");
    add<sad::Size2I>("sad::Size2I");
    add<sad::Vector<sad::Vector<sad::AColor> > >("sad::Vector<sad::Vector<sad::AColor> >");
}

sad::db::StoredPropertyFactory::~StoredPropertyFactory()
{
    PROFILER_EVENT;
    
}

void sad::db::StoredPropertyFactory::add(
    const sad::String & name, 
    sad::db::StoredPropertyFactory::AbstractDelegate * d
)
{
    PROFILER_EVENT;
    if (m_delegates.contains(name))
    {
        delete m_delegates[name]; //-V515
        m_delegates[name] = d;
    }
    else
    {
        m_delegates.insert(name, d);			
    }
}

 sad::db::Property *  sad::db::StoredPropertyFactory::create(const sad::String & name) const
{
    PROFILER_EVENT;
    sad::db::Property* p = nullptr;
    if (m_delegates.contains(name))
    {
        p = m_delegates[name]->create();
    }
    return p;
}


bool sad::db::StoredPropertyFactory::canCreate(const sad::String & name) const
{
    PROFILER_EVENT;
    return m_delegates.contains(name);
}

sad::db::StoredPropertyFactory* sad::db::StoredPropertyFactory::clone() const
{
    PROFILER_EVENT;
    sad::db::StoredPropertyFactory* result = new sad::db::StoredPropertyFactory();
    for(sad::PtrHash<sad::String, sad::db::StoredPropertyFactory::AbstractDelegate>::const_iterator it 
        = m_delegates.const_begin();
        it != m_delegates.const_end();
        ++it)
    {
        if (result->m_delegates.contains(it.key()) == false)
        {
            result->m_delegates.insert(it.key(), it.value()->clone());
        }
    }
    return result;
}
