/*! \file db/dblink.h
    
    
    Describes a link to database object, used to link object to a database.
 */ 
#pragma once
#include "../sadstring.h"

namespace sad
{

namespace db
{
class Object;
class Database;
class Table;

/*! Describes a link to database item, used to link object to a database
 */
class Link
{
public:
    /*! Creates new empty link to an object
     */
    Link();
    /*! This class could be inherited
     */
    virtual ~Link();
    /*! Tries to get cached object from a link
        \return object or null if unable to get one
     */
    sad::db::Object* get();
    /*! Sets database for a link
        \param[in] db a linked database
     */
    void setDatabase(sad::db::Database * db);
    /*! Returns linked database
        \return linked database
     */
    sad::db::Database* database() const;
    /*! Sets a table name for a  link
        \param[in] t a table name
     */
    void setTableName(const sad::String & t);
    /*! Returns a link for a table
        \return a table name 
     */
    const sad::String& tableName() const;
    /*! Sets linked table for a link
        \param[in] t a table for a link
     */
    void setTable(sad::db::Table * t);
    /*! Returns a table for a link
        \return a table for a link
     */
    sad::db::Table* table() const;
    /*! Makes link fetch object by it's major id
        \param[in] major_id a specified major id, which object must have
     */
    void setMajorId(unsigned long long major_id);
    /*! A major id for a link
        \return major id or 0, if objects are linked by other ways
     */
    unsigned long long majorId() const;
    /*! Makes link fetch object by it's minor id
        \param[in] minor_id a specified minor id, which object must have
     */
    void setMinorId(unsigned long long minor_id);
    /*! A minor id for a link
        \return minor id or 0, if objects are linked by name
     */
    unsigned long long minorId() const;
    /*! Makes link fetch first object by it's name
        \param[in] name a specified name of object
     */
    void setName(const sad::String & name);
    /*! Returns name of linked object, it it must be fetched by it or empty string otherwise
        \return name of object or empty string
     */ 
    const sad::String & name() const;
    /*! Explicitly sets cached object
        \param[in] o new object
     */
    void setObject(sad::db::Object* o);
protected:
    /*! An object, cached in a linked
     */
    sad::db::Object* m_cached_object;
    /*! A linked minor id, if we are linking it by id (m_link_by_name is false)
     */
    unsigned long long m_minor_id;
    /*! A linked major id, if we are linking it by id (m_link_by_name is false)
     */
    unsigned long long m_major_id;
    /*! A name of linked object, if we are linking by object
     */
    sad::String m_name;
    /*! Whether we are linking item by name
     */
    bool m_link_by_name;
    /*! Whether we are linking item by major id
     */
    bool m_link_by_major_id;
    /*! A linked table to object
     */
    sad::db::Table * m_table;
    /*! A table name, where link belongs to
     */ 
    sad::String m_table_name;
    /*! A database, where link belongs to
     */
    sad::db::Database * m_database;
};

}

}
