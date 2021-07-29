/*! \file db/database.h
    
    
    Contains definition of class Database, which is a database of game objects, which could be organized in tables
    and queried by various parameters, like name and identifying ids.
 */
#pragma once
#include "../sadstring.h"
#include "../refcountable.h"
#include "dbobjectfactory.h"
#include "dberror.h"
#include "dbtable.h"
#include "dbstoredpropertyfactory.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "dbstoredproperty.h"
#include "dbcanbecastedfromto.h"
#include "dbvariant.h"

namespace sad
{

class Renderer;

namespace db
{

/*! \class Database

    A database of game objects, which could be organized in tables
    and queried by various parameters, like name and identifying major and minor ids.
 */
class Database: public sad::RefCountable  
{	
public:	
    /*! A typename for property list
     */
    typedef sad::PtrHash<sad::String, sad::db::Property> Properties;
    /*! Defines a snapshot for a table as mapping from major id of object to
        picojson value
     */
    typedef sad::Hash<unsigned long long, picojson::value> TableSnapshot;
    /*! Defines global snapshot for all tables
     */
    typedef sad::Vector<sad::Pair<sad::String, TableSnapshot> > TablesSnapshot; 
    /*! A snapshot for object properties
     */
    typedef picojson::object PropertiesSnapshot;
    /*! Defines a snapshot for database
     */
    struct Snapshot  // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        TablesSnapshot Tables;         //!< A snapshot for tables
        PropertiesSnapshot Properties; //!< A snapshot for properties
        unsigned long long MaxId;      //!< Maximal id for database
    };
    /*! Constructs default empty database
     */
    Database();
    /*! This class can be inherited 
     */
    virtual ~Database();
    /*! Saves database to string
        \param[out] output output string
     */
    void save(sad::String & output);
    /*! Saves database to string
        \return output string
     */
    sad::String save();
    /*! Saves database to file
        \param[in] filename a name of file
     */
    // ReSharper disable once CppInconsistentNaming
    void saveToFile(const sad::String& filename);  // NOLINT(readability-inconsistent-declaration-parameter-name)
    /*! Loads database from specified string. Saves a snapshot if successfull.
        \param[in] input a text with JSON description of database
        \return whether load was successfull
     */
    bool load(const sad::String& input);  // NOLINT(readability-inconsistent-declaration-parameter-name)
    /*! Loads database from file, using specifying name. Saves a snapshot if successfull.
        \param[in] name a name for file
        \return whether load was successfull
     */
    bool tryLoadFrom(const sad::String& name);
    /*! Loads database from file, using specifying name. Saves a snapshot if successfull.
        \param[in] name a name for file
        \param[in] r renderer, which is used to determine global path's (nullptr for global)
        \return whether load was successfull
     */
    bool loadFromFile(const sad::String& name, sad::Renderer * r = nullptr);
    /*! Adds new custom property to database. Replaces another property, if such property exists.
        \param[in] name name of property
        \param[in] type a property
     */
    virtual void addPropertyOfType(const sad::String & name,  const sad::String& type);
    /*! Adds new custom property to database. Replaces another property, if such property exists.
        \param[in] name name of property
        \param[in] p a property
     */
    virtual void addProperty(const sad::String & name, sad::db::Property * p);
    /*! Removes a custom property from data. A custom property is freed and excluded from memory
        \param[in] name name of property
     */
    virtual void removeProperty(const sad::String & name);
    /*! Returns a property by it's name
        \param[in] name a name of property
        \return value of property
     */
    virtual sad::db::Property* propertyByName(const sad::String & name) const;

    /*! Sets database property
        \param[in] name a name
        \param[in] v variant
     */
    bool setDBProperty(const sad::String& name, sad::db::Variant& v);
    /*! Checks if database has specified property
        \param[in] name a name of property
     */
    bool hasDBProperty(const sad::String& name) const;
    /*! Returns database property
        \param[in] name a name of property
        \return a property
     */
    sad::Maybe<sad::db::Variant> getDBProperty(const sad::String& name);
    /*! Tries to set property to a value 
        \param[in] name a name of property
        \param[in] value a value of property
        \return whether setting property was successfull
     */
    template<
        typename T
    >
    bool setProperty(const sad::String & name, const T & value)
    {
        sad::db::Property * prop = this->propertyByName(name); //-V595
        bool result = false;
        if (prop && this) //-V704
        {
            sad::db::TypeName<T>::init();
            const bool can_be_casted = sad::db::can_be_casted_from_to(
                sad::db::TypeName<T>::baseName(),
                sad::db::TypeName<T>::isSadObject(),
                sad::db::TypeName<T>::POINTER_STARS_COUNT,
                prop->baseType(), 
                prop->typeIsKindOfSadObject(),
                prop->pointerStarsCount()
            );
            if (can_be_casted)
            {
                const sad::db::Variant v(value);
                result = prop->set(nullptr, v);
            }
        }
        return result;
    }
    /*! Tries to fetch property value from a database
        \param[in] s name of property
        \return property value
     */
    template<
        typename T
    >
    sad::Maybe<T> getProperty(const sad::String & s) const
    {
        sad::db::Property * prop = this->propertyByName(s);
        sad::Maybe<T> result;
        if (prop)
        {
            sad::db::TypeName<T>::init();
            const bool can_be_casted = sad::db::can_be_casted_from_to(
                prop->baseType(), 
                prop->typeIsKindOfSadObject(),
                prop->pointerStarsCount(),
                sad::db::TypeName<T>::baseName(),
                sad::db::TypeName<T>::isSadObject(),
                sad::db::TypeName<T>::POINTER_STARS_COUNT
            );
            if (can_be_casted)
            {
                sad::db::Variant v;
                prop->get(nullptr, v);
                result = v.get<T>();
            }
        }
        return result;
    }
    /*! Returns new stored property factory for a database
        \return stored property factory in database
     */
    sad::db::StoredPropertyFactory* storedPropertyFactory() const;
    /*! Sets a stored property factory for a database. Old factory will
        be deleted.
        \param[in] f a factory.
     */
    void setStoredPropertyFactory(sad::db::StoredPropertyFactory * f);
    /*! Adds new table to database. If table already exists, you can't replace it
        \param[in] name a name for table
        \param[in] table a table name
        \return whether table was added to databases
     */
    bool addTable(const sad::String& name, sad::db::Table* table);
    /*! Removes table from database by name. All objects from it will be freed afterwards.
        \param[in] name a name for table
     */
    void removeTable(const sad::String& name);
    /*! Returns table by it's name 
        \param[in] name a name for a table
        \return a name for a table
    */
    sad::db::Table* table(const sad::String& name);
    /*! Returns new unique major id. Note, that new id is being registered in db for specified table
        \param[in] t a table
        \return new unique major id
     */
    unsigned long long uniqueMajorId(sad::db::Table * t);
    /*! Queries all tables in search of object by name
        \param[in] name name of searched objects
        \return a vector of objects by name
     */
    sad::Vector<sad::db::Object *> queryByName(const sad::String & name) const;
    /*! Queries all tables in search of object by name
        \param[in] name name of searched objects
        \return a vector of objects by name
     */
    sad::db::Object* objectByName(const sad::String & name) const;
    /*! Tries to get objects by name
        \param[in] name object name
        \return objects
     */
    template<typename T>
    sad::Vector<T*>  objectsByName(const sad::String & name) const
    {
        sad::Vector<T*> result;
        const sad::Vector<sad::db::Object *> o = queryByName(name);
        this->filterObjectsByType<T>(result, o);
        return result;
    }
    /*! Tries to get objects by name
        \param[in] name object name
        \return object
     */
    template<typename T>
    T*  objectByName(const sad::String & name) const
    {
        T* result = nullptr;
        const auto o = queryByName(name);
        this->filterObjectByType<T>(result, o);
        return result;
    }
    /*! Queries all tables in search of object by minor id
        \param[in] id a minor id of searched objects
        \return a vector of objects by name
     */
    sad::Vector<sad::db::Object *> queryByMinorId(unsigned long long id) const;
    /*! Tries to get objects by minor id
        \param[in] id object id
        \return objects
     */
    template<typename T>
    sad::Vector<T*>  objectsByMinorId(unsigned long long id) const
    {
        sad::Vector<T*> result;
        const auto o = queryByMinorId(id);
        this->filterObjectsByType<T>(result, o);
        return result;
    }
    /*! Tries to get objects by minor id
        \param[in] id object id
        \return object
     */
    template<typename T>
    T*  objectByMinorId(unsigned long long id) const
    {
        T* result = nullptr;
        const auto o = queryByMinorId(id);
        this->filterObjectByType<T>(result, o);
        return result;
    }
    /*! Queries tables by major id
        \param[in] id  a major if of searched objects
        \return object
     */
    sad::db::Object * queryByMajorId(unsigned long long id) const;
    /*! Tries to get one object by major id
        \param[in] id object id
        \return object
     */
    template<typename T>
    T*  objectByMajorId(unsigned long long id) const
    {
        T* result = nullptr;
        sad::db::Object* o = queryByMajorId(id);
        if (o)
        {
            sad::db::TypeName<T>::init();
            if (o->isInstanceOf(sad::db::TypeName<T>::name()))
            {
                result = static_cast<T*>(o);
            }
        }
        return result;
    }
    /*! Fetches tables from a database
        \param[out] tables a tables in database
     */
    void getTables(sad::Vector<sad::Pair<sad::String, sad::db::Table*> > & tables) const;
    /*! Returns table list from a database
        \return table list
     */
    sad::Vector<sad::Pair<sad::String, sad::db::Table*> > tableList() const;
    /*! Returns property names list
        \return list of properties
     */
    sad::Vector<sad::String> propertyNames() const;
    /*! Returns object factory for a database
        \return  object factory for a database
     */
    sad::db::ObjectFactory* factory();
    /*! Sets object factory for a database
        \param[in] f object factory for a database
     */
    void setFactory(sad::db::ObjectFactory* f);
    /*! Tries to set new max major id in database 
        \param[in] v a value
        \param[in] t a table, which want to set it
     */
    void trySetMaxMajorId(unsigned long long v, sad::db::Table * t);
    /*! Removes major id from hint tables
        \param[in] v a major id
     */
    void removeMajorId(unsigned long long v);
    /*! Sets renderer for database. Database will use this renderer to take executable path from it
        \param[in] r renderer
     */
    void setRenderer(sad::Renderer * r);
    /*! Returns local renderer, if set, otherwise returns global renderer
     */
    sad::Renderer* renderer() const;
    /*! Returns iterator begin for properties
        \return begin iterator
     */
    sad::db::Database::Properties::const_iterator begin() const;
    /*! Returns iterator begin for properties
        \return begin iterator
     */
    sad::db::Database::Properties::const_iterator end() const;
    /*! Sets name for a default tree in database, which objects should link themselves to, when loading
        \param[in] tree a tree name
     */
    void setDefaultTreeName(const sad::String& tree);
    /*! Returns name for a default tree, which objects should link themselves to, when loading
        \return default tree name
     */
    const sad::String& defaultTreeName() const;
    /*! Saves snapshot for database
     */
    void saveSnapshot();
    /*! Returns count of snapshots in database
     */
    unsigned long snapshotsCount() const;
    /*! Restores a database snapshot from index
        \param[in] index index of snapshot
        \return whether it was successfull (false if index is out of range)
     */
    bool restoreSnapshot(unsigned long index = 0);
    /*! Tests, whether all tables are empty
        \return true if all tables are empty
     */
    bool tablesAreEmpty() const;
protected: 
    /*! Clears properties of table
     */
    void clearProperties();
    /*! Loads properties from value
        \param[in] properties a data for properties
        \param[out] new_properties a new properties for database
     */
    bool loadProperties(
        const picojson::object& properties, 
        sad::Hash<sad::String, sad::db::Property*>& new_properties
    );
    /*! Sets properties from specified hash
        \param[in] new_properties a container for properties
     */
    void setPropertiesFrom(const sad::Hash<sad::String, sad::db::Property*>& new_properties);
    /*! Loads properties and tavles from a database
        \param[in] properties a property items
        \param[in] tables a tables from database
        \return whether it was successfull
     */
    bool loadPropertiesAndTables(
        const picojson::object & properties, 
        const picojson::object & tables
    );
    /*! Saves properties into JSON object
        \param[out] o object
     */
    void saveProperties(picojson::object& o);
    /*! Sets default tree name for a database
     */
    sad::String m_default_tree_name;
    /*! Current maximal major id in database. Every object should have major id less,
        than it.
     */
    unsigned long long m_max_major_id;
    /*! A links from major id to specified table
     */
    sad::Hash<unsigned long long, sad::db::Table*> m_majorid_to_table;
    /*! A names of tables, linked to tables
     */
    sad::Hash<sad::String, sad::db::Table*> m_names_to_tables;
    /*! A factory of objects
     */
    sad::db::ObjectFactory* m_factory;
    /*! A stored property factory, used to link objects to properties
     */
    sad::db::StoredPropertyFactory* m_prop_factory;
    /*! A map of properties,linked to database
     */
    sad::db::Database::Properties m_properties;
    /*! Linked renderer in database
     */
    sad::Renderer * m_renderer;
    /*! A snapshots for database
     */
    sad::Vector<sad::db::Database::Snapshot> m_snapshots;
    /*! Filters objects by specific type
        \param[out] result a resulting vector
        \param[in] o objects
     */
    template<typename T>
    static void filterObjectsByType(sad::Vector<T*> & result, const sad::Vector<sad::db::Object*>& o)
    {
        sad::db::TypeName<T>::init();
        for(size_t i = 0; i < o.size(); i++)
        {
            if (o[i]->isInstanceOf(sad::db::TypeName<T>::name()))
            {
                result << static_cast<T*>(o[i]);
            }
        }   
    }
    /*! Filters object by specific type
        \param[out] result a resulting vector
        \param[in] o objects
     */
    template<typename T>
    static void filterObjectByType(T*& result, const sad::Vector<sad::db::Object*>& o)
    {
        sad::db::TypeName<T>::init();
        result = nullptr;
        for(size_t i = 0; i < o.size() && result == nullptr; i++)
        {
            if (o[i]->isInstanceOf(sad::db::TypeName<T>::name()))
            {
                result = static_cast<T*>(o[i]);
            }
        }   
    }
};

}

}

