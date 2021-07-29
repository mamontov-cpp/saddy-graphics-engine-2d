#include "db/dbdatabase.h"
#include "db/dbtypename.h"

#include "util/fs.h"

#include "renderer.h"

#include <fstream>

// ===================================  PUBLIC METHODS ===================================

sad::db::Database::Database() : m_max_major_id(1), m_renderer(nullptr)
{
    m_factory = new sad::db::ObjectFactory();
    m_prop_factory = new sad::db::StoredPropertyFactory();
}

sad::db::Database::~Database()
{
    delete m_factory;
    delete m_prop_factory;
    for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
        it != m_names_to_tables.end();
        ++it
       )
    {
        it.value()->clear();
    }
    for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
        it != m_names_to_tables.end();
        ++it
       )
    {
        delete it.value();
    }
}

void sad::db::Database::save(sad::String & output)
{
    picojson::value resultvalue(picojson::object_type, false);

    picojson::object propertiesresult;
    saveProperties(propertiesresult);
    resultvalue.insert("properties", propertiesresult);

    picojson::value tablesresult(picojson::object_type, false);
    for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
        it!= m_names_to_tables.end();
        ++it
       )
    {
        picojson::value tablevalue(picojson::array_type, false);
        it.value()->save(tablevalue);
        tablesresult.insert(it.key(), tablevalue);
    }
    resultvalue.insert("tables", tablesresult);

    output = resultvalue.serialize(0);
}

sad::String sad::db::Database::save()
{
    sad::String result;
    this->save(result);
    return result;
}

void sad::db::Database::saveToFile(const sad::String& s)
{
    std::ofstream file(s.c_str(), std::ofstream::out);
    if (file.good())
    {
        sad::String output;
        this->save(output);
        file << output;
        file.close();
    }
}

bool sad::db::Database::load(const sad::String& text)
{
    bool result = false;
    if (text.consistsOfWhitespaceCharacters() == false)
    {
        picojson::value root = picojson::parse_string(text);
        if (root.is<picojson::object>())
        {
            picojson::value const* propertiesroot = picojson::get_property(root, "properties");
            picojson::value const* tablesroot = picojson::get_property(root, "tables");
            if (propertiesroot && tablesroot)
            {
                if (propertiesroot->is<picojson::object>() && tablesroot->is<picojson::object>())
                {
                    result = this->loadPropertiesAndTables(
                        propertiesroot->get<picojson::object>(),
                        tablesroot->get<picojson::object>()
                    );
                    if (result)
                    {
                        this->saveSnapshot();
                    }
                }
            }
        }
    }
    return result;
}

bool sad::db::Database::tryLoadFrom(const sad::String& name)
{
    return this->loadFromFile(name, sad::Renderer::ref());
}

bool sad::db::Database::loadFromFile(const sad::String& name, sad::Renderer * r)
{
    bool loadingresult = false;
    sad::Maybe<sad::String> result;
    std::ifstream stream(name.c_str());
    if (stream.good())
    {
        std::string alldata(
            (std::istreambuf_iterator<char>(stream)), 
            std::istreambuf_iterator<char>()
        );
        result.setValue(alldata);
    }
    else
    {
        if (util::isAbsolutePath(name) == false)
        {
            if (r == nullptr)
            {
                r = this->renderer();
            }
            sad::String path = util::concatPaths(r->executablePath(), name);
            stream.clear();
            stream.open(path.c_str());
            if (stream.good())
            {
                std::string alldata(
                    (std::istreambuf_iterator<char>(stream)), 
                     std::istreambuf_iterator<char>()
                    );
                result.setValue(alldata);
            }
        }
    }

    if (result.exists())
    {
        loadingresult = this->load(result.value());
    }

    return loadingresult;
}

void sad::db::Database::addPropertyOfType(const sad::String & name,  const sad::String& type)
{
    if (m_prop_factory->canCreate(type))
    {
        addProperty(name, m_prop_factory->create(type));
    }
}

void sad::db::Database::addProperty(const sad::String & name, sad::db::Property * p)
{
    assert( p );

    if (m_properties.contains(name))
    {
        delete m_properties[name]; //-V515
        m_properties[name] = p;
    }
    else
    {
        m_properties.insert(name, p);
    }
}

void sad::db::Database::removeProperty(const sad::String & name)
{
    if (m_properties.contains(name))
    {
        delete m_properties[name]; //-V515
        m_properties.remove(name);
    }
}

sad::db::Property* sad::db::Database::propertyByName(const sad::String & name) const
{
    sad::db::Property* p = nullptr;
    if (m_properties.contains(name))
    {
        p = m_properties[name];
    }
    return p;
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool sad::db::Database::setDBProperty(const sad::String& name, sad::db::Variant& v)
{
    sad::db::Property * prop = this->propertyByName(name);
    bool result = false;
    if (prop)
    {
        result = prop->set(nullptr, v);
    }
    return result;
}

bool sad::db::Database::hasDBProperty(const sad::String& name) const
{
    return this->propertyByName(name) != nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
sad::Maybe<sad::db::Variant> sad::db::Database::getDBProperty(const sad::String& name)
{
    sad::db::Property * prop = this->propertyByName(name);
    if (prop)
    {
        sad::db::Variant v;
        prop->get(nullptr, v);
        return sad::Maybe<sad::db::Variant>(v);
    }
    return sad::Maybe<sad::db::Variant>();
}

sad::db::StoredPropertyFactory* sad::db::Database::storedPropertyFactory() const
{
    return m_prop_factory;
}

void sad::db::Database::setStoredPropertyFactory(sad::db::StoredPropertyFactory * f)
{
    assert( f );
    delete m_prop_factory;
    m_prop_factory = f;
}

bool sad::db::Database::addTable(const sad::String& name, db::Table* table)
{
    assert( table );

    bool result = false;
    if (m_names_to_tables.contains(name) == false)
    {
        result = true;
        table->addRef();
        table->setDatabase(this);
        m_names_to_tables.insert(name, table);
    }
    return result;		
}

void sad::db::Database::removeTable(const sad::String& name)
{
    if (m_names_to_tables.contains(name))
    {
        // Remove major ids, since we need to cleanup them
        sad::Vector<sad::db::Object*> objects;
        m_names_to_tables[name]->objects(objects);
        for(size_t  i = 0; i < objects.size(); i++)
        {
            this->removeMajorId(objects[i]->MajorId);
        }

        m_names_to_tables[name]->delRef();
        m_names_to_tables.remove(name);
    }
}

sad::db::Table* sad::db::Database::table(const sad::String& name)
{
    sad::db::Table* result = nullptr;	
    if (m_names_to_tables.contains(name))
    {
        result = m_names_to_tables[name];
    }
    return result;
}

unsigned long long sad::db::Database::uniqueMajorId(sad::db::Table * t)
{
    m_majorid_to_table.insert(m_max_major_id, t);
    return m_max_major_id++;	
}

sad::Vector<sad::db::Object *> sad::db::Database::queryByName(const sad::String & name) const
{
    sad::Vector<sad::db::Object *> result;
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        result << it.value()->queryByName(name);
    }
    return result;
}

sad::db::Object* sad::db::Database::objectByName(const sad::String & name) const
{
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        sad::Vector<sad::db::Object*> o =  it.value()->queryByName(name);
        if (o.size())
        {
            return o[0];
        }
    }
    return nullptr;	
}

sad::Vector<sad::db::Object *> sad::db::Database::queryByMinorId(unsigned long long id) const
{
    sad::Vector<sad::db::Object *> result;
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        sad::db::Object * tmp = it.value()->queryByMinorId(id);
        if (tmp)
        {
            result << tmp;
        }
    }
    return result;
}

sad::db::Object* sad::db::Database::queryByMajorId(unsigned long long id) const
{
    sad::db::Object* result = nullptr;
    if (m_majorid_to_table.contains(id))
    {
        result = m_majorid_to_table[id]->queryByMajorId(id);
    }
    return result;
}

void sad::db::Database::getTables(sad::Vector<sad::Pair<sad::String, sad::db::Table*> > & tables) const
{
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        tables << sad::Pair<sad::String, sad::db::Table*>(it.key(), it.value());
    }
}

sad::Vector<sad::Pair<sad::String, sad::db::Table*> > sad::db::Database::tableList() const
{
    sad::Vector<sad::Pair<sad::String, sad::db::Table*> > tables;
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        tables << sad::Pair<sad::String, sad::db::Table*>(it.key(), it.value());
    }
    return tables;
}

sad::Vector<sad::String> sad::db::Database::propertyNames() const
{
    sad::Vector<sad::String> result;
    for(sad::PtrHash<sad::String, sad::db::Property>::const_iterator it = m_properties.const_begin();
        it != m_properties.const_end();
        ++it
       )
    {
        result << it.key();
    }
    return result;
}

sad::db::ObjectFactory* sad::db::Database::factory()
{
    return m_factory;	
}

void sad::db::Database::setFactory(sad::db::ObjectFactory* f)
{
    assert( f );
    delete m_factory;
    m_factory = f;
}

void sad::db::Database::trySetMaxMajorId(unsigned long long v, sad::db::Table * t)
{
    if (v >= m_max_major_id)
    {
        m_max_major_id = v + 1;
    }
    m_majorid_to_table.insert(v, t);
}

void sad::db::Database::removeMajorId(unsigned long long v)
{
    m_majorid_to_table.remove(v);
}

void sad::db::Database::setRenderer(sad::Renderer * r)
{
    m_renderer = r;
}

sad::Renderer* sad::db::Database::renderer() const
{
    if (!m_renderer)
    {
        return sad::Renderer::ref();
    }
    return m_renderer;
}

sad::db::Database::Properties::const_iterator sad::db::Database::begin() const
{
    return m_properties.const_begin();
}


sad::db::Database::Properties::const_iterator sad::db::Database::end() const
{
    return m_properties.const_end();
}

void sad::db::Database::setDefaultTreeName(const sad::String& tree)
{
    m_default_tree_name = tree;
}

const sad::String& sad::db::Database::defaultTreeName() const
{
    return m_default_tree_name;
}

void sad::db::Database::saveSnapshot()
{
    sad::db::Database::Snapshot snapshotstub;
    snapshotstub.MaxId = m_max_major_id;
    m_snapshots << snapshotstub;

    sad::db::Database::Snapshot& snapshot = m_snapshots[m_snapshots.size() - 1];
    saveProperties(snapshot.Properties);

    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        sad::db::Database::TableSnapshot tablesnapshot;
        sad::Vector<sad::db::Object*> objects;
        it.value()->objects(objects);
        for(size_t i = 0; i < objects.size(); i++)
        {
            picojson::value tmp(picojson::object_type, false);
            objects[i]->save(tmp);
            tablesnapshot.insert(objects[i]->MajorId, tmp);
        }
        snapshot.Tables << sad::Pair<sad::String, sad::db::Database::TableSnapshot>(it.key(), tablesnapshot);
    }
}

unsigned long sad::db::Database::snapshotsCount() const
{
    return m_snapshots.size();
}

bool sad::db::Database::restoreSnapshot(unsigned long index)
{
    if (index >= m_snapshots.size())
    {
        return false;
    }

    sad::db::Database::Snapshot& snapshot = m_snapshots[index];
    sad::Hash<sad::String, sad::db::Property*> newproperties;
    assert( loadProperties(snapshot.Properties, newproperties) );
    setPropertiesFrom(newproperties);
    
    // Fill database with snapshot tables
    sad::Hash<sad::String, char> snapshottables;
    for(size_t i = 0; i <  snapshot.Tables.size(); i++)
    {
        snapshottables.insert(snapshot.Tables[i].p1(), 1);
    }

    // Remove tables, that are absent in table
    sad::Vector<sad::Pair<sad::String, sad::db::Table*> > tables;
    this->getTables(tables);
    for(size_t i = 0; i < tables.size(); i++)
    {
        if (snapshottables.contains(tables[i].p1()) == false) 
        {
            removeTable(tables[i].p1());
        }
    }
    
    // Add absent tables
    for(size_t i = 0; i < snapshot.Tables.size(); i++)
    {
        if (this->table(snapshot.Tables[i].p1()) == nullptr)
        {
            addTable(snapshot.Tables[i].p1(), new sad::db::Table());                
        }
    }


    // Reset all tables
    for(size_t i = 0; i < snapshot.Tables.size(); i++)
    {
        sad::db::Table* table = this->table(snapshot.Tables[i].p1());
        sad::Vector<sad::db::Object*> objects;
        table->objects(objects);
        sad::db::Database::TableSnapshot& tablesnapshot = snapshot.Tables[i]._2();
        // Remove object, absent in snapshot
        for(size_t j = 0; j < objects.size(); j++)
        {
            if (tablesnapshot.contains(objects[j]->MajorId) == false)
            {
                table->remove(objects[j]);
            }
        }
        for(sad::db::Database::TableSnapshot::iterator it = tablesnapshot.begin();
            it != tablesnapshot.end();
            ++it)
        {
            sad::db::Object* o = table->queryByMajorId(it.key());
            if (o)
            {
                 o->reset();
            }
        }
        for(sad::db::Database::TableSnapshot::iterator it = tablesnapshot.begin();
            it != tablesnapshot.end();
            ++it)
        {
            sad::db::Object* o = table->queryByMajorId(it.key());
            bool loaded = false;
            if (o)
            {
                loaded = o->load(it.value());
                if (!loaded)
                {
                    table->remove(o);
                }
            }
            // If failed to load, try recreate object
            if (!loaded)
            {
                o = factory()->createFromEntry(it.value());
                if (o)
                {
                    o->setTreeName(renderer(), defaultTreeName());
                    loaded = o->load(it.value());
                    if (loaded)
                    {
                        table->add(o);
                    }
                    else
                    {
                        delete o;
                    }
                }
            }
            assert(loaded && "Loaded of snapshot failed! Please check object properies");
        }
    }

    m_max_major_id = snapshot.MaxId;
    return true;
}


bool sad::db::Database::tablesAreEmpty() const
{
    bool result = true;
    for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
        it != m_names_to_tables.const_end();
        ++it)
    {
        result = result && it.value()->empty();
    }
    return result;
}
// ===================================  PROTECTED METHODS ===================================


void sad::db::Database::clearProperties()
{
    for(sad::Hash<sad::String, sad::db::Property*>::iterator it = m_properties.begin();
            it != m_properties.end();
            ++it)
    {
        delete it.value();
    }
    m_properties.clear();
}

 bool sad::db::Database::loadProperties(
        const picojson::object& properties, 
        sad::Hash<sad::String, sad::db::Property*>& new_properties
)
{
    bool result = true;
    for(picojson::object::const_iterator it = properties.begin();
        it != properties.end();
        ++it)
    {
        bool deserialized =  false;
        if (it->second.is<picojson::object>())
        {
            picojson::object o = it->second.get<picojson::object>();
            const picojson::value* maybetype = nullptr;
            if (o.find("type") != o.end())
            {
                maybetype = &(o["type"]);
            }			
            const picojson::value* maybevalue = nullptr;
            if (o.find("value") != o.end())
            {
                maybevalue = &(o["value"]);
            }

            if (maybetype && maybevalue)
            {
                if (maybetype->is<std::string>())
                {
                    sad::db::Property * p = m_prop_factory->create(maybetype->get<std::string>()); //-V601
                    if (p)
                    {
                        sad::db::Variant value;
                        p->get(nullptr, value);
                        deserialized = value.load(*maybevalue);
                        if (deserialized)
                        {
                            deserialized = p->set(nullptr, value);
                            if (deserialized)
                            {
                                new_properties.insert(it->first, p);
                            }
                        }
                        if (deserialized == false)
                        {
                            delete p;
                        }
                    }
                }
            }
        }
        
        result = result && deserialized;
    }
    return result;
}

void sad::db::Database::setPropertiesFrom(const sad::Hash<sad::String, sad::db::Property*>& new_properties)
{
    clearProperties();
    for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = new_properties.const_begin();
        it != new_properties.const_end();
        ++it)
    {
        m_properties.insert(it.key(), it.value());
    }

}

bool sad::db::Database::loadPropertiesAndTables(
    const picojson::object & properties, 
    const picojson::object & tables
)
{
    sad::Hash<unsigned long long, sad::db::Table*> oldmajoridtotable = m_majorid_to_table;
    unsigned long long oldmaxmajorid = m_max_major_id;
    sad::Hash<sad::String, sad::db::Property*> newproperties;
    bool result = loadProperties(properties, newproperties);

    sad::Hash<sad::String, sad::db::Table*> newtables;
    for(picojson::object::const_iterator it = tables.begin();
        it != tables.end();
        ++it)
    {
        sad::db::Table* t = new sad::db::Table();
        t->setDatabase(this);
        bool deserialized = t->load(it->second, m_factory, this->renderer(), this->defaultTreeName());
        if (deserialized)
        {
            newtables.insert(it->first, t);
        }
        result = result && deserialized;
    }

    if (result)
    {
        // Remove old keys
        for(sad::Hash<unsigned long long, sad::db::Table*>::iterator it = oldmajoridtotable.begin();
            it != oldmajoridtotable.end();
            ++it)
        {
            m_majorid_to_table.remove(it.key());
        }
        
        // Reset old properties
        setPropertiesFrom(newproperties);

        // Reset old tables
        for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
            it != m_names_to_tables.end();
            ++it)
        {
            delete it.value();
        }
        m_names_to_tables.clear();
        m_names_to_tables = newtables;
    }
    else
    {
        // Restore old values
        m_majorid_to_table = oldmajoridtotable;
        m_max_major_id = oldmaxmajorid;

        // Free allocated resources
        for(sad::Hash<sad::String, sad::db::Property*>::iterator it = newproperties.begin();
            it != newproperties.end();
            ++it)
        {
            delete it.value();
        }

        for(sad::Hash<sad::String, sad::db::Table*>::iterator it = newtables.begin();
            it != newtables.end();
            ++it)
        {
            delete it.value();
        }

    }
    return result;
}

void sad::db::Database::saveProperties(picojson::object& o)
{
    for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
        it != m_properties.end();
        ++it
       )
    {
        picojson::value prop(picojson::object_type, false);
        prop.insert("type", it.value()->serializableType());
        sad::db::Variant tmp;
        it.value()->get(nullptr, tmp);
        prop.insert("value", tmp.save());
        o.insert(std::make_pair(it.key(), prop));
    }
}

DECLARE_COMMON_TYPE(sad::db::Database);
