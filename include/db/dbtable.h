/*! \file db/dbtable.h
    
    
    Contains definition of class Table, which represents object table in database.
 */
#pragma once
#include "dbobject.h"
#include "../refcountable.h"

namespace sad
{

namespace db
{

class Database;
class ObjectFactory;
/*! \class Table

    A table, which represents objects in database, which could be used to fetch objects
    dynamically and do some other stuff.
 */
class Table: public sad::RefCountable  
{	
public:	
    /*! Creates new empty table
     */
    Table();
    /*! This class can be inherited 
     */
    virtual ~Table();
    /*! Adds new object to a table
        \param[in] a an object
     */
    virtual void add(sad::db::Object* a);
    /*! Removes objects from table
        \param[in] a an object
     */
    virtual void remove(sad::db::Object* a);
    /*! Queries a table by full key
        \param[in] major_id a major id, stored in DB
        \param[in] minor_id a minor local key
        \return a found object (nullptr if not found) 
     */
    virtual sad::db::Object* queryById(unsigned long long major_id, unsigned long long minor_id);
    /*! Query table by minor id
        \param[in] minor_id a minor id
        \return a found object (nullptr if not found)
     */
    virtual sad::db::Object* queryByMinorId(unsigned long long minor_id);
    /*! Queries a table by name
        \param[in] name a name
        \return a vector of linked object
     */
    virtual sad::Vector<sad::db::Object*> queryByName(const sad::String& name);
    /*! Queries a table by name
        \param[in] name a name
        \return an object or null if not found
     */
    virtual sad::db::Object* objectByName(const sad::String& name);
    /*! Queries objects from table by major id field
        \param[in] major_id an id
        \return	an object, or null if not found
     */
    virtual sad::db::Object* queryByMajorId(unsigned long long major_id);
    /*! Loads table from a  value
        \param[in] v value
        \param[in] factory a factory
        \param[in] renderer a renderer, where should resources, linked to objects be stored
        \param[in] tree_name a name for tree, where should resourced, linked to objects be stored
        \return whether value was successfull
     */
    virtual bool load(
        const picojson::value & v, 
        sad::db::ObjectFactory* factory,
        sad::Renderer* renderer = nullptr,
        const sad::String& tree_name = ""
    );
    /*! Saves a table to a value
        \param[out] v a value for table
     */
    virtual void save(picojson::value & v);
    /*! Returns database, linked with table
        \return database
     */
    sad::db::Database* database() const;
    /*! Sets database for a table
        \param[in] d database
     */
    void setDatabase(sad::db::Database* d);
    /*! Fills vector with objects from table
        \param[out] o objects
     */
    virtual void objects(sad::Vector<sad::db::Object*> & o);
    /*! Returns object list from table
        \return object list from table
     */
    sad::Vector<sad::db::Object*> objectList();
    /*! Returns object list from table with only objects of specified type
        \param[in] s string
        \return object list from table
     */
    sad::Vector<sad::db::Object*> objectListOfType(const sad::String& s);
    /*! Fetches objects of specified type from table
        \param[out] o objects
     */
    template<
        typename T
    >
    void objectsOfType(sad::Vector<T*> & o)
    {
        sad::db::TypeName<T>::init();
        const sad::String type_name = (sad::db::TypeName<T>::name());
        o.clear();
        for(sad::Hash<unsigned long long, sad::db::Object*>::iterator it = m_objects_by_minorid.begin(); 
        it != m_objects_by_minorid.end();
        ++it)
        {
           if (it.value()->isInstanceOf(type_name))
           {
               o << static_cast<T*>(it.value());
           }
        }
    }
    /*! Changes object name in hash table to make container consistent
        \param[in] o object
        \param[in] old_name old name of object
        \param[in] name a name of object
     */
    virtual void changeObjectName(
        sad::db::Object * o, 
        const sad::String & old_name,
        const sad::String & name
    );
    /*! Clears a table
     */
    void clear();
    /*! Returns, whether table is empty
     */
    bool empty() const;
protected: 
    /*! Maximum minor id 
     */
    unsigned long long m_max_minor_id;
    /*! A database, which table is belongs to
     */
    sad::db::Database * m_database;
    /*! Objects, determined by name
     */
    sad::Hash<sad::String, sad::Vector<sad::db::Object*> > m_object_by_name;
    /*! A hash, storing objects by minor id
     */
    sad::Hash<unsigned long long, sad::db::Object*> m_objects_by_minorid;
    /*! A hash, storing objects by major id
     */
    sad::Hash<unsigned long long, sad::db::Object*> m_objects_by_majorid;
};

}

}

