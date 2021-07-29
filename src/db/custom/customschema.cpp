#include "db/custom/customschema.h"

#include "util/fs.h"

#include "renderer.h"

#include <fstream>
#include <cassert>

DECLARE_SOBJ_INHERITANCE(sad::db::custom::Schema, sad::resource::Resource);

sad::db::custom::Schema::Schema()
{
    // TODO: Add new properties, same as Sprite2D
    m_factory = new sad::db::StoredPropertyFactory();
}

sad::db::custom::Schema::~Schema()
{
    delete m_factory;
}

void sad::db::custom::Schema::setFactory(sad::db::StoredPropertyFactory * factory)
{
    m_factory_lock.lock();

    assert(factory);
    delete m_factory;
    m_factory = factory->clone();

    m_factory_lock.unlock();
}

sad::db::StoredPropertyFactory* sad::db::custom::Schema::factory() const
{
    return m_factory;   
}

void sad::db::custom::Schema::setTreeItemName(const sad::String & item)
{
    m_tree_item = item;
}

const sad::String& sad::db::custom::Schema::treeItemName() const
{
    return m_tree_item; 
}

bool sad::db::custom::Schema::load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
)
{
    std::ifstream stream(file.name().c_str());
    if (stream.bad())
    {
        sad::String newpath = util::concatPaths(r->executablePath(), file.name());
        stream.open(newpath.c_str());
    }   
    bool result = false;
    if (stream.good())
    {
        picojson::value v;
        stream >> v;        
        if (picojson::get_last_error().size() == 0)
        {
            result  = this->load(v);
        }
    }
    return result;
}

// A length of non-custom properties, taken from Sprite2D
static const size_t Sprite2DPropertiesLength = 4;
// An own properties for Sprite 2D
static const char* Sprite2DOwnProperties[Sprite2DPropertiesLength] = {
    "pos",
    "size",
    "angle",
    "rect"    
};

namespace sad
{

namespace db 
{

namespace custom
{

/*! Tests, whether properties are not inherited
    \param[in] prop property
    \return whether property is not inherited
 */
bool is_not_inherited(const sad::String & prop)
{
    for(size_t i = 0; i <  Sprite2DPropertiesLength; i++)
    {
        if (Sprite2DOwnProperties[i] == prop)
            return false;
    }
    return true;
}

}

}

}

bool sad::db::custom::Schema::load(const picojson::value& v)
{
    m_properties_lock.lock();
    bool result = false;
    if (v.is<picojson::object>())
    {
        sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
                picojson::get_property(v, "resource")
        );
        picojson::value const * maybeschema = picojson::get_property(v, "schema");
        if (maybename.exists() && maybeschema)
        {
            if (maybeschema->is<picojson::object>())
            {
                // Key in schema is a property name, and value is a type
                sad::Hash<sad::String, sad::db::Property*> props;
                bool loadresult = true;
                picojson::object o = maybeschema->get<picojson::object>();
                for(picojson::object::iterator it = o.begin(); it != o.end(); ++it)
                {
                    bool entryloadresult = false;
                    const picojson::value * maybetypeentry = picojson::get_property(it->second, "type");
                    const picojson::value * maybevalueentry = picojson::get_property(it->second, "value");
                    const picojson::value * maybedefaultentry = picojson::get_property(it->second, "default");
                    if (maybetypeentry && maybevalueentry && sad::db::custom::is_not_inherited(it->first))
                    {
                        sad::Maybe<sad::String> maybeproptype = picojson::to_type<sad::String>(*maybetypeentry);
                        if (maybeproptype.exists())
                        {
                            sad::db::Property * p = m_factory->create(maybeproptype.value());
                            if (p)
                            {
                                sad::db::Variant value;
                                p->get(nullptr, value);
                                if (value.load(*maybevalueentry)) 
                                {
                                    if (p->set(nullptr, value))
                                    {
                                        bool caninsert = true;
                                        if (maybedefaultentry)
                                        {
                                            caninsert = false;
                                            sad::db::Variant* default_value = new sad::db::Variant();
                                            p->get(nullptr, *default_value);
                                            if (default_value->load(*maybedefaultentry)) 
                                            {
                                                if (p->couldBeSetFrom(*default_value))
                                                {
                                                    caninsert = true;
                                                    p->makeNonRequiredWithDefaultValue(default_value);
                                                }
                                            }
                                            if (!caninsert)
                                            {
                                                delete default_value;
                                            }
                                        }
                                        if (caninsert)
                                        {
                                            props.insert(it->first, p);
                                            entryloadresult = true;
                                        }                                       
                                    }                                   
                                } 
                                if (!entryloadresult)
                                {
                                    delete p;
                                }
                            }                       
                        }   
                    }

                    loadresult = loadresult && entryloadresult;
                }
                if (loadresult == false)
                {
                    for(sad::Hash<sad::String, sad::db::Property*>::iterator it = props.begin();
                        it != props.end();
                        ++it)
                    {
                        delete it.value();
                    }
                }
                else
                {
                    for(sad::Hash<sad::String, sad::db::Property*>::iterator it = props.begin();
                        it != props.end();
                        ++it)
                    {
                        m_properties.insert(it.key(), it.value());
                    }
                }
                result = loadresult;
            }
        }
    }
    m_properties_lock.unlock();
    return result;
}




void sad::db::custom::Schema::getCustomProperties(sad::Hash<sad::String, sad::db::Property*>& props)
{
    m_properties_lock.lock();
    for (sad::PtrHash<sad::String, sad::db::Property>::const_iterator it = m_properties.const_begin(); 
         it != m_properties.const_end(); 
        ++it)
    {
        bool isown = true;
        for(size_t i = 0; (i < Sprite2DPropertiesLength) && isown; i++)
        {
            if (it.key() == Sprite2DOwnProperties[i])
            {
                isown = false;
            }
        }
        if (isown)
        {
            sad::db::Property* propclone = it.value()->clone();
            props.insert(it.key(), propclone);
        }
    }
    m_properties_lock.unlock();
}


void sad::db::custom::Schema::getNamesOfCustomProperties(sad::Vector<sad::String>& names)
{
    m_properties_lock.lock();
    for (sad::PtrHash<sad::String, sad::db::Property>::const_iterator it = m_properties.const_begin();
         it != m_properties.const_end();
        ++it)
    {
        bool isown = true;
        for(size_t i = 0; (i < Sprite2DPropertiesLength) && isown; i++)
        {
            if (it.key() == Sprite2DOwnProperties[i])
            {
                isown = false;
            }
        }
        if (isown)
        {
            names << it.key();
        }
    }
    m_properties_lock.unlock();
}

sad::db::custom::Schema::Schema(const sad::db::custom::Schema& s) : m_factory(nullptr)
{
    throw std::logic_error("Not implemented!");
}

sad::db::custom::Schema& sad::db::custom::Schema::operator=(const sad::db::custom::Schema& s)
{
    throw std::logic_error("Not implemented!");
    return *this;
}
