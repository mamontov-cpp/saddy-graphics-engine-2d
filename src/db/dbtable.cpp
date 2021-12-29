#include "db/dbtable.h"
#include "db/dbdatabase.h"
#include "db/dbobjectfactory.h"
#include "db/dbtypename.h"

#include "renderer.h"


sad::db::Table::Table() : m_max_minor_id(1), m_database(nullptr)
{
    
}

sad::db::Table::~Table() 
{
    for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
    {
        it.value()->delRef();
    }
}

#ifdef LOG_TABLE_ADD
    #define LOG_TABLE_ADD_PRINTF(X) printf(X)
#else
    #define LOG_TABLE_ADD_PRINTF(X)
#endif

void sad::db::Table::add(sad::db::Object* a)
{
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::1\n");
    assert(a);
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2\n");
    if (a->MajorId > 0)
    {
        sad::db::Object * old;
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::21\n");
        if (a->MinorId > 0)
        {
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::211\n");
            old = queryById(a->MajorId, a->MinorId);
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::212\n");
            if (old)
            {
                LOG_TABLE_ADD_PRINTF("sad::db::Table::add::213\n");
                remove(old);
            }
        }
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::22\n");
        if (database())
        {
            old = database()->queryByMajorId(a->MajorId);
            if (old)
            {
                old->table()->remove(old);
            }
        }
        else 
        {
            old = queryByMajorId(a->MajorId);
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::23\n");
            if (old)
            {
                LOG_TABLE_ADD_PRINTF("sad::db::Table::add::24\n");
                remove(old);
            }
        }
    }
    // Create major id if needed
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::25\n");
    if (a->MajorId == 0)
    {
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::251\n");
        unsigned long long majorid = 1;
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::252\n");
        if (database())
        {
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2523\n");
            majorid = database()->uniqueMajorId(this);
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2524\n");
        }
        a->MajorId = majorid;
    }
    else
    {
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2525\n");
        if (database())
        {
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::25251\n");
            database()->trySetMaxMajorId(a->MajorId, this);
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::25252\n");
        }
    }
    
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::26\n");
    if (a->MinorId == 0)
    {
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::261\n");
        a->MinorId = this->m_max_minor_id++;
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::262\n");
    }
    else
    {
        if (a->MinorId >= m_max_minor_id)
        {
            m_max_minor_id = a->MinorId + 1;
        }
    }
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::27\n");
    a->addRef();
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::28\n");

    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::29\n");
    if (a->objectName().size() != 0)
    {
        LOG_TABLE_ADD_PRINTF("sad::db::Table::add::291\n");
        if (m_object_by_name.contains(a->objectName()) == false)
        {
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2911\n");
            m_object_by_name.insert(a->objectName(), sad::Vector<sad::db::Object*>());
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2912\n");
            m_object_by_name[a->objectName()].push_back(a);
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2913\n");
        }
        else
        {
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2914\n");
            sad::Vector<sad::db::Object*> & list = m_object_by_name[a->objectName()];
            LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2915\n");
            if (std::find(list.begin(), list.end(), a) == list.end())
            {
                LOG_TABLE_ADD_PRINTF("sad::db::Table::add::29151\n");
                list.push_back(a);
                LOG_TABLE_ADD_PRINTF("sad::db::Table::add::29152\n");
            }
        }
    }
    
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2A\n");
    m_objects_by_minorid.insert(a->MinorId, a);
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2B\n");
    m_objects_by_majorid.insert(a->MajorId, a);	
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2C\n");
    a->setTable(this);
    LOG_TABLE_ADD_PRINTF("sad::db::Table::add::2D\n");
}

void sad::db::Table::remove(sad::db::Object* a)
{
    if (a)
    {
        if (a->objectName().size() != 0)
        {
            if (m_object_by_name.contains(a->objectName()))
            {
                sad::Vector<sad::db::Object*> & list = m_object_by_name[a->objectName()];
                sad::Vector<sad::db::Object*>::iterator pos =  std::find(list.begin(), list.end(), a);
                list.erase(pos);
            }
        }

        if (a->MajorId > 0 && m_objects_by_majorid.contains(a->MajorId))
        {
            m_objects_by_majorid.remove(a->MajorId);
            if (database())
            {
                this->database()->removeMajorId(a->MajorId);
            }
        }

        if (a->MinorId > 0 && m_objects_by_minorid.contains(a->MinorId))
        {
            m_objects_by_minorid.remove(a->MinorId);
        }
        
        a->setTable(nullptr);
        a->delRef();		
    }
}

sad::db::Object* sad::db::Table::queryById(unsigned long long major_id, unsigned long long minor_id)
{
    sad::db::Object* result = nullptr;
    if (m_objects_by_majorid.contains(major_id) && m_objects_by_minorid.contains(minor_id))
    {
        sad::db::Object*  f1 = m_objects_by_majorid[major_id];
        sad::db::Object * f2 = m_objects_by_minorid[minor_id];
        if (f1 == f2)
        {
            result = f1;
        }
    }
    return result;	
}

sad::db::Object* sad::db::Table::queryByMinorId(unsigned long long minor_id)
{
    sad::db::Object* result = nullptr;
    if (m_objects_by_minorid.contains(minor_id))
    {
        result = m_objects_by_minorid[minor_id];
    }
    return result;
}

sad::Vector<sad::db::Object*> sad::db::Table::queryByName(const sad::String& name)
{
    sad::Vector<sad::db::Object*> result;
    if (m_object_by_name.contains(name))
    {
        result = m_object_by_name[name];
    }
    return result;	
}

sad::db::Object* sad::db::Table::objectByName(const sad::String& name)
{
    sad::Vector<sad::db::Object*> objects = this->queryByName(name);
    if (objects.size())
    {
        return objects[0];
    }
    return nullptr;
}

sad::db::Object* sad::db::Table::queryByMajorId(unsigned long long major_id)
{
    sad::db::Object* result = nullptr;
    if (m_objects_by_majorid.contains(major_id))
    {
        result = m_objects_by_majorid[major_id];
    }
    return result;	
}

bool sad::db::Table::load(
    const picojson::value & v, 
    sad::db::ObjectFactory* factory,
    sad::Renderer* renderer,
    const sad::String& tree_name
)
{
    if (renderer == nullptr)
    {
        renderer = sad::Renderer::ref();
    }
    bool result = false;
    if (v.is<picojson::array>())
    {
        picojson::array entries = v.get<picojson::array>();
        sad::Vector<sad::db::Object*> buffer;
        bool ok = true;
        // Load items to buffer
        for(size_t i = 0; i < entries.size() && ok; i++)
        {
            sad::db::Object * tmp = factory->createFromEntry(entries[i]);
            if (!tmp)
            {
                ok = false;
            }
            else
            {
                tmp->setTreeName(renderer, tree_name);
                ok = tmp->load(entries[i]);
                if (!ok)
                {
                    delete  tmp;
                }
            }
            if (ok)
            {
                buffer << tmp;
            }
        }
        // Insert buffer to table, otherwise delete buffer
        if (ok)
        {
            for(size_t i = 0; i < buffer.size(); i++)
            {
                add(buffer[i]);
            }
        }
        else
        {
            for(size_t i = 0; i < buffer.size(); i++)
            {
                delete buffer[i];
            }
        }
        result = ok;
    }
    return result;
}

void sad::db::Table::save(picojson::value & v)
{
    if (v.is<picojson::array>() == false)
    {
        v = picojson::value(picojson::array_type, false);
    }
    sad::Vector<sad::db::Object*> result;
    this->objects(result);
    for(size_t i = 0; i < result.size(); i++)
    {
        if (result[i]->Active)
        {
            picojson::value tmp(picojson::object_type, false);
            result[i]->save(tmp);
            v.push_back(tmp);
        }
    }
}


sad::db::Database* sad::db::Table::database() const
{
    return m_database;
}

void sad::db::Table::setDatabase(sad::db::Database* d)
{
    m_database = d;
}


void sad::db::Table::objects(sad::Vector<sad::db::Object*> & o)
{
    for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
    {
        o << it.value();
    }
}

sad::Vector<sad::db::Object*> sad::db::Table::objectList()
{
    sad::Vector<sad::db::Object*> result;
    for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
    {
        result << it.value();
    }
    return result;
}

sad::Vector<sad::db::Object*>  sad::db::Table::objectListOfType(const sad::String& s)
{
    sad::Vector<sad::db::Object*> result;
    for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
    {
        if (it.value()->isInstanceOf(s)) 
        {
            result << it.value();
        }
    }
    return result;	
}

void sad::db::Table::changeObjectName(
        sad::db::Object * o, 
        const sad::String & old_name,
        const sad::String & name
    )
{
    // Check, whether we own object
    if (o->table() != this)
    {
        return;
    }

    if (old_name.length())
    {
        if (m_object_by_name.contains(old_name))
        {
            sad::Vector<sad::db::Object*>& objects = m_object_by_name[old_name];
            objects.removeAll(o);
        }
    }

    if (name.length())
    {
        if (m_object_by_name.contains(name) == false)
        {
            m_object_by_name.insert(name, sad::Vector<sad::db::Object*>());
        }
        sad::Vector<sad::db::Object*>& objects =m_object_by_name[name];
        if (std::find(objects.begin(), objects.end(), o) == objects.end())
        {
            objects << o;
        }
    }
}

void sad::db::Table::clear()
{
    for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_objects_by_minorid.clear();
    m_objects_by_majorid.clear();
    m_object_by_name.clear();
}

bool sad::db::Table::empty() const
{
    return m_objects_by_majorid.empty();
}

DECLARE_COMMON_TYPE(sad::db::Table);
