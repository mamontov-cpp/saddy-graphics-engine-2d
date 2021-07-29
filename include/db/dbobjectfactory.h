/*! \file dbobjectfactory.h
    
    
    A factory for creating objects, by their names. Could be used for deserializing  
    objects from file in database.
 */ 
#pragma once
#include "dbobject.h"
#include "schema/schema.h"
#include "../sadmutex.h"

namespace sad
{

namespace db
{
/*! Describes a factory for creating objects by their name. Used for deserializing 
    objects in database
 */
class ObjectFactory
{
public:

    /*! An abstract delegate for creating items of factory
     */
    class AbstractDelegate
    {
    public:
        /*! Creates new object
            \return object
         */
        virtual sad::db::Object * create() = 0;
        /*! This class can be inherited
         */
        virtual ~AbstractDelegate();
    };
    
    /*!  A delegate for creating object by invoking it's constructor
     */
    template<
        typename T
    >
    class Delegate: public sad::db::ObjectFactory::AbstractDelegate
    {
    public:
        /*! A default delegate for object
         */
        inline Delegate() = default;
        /*! Creates new object
            \return object
         */
        virtual sad::db::Object * create() override
        {
            return new T();
        }
        /*! This class can be inherited
         */
        virtual ~Delegate() override = default;
    };
    /*! Creates new factory with default objects
     */
    ObjectFactory();

    /*! Creates new factory. If empty flag is set, new factory is created empty, without any default data
        \param[in] empty whether factory must be empty
     */
    ObjectFactory(bool empty);
    
    /*! Adds new metadata to factory
        \param[in] name a class name for object
        \param[in] schema a pointer to schema
        \param[in] own  whether factory owns schema
        \param[in] d delegate for creating objects
     */
    virtual void add(
        const sad::String & name,
        sad::db::schema::Schema* schema, 
        bool own, 
        sad::db::ObjectFactory::AbstractDelegate * d
    );
    /*! Adds new metadata to factory
        \param[in] name a class name for object
        \param[in] schema a pointer to schema
        \param[in] own  whether factory owns schema
     */	
    template<
        typename T
    >
    void add(
        const sad::String & name,
        sad::db::schema::Schema* schema, 
        bool own = true
    )
    {
        this->add(name, schema, own, new sad::db::ObjectFactory::Delegate<T>());
    }
    template<
        typename T
    >
    void addCustom(const sad::String & name)
    {
        if (m_special_custom_handlers.contains(name))
        {
            delete m_special_custom_handlers[name]; //-V515
            m_special_custom_handlers.remove(name);
        }
        m_special_custom_handlers.insert(name, new sad::db::ObjectFactory::Delegate<T>());
    }
    /*! Returns schema for an object's class
        \param[in] name  a name of object's class
        \return schema or nullptr, if no schema specified
     */
    virtual sad::db::schema::Schema* schema(const sad::String& name);
    /*! Creates new object, specified by class name
        \param[in] name a name of object's class
        \return object or nullptr, if can't create object 
     */
    virtual sad::db::Object* create(const sad::String& name);
    /*! Creates new object, fetching type from JSON value, mostly checking "type" value
        \param[in] v a value
        \return object or nullptr, if can't create object
     */
    virtual sad::db::Object* createFromEntry(const picojson::value & v);
    /*! This class can be inherited
     */
    virtual ~ObjectFactory();
protected:
    /*! An entry, stored in factory, holding sources to all data
     */
    struct Entry
    {
        /*! Whether entry owns schema and should kill it, when dying
         */
        bool OwnsSchema;
        /*! A schema inside of entry
         */
        sad::db::schema::Schema* Schema;
        /*! A delegate, which could create objects
         */
        sad::db::ObjectFactory::AbstractDelegate* Delegate;
        
        /*! Constructs new entry, from following data
            \param[in] own whether entry owns schema
            \param[in] schema a schema, which is being owned
            \param[in] d a delegate, which could create object
         */
        inline Entry(bool own, sad::db::schema::Schema* schema, sad::db::ObjectFactory::AbstractDelegate* d)
        : OwnsSchema(own), Schema(schema), Delegate(d)
        {
        
        }
        /*! Destroys delegate and schema, if needs to
         */
        ~Entry();
    };

    /*! Initializes factory with default callbacks
     */
    void initWithDefaultCallbacks();
    /*!  A special custom handlers for name and sad::db::custom::Object
     */
    sad::PtrHash<sad::String, sad::db::ObjectFactory::AbstractDelegate> m_special_custom_handlers;
    
    /*! Contains all metadata with class name as a key
     */
    sad::PtrHash<sad::String, sad::db::ObjectFactory::Entry> m_metadata_container;
    
    /*! Lock for creating entries
     */
    sad::Mutex m_lock;
};

}

}
